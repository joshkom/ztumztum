#include "../../_test_common.h"

// Headers
#include "../../../src/unstdinttypes.h"
#include "../../../src/unstdpair.h"

//! [pair_onstackM]
void test_unstdpair_pairM(void) {
    unstdpair_pairM(int, int) test_pair_1 = {.first=1, .second=2};

    // [Succeeds]
    assert(test_pair_1.first == 1);
    assert(test_pair_1.second == 2);

    _notify("[+]", "`test_unstdpair_pair_onstackM()` passed");
}


void test_unstdpair(void) {
    test_unstdpair_pairM();

    _notify("[+]", "`unstdpair` passed");
}
