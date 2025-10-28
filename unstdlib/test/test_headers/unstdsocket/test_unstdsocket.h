#include "../../_test_common.h"

// Headers
#include "../../../src/unstdinttypes.h"
#include "../../../src/unstdsocket.h"

//! [pair_onstackM]
void test_unstdsocket_isvalidipaddr(void) {
    u8t test_result = unstdsocket_isvalidipaddr("lol");

    assert(test_result != 1);

    _notify("[+]", "`test_unstdpair_pair_onstackM()` passed");
}


void test_unstdsocket(void) {
    test_unstdsocket_isvalidipaddr();

    _notify("[+]", "`unstdpair` passed");
}
