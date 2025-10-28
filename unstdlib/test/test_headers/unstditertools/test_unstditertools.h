#include "../../_test_common.h"

// Headers
#include "../../../src/unstdinttypes.h"
#include "../../../src/unstditertools.h"


//! [foreach_rangeM]
void test_unstditertools_foreach_rangeM(void) {
    // [Succeeds]
    unstditertools_foreach_rangeM(u8t, i, { 0, 10, 1 }) {
        assert(!(i < 0 || i > 10));
    }

    unstditertools_foreach_rangeM(u8t, i, { 10, 0 }) {
        assert(!(i < 0 || i > 10));

        if (i == 0) { break; }
    }

    unstditertools_foreach_rangeM(u8t, i, { 10, 0, 0 }) {
        assert(!(i < 0 || i > 10));

        if (i == 0) { break; }
    }

    unstditertools_foreach_rangeM(s8t, i, { 10, -2, (s8t) -200 }) {
        assert(!(i < -2 || i > 10));
    }

    unstditertools_foreach_rangeM(s32t, i, { -8, 10, 1 }) {
        assert(!(i < -8 || i > 10));
    }

    unstditertools_foreach_rangeM(s16t, i, { -8, 10, 2 }) {
        assert(!(i < -8 || i > 10 || i == 7 || i == 3));
    }

    _notify("[+]", "`unstditertools_foreach_rangeM()` passed");
}


void test_unstditertools(void) {
    test_unstditertools_foreach_rangeM();

    _notify("[+]", "`unstditertools` passed");
}
