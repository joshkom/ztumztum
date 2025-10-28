#include "unstddescriptor.h"
#include <fcntl.h>
#include <errno.h>


bool unstddescriptor_isvalidfd(const s32t fd_arg) {
    return fcntl(fd_arg, F_GETFD) != -1 || errno != EBADF;
}

u8t unstddescriptor_setfdtononblock(const s32t fd_arg) {
    if (fd_arg < 0) {
        return 2;
    }

    s32t flags;
    if ((flags = fcntl(fd_arg, F_GETFL, 0)) == -1) {
        return 3;
    }

    if (fcntl(fd_arg, F_SETFL, flags | O_NONBLOCK) == -1) {
        return 4;
    }

    return 1;
}
