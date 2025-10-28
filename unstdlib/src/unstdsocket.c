#include "unstdsocket.h"


u8t unstdsocket_isvalidipaddr(const i8 *ip_address_arg) {
    if (!ip_address_arg) {
        return 2;
    }

    if (!*ip_address_arg) {
        return 3;
    }

    struct sockaddr_in sa = {};
    int result = inet_pton(AF_INET, ip_address_arg, &(sa.sin_addr));
    if (result < 0) {
        return 4;
    }

    return result == 1;
}

s32t unstdsocket_opentcpfd4(const i8 *destination_ip_address_arg, const u16t destination_port_number_arg) {
    if (!destination_ip_address_arg) {
        return -1;
    }

    if (!*destination_ip_address_arg) {
        return -2;
    }


    u8t isvalidipaddr_result = unstdsocket_isvalidipaddr(
            strcmp(destination_ip_address_arg, "localhost") == 0
            ? "127.0.0.1"
            : destination_ip_address_arg);
    if (isvalidipaddr_result != 1) {
        if (isvalidipaddr_result == 0) {
            return -3;
        } else {
            return -5;
        }
    }


    if (!(destination_port_number_arg > 0 && destination_port_number_arg <= 65535)) {
        return -4;
    }

    s32t socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        return -6;
    }

    struct sockaddr_in sock_addr_in = {};
    memset(&sock_addr_in, 0, sizeof(sock_addr_in));
    sock_addr_in.sin_family = AF_INET;
    sock_addr_in.sin_addr.s_addr = inet_addr(destination_ip_address_arg);
    sock_addr_in.sin_port = htons(destination_port_number_arg);

    if (connect(socket_fd, (struct sockaddr *) &sock_addr_in, sizeof(sock_addr_in)) < 0) {
        close(socket_fd);
        return -7;
    }

    return socket_fd;
}