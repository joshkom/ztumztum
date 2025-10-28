#include "frida_protocol_protection.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/time.h>
#include <ifaddrs.h>
#include <net/if.h>

#include "TaleProtect/tools/utils.h"
#include "TaleProtect/demolisher/threads_kill_recursive.h"
#include "TaleProtect/demolisher/smash.h"
#include <unstdprintf_compat.h>
#include <unstdstring_compat.h>

static const char dbus_auth_probe[] = "\x00AUTH\r\n";
static const size_t dbus_auth_probe_len = sizeof(dbus_auth_probe) - 1;

#define DEFAULT_FRIDA_PORT 27042
#define MAX_LOCAL_ADDRS 32

static void try_spawn_demolisher_on_detection(void) {
    LOGW("(FridaProtocol) Spawning demolisher due to detection...");
    pthread_t t;
    if (pthread_createM(&t, NULL, (void *(*)(void *)) demolisher_thread_kills_recursive, NULL)) {
        LOGE("(FridaProtocol) Failed to spawn demolisher thread. Smashing immediately!");
        demolisher_smash();
    } else {
        LOGD("(FridaProtocol) Demolisher thread created and detached.");
        pthread_detach(t);
    }
}

static int probe_port_for_frida_ip(const char *ip_str, uint16_t port, int timeout_ms) {
    LOGD("(FridaProtocol) Probing %s:%d", ip_str, port);
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        LOGE("(FridaProtocol) Socket creation failed for %s:%d: %s", ip_str, port, strerror(errno));
        return -1;
    }

    int flags = fcntl(sock, F_GETFL, 0);
    if (flags >= 0) fcntl(sock, F_SETFL, flags | O_NONBLOCK);

    struct sockaddr_in sa;
    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_port = htons(port);
    if (inet_pton(AF_INET, ip_str, &sa.sin_addr) != 1) {
        LOGE("(FridaProtocol) Invalid IP string: %s", ip_str);
        close(sock);
        return -1;
    }

    int c = connect(sock, (struct sockaddr *) &sa, sizeof(sa));
    if (c < 0 && errno != EINPROGRESS) {
        LOGD("(FridaProtocol) Immediate connect failed for %s:%d: %s", ip_str, port,
             strerror(errno));
        close(sock);
        return 0;
    }

    if (c < 0 && errno == EINPROGRESS) {
        struct timeval tv;
        tv.tv_sec = timeout_ms / 1000;
        tv.tv_usec = (timeout_ms % 1000) * 1000;
        fd_set wfds;
        FD_ZERO(&wfds);
        FD_SET(sock, &wfds);

        int s = select(sock + 1, NULL, &wfds, NULL, &tv);
        if (s <= 0) {
            LOGD("(FridaProtocol) Connect timeout for %s:%d", ip_str, port);
            close(sock);
            return 0;
        }
        int soerr = 0;
        socklen_t len = sizeof(soerr);
        if (getsockopt(sock, SOL_SOCKET, SO_ERROR, &soerr, &len) < 0 || soerr != 0) {
            LOGD("(FridaProtocol) Connect failed (SO_ERROR=%d) for %s:%d", soerr, ip_str, port);
            close(sock);
            return 0;
        }
    }

    if (flags >= 0) fcntl(sock, F_SETFL, flags & ~O_NONBLOCK);

    ssize_t sret = send(sock, dbus_auth_probe, dbus_auth_probe_len, 0);
    if (sret != (ssize_t) dbus_auth_probe_len) {
        LOGD("(FridaProtocol) Failed to send probe to %s:%d", ip_str, port);
        close(sock);
        return 0;
    }

    utils_msleep(100);

    char recvbuf[128];
    ssize_t rlen = recv(sock, recvbuf, sizeof(recvbuf) - 1, MSG_DONTWAIT);
//    ssize_t rlen = recv(sock, recvbuf, sizeof(recvbuf) - 1, 0);
    int ret = 0;
    if (rlen > 0) {
        recvbuf[rlen] = '\0';
        LOGD("(FridaProtocol) Received response from %s:%d -> %s", ip_str, port, recvbuf);
        if (unstdstring_compat_strcasestr(recvbuf, "REJECT") != NULL
            || unstdstring_compat_strcasestr(recvbuf, "REPLY") != NULL
            || unstdstring_compat_strcasestr(recvbuf, "OK") != NULL) {
            ret = 1;
        }
    } else if (rlen == 0) {
        LOGD("(FridaProtocol) Connection closed by %s:%d", ip_str, port);
    } else {
        LOGD("(FridaProtocol) No response from %s:%d (errno=%d)", ip_str, port, errno);
    }

    close(sock);
    return ret;
}

