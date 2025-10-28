#include "../../_test_common.h"

// Headers
#include "../../../src/unstdinttypes.h"
#include "../../../src/unstddescriptor.h"
#include <unistd.h>
#include <fcntl.h>

// Function to check if a file descriptor is present in /proc/self/fd/
bool is_fd_in_proc(int fd) {
    char fd_path[256];
    snprintf(fd_path, sizeof(fd_path), "/proc/self/fd/%d", fd);

    return access(fd_path, F_OK) == 0;
}

//! [isvalidfd]
void test_unstddescriptor_isvalidfd(void) {
    // [Succeeds]
    // Creating a new file.
    const char *test_filename = "testfile.txt";
    s32t fd = open(test_filename, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    assert(fd != -1);

    // Trying to check the previously created/opened file.
    assert(unstddescriptor_isvalidfd(fd) == true);

    // Closing the file.
    close(fd);

    // [Fails]
    // Trying to check on a closed file.
    assert(unstddescriptor_isvalidfd(fd) == false);

    // [Fails]
    // Trying to check an invalid file descriptor (-1).
    assert(unstddescriptor_isvalidfd(-1) == false);

    // Check if 9999 is occupied
    if (is_fd_in_proc(9999)) {
        printf("File descriptor 9999 is occupied. Skipping test for FD 9999.\n");
    } else {
        // [Fails]
        // Trying to check a file descriptor that was never opened.
        assert(unstddescriptor_isvalidfd(9999) == false);
    }

    unlink(test_filename);

    _notify("[+]", "`unstddescriptor_isvalidfd()` passed");
}


void test_unstddescriptor(void) {
    test_unstddescriptor_isvalidfd();

    _notify("[+]", "`unstddescriptor` passed");
}
