#include "unstdio_compat.h"

//ssize_t unstdio_compat_fd_fgets(int fd, char *buf, size_t n) {
//    if (!buf || n == 0) {
//        return -1;
//    }
//
//    size_t i = 0;
//    char c;
//    ssize_t r;
//
//    while (i < n - 1) {
//        r = read(fd, &c, 1);
//        if (r < 0) {
//            return -1;
//        }
//        if (r == 0) {
//            break;
//        }
//
//        buf[i++] = c;
//        if (c == '\n') {
//            break;
//        }
//    }
//
//    buf[i] = '\0';
//    return i;
//}