/* Collect local IPv4 addresses */
static int collect_local_ipv4_addresses(char out_addrs[][INET_ADDRSTRLEN], int max_addrs) {
    struct ifaddrs *ifaddr = NULL;
    if (getifaddrs(&ifaddr) != 0) {
        LOGE("(FridaProtocol) getifaddrs failed: %s", strerror(errno));
        return -1;
    }

    int cnt = 0;
    for (struct ifaddrs *ifa = ifaddr; ifa && cnt < max_addrs; ifa = ifa->ifa_next) {
        if (!ifa->ifa_addr || ifa->ifa_addr->sa_family != AF_INET) continue;
        if (!(ifa->ifa_flags & IFF_UP)) continue;

        struct sockaddr_in *sa = (struct sockaddr_in *) ifa->ifa_addr;
        char addrbuf[INET_ADDRSTRLEN];
        if (!inet_ntop(AF_INET, &sa->sin_addr, addrbuf, sizeof(addrbuf))) continue;
        if (strcmp(addrbuf, "0.0.0.0") == 0) continue;

        int dup = 0;
        for (int i = 0; i < cnt; ++i) {
            if (strcmp(out_addrs[i], addrbuf) == 0) {
                dup = 1;
                break;
            }
        }
        if (dup) continue;

        strncpy(out_addrs[cnt], addrbuf, INET_ADDRSTRLEN);
        out_addrs[cnt][INET_ADDRSTRLEN - 1] = '\0';
        LOGD("(FridaProtocol) Found local interface: %s", out_addrs[cnt]);
        ++cnt;
    }

    freeifaddrs(ifaddr);
    return cnt;
}

static void *frida_protocol_thread(void *arg) {
    (void) arg;
    const int scan_sleep_seconds = 2;
    const int probe_timeout_ms = 250;
    char local_addrs[MAX_LOCAL_ADDRS][INET_ADDRSTRLEN];

    while (1) {
        LOGD("(FridaProtocol) Starting scan cycle...");

        int rc = probe_port_for_frida_ip("127.0.0.1", DEFAULT_FRIDA_PORT, probe_timeout_ms);
        if (rc == 1) {
            LOGE("(FridaProtocol) FRIDA DETECTED on 127.0.0.1:%d!", DEFAULT_FRIDA_PORT);
            try_spawn_demolisher_on_detection();
            utils_msleep(scan_sleep_seconds * 1000);
            continue;
        }
        LOGD("(FridaProtocol) No Frida on 127.0.0.1:%d, checking other local IPs...",
             DEFAULT_FRIDA_PORT);

        int naddrs = collect_local_ipv4_addresses(local_addrs, MAX_LOCAL_ADDRS);
        if (naddrs < 0) {
            LOGD("(FridaProtocol) Failed to enumerate interfaces, proceeding with loopback only");
            naddrs = 0;
        }

        for (int i = 0; i < naddrs; ++i) {
            if (strcmp(local_addrs[i], "127.0.0.1") == 0) continue;
            int r2 = probe_port_for_frida_ip(local_addrs[i], DEFAULT_FRIDA_PORT, probe_timeout_ms);
            if (r2 == 1) {
                LOGE("(FridaProtocol) FRIDA DETECTED on %s:%d!", local_addrs[i],
                     DEFAULT_FRIDA_PORT);
                try_spawn_demolisher_on_detection();
                goto sleep_and_continue;
            }
        }

        for (int i = -1; i < naddrs; ++i) {
            const char *addr_to_use = (i == -1) ? "127.0.0.1" : local_addrs[i];
            for (uint32_t port = 0; port <= 65535; ++port) {
                if (port == (uint32_t) DEFAULT_FRIDA_PORT) continue;
                int r = probe_port_for_frida_ip(addr_to_use, (uint16_t) port, probe_timeout_ms);
                if (r < 0) continue;
                if (r == 1) {
                    LOGE("(FridaProtocol) FRIDA DETECTED on %s:%u!", addr_to_use, port);
                    try_spawn_demolisher_on_detection();
                    goto sleep_and_continue;
                }
                if ((port & 0x3FF) == 0)
                    LOGD("(FridaProtocol) Scanned port %u on %s", port, addr_to_use);
            }
        }

        sleep_and_continue:
        LOGD("(FridaProtocol) Scan cycle complete, sleeping %d seconds", scan_sleep_seconds);
        utils_msleep(scan_sleep_seconds * 1000);
    }

    return NULL;
}

void frida_protocol_protection(void) {
    LOGD("(FridaProtocol) Initializing frida protocol protection...");
    pthread_t tid;
    if (pthread_createM(&tid, NULL, frida_protocol_thread, NULL)) {
        LOGE("(FridaProtocol) Failed to create protection thread. Smashing immediately!");
        demolisher_smash();
    } else {
        LOGD("(FridaProtocol) Protection thread created successfully.");
        pthread_detachM(tid);
    }
}
