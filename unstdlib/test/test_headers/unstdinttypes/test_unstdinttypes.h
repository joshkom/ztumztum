#include "../../_test_common.h"

// Headers
#include "../../../src/unstdinttypes.h"


//! [unsigned_maximumM]
void test_unstdinttypes_unsigned_maximumM(void) {
    // [Succeeds]
    u8t test_1 = unstdinttypes_unsigned_maximumM(u8t);
    assert(test_1 > 0);
    assert(test_1 != 100);

    u16t test_2 = unstdinttypes_unsigned_maximumM(u16t);
    assert(test_2 > 0);
    assert(test_2 != 100);

    u64lt test_3 = unstdinttypes_unsigned_maximumM(u64lt);
    assert(test_3 > 1024);
    assert(test_3 != 100);

    _notify("[+]", "`unstdinttypes_unsigned_maximumM()` passed");
}

//! [signed_minimumM]
void test_unstdinttypes_signed_minimumM(void) {
    // [Succeeds]
    u8t test_1 = unstdinttypes_signed_minimumM(u8t);
    assert(test_1 == 0);
    assert(test_1 != 100);

    s8t test_2 = unstdinttypes_signed_minimumM(s8t);
    assert(test_2 < -100);
    assert(test_2 != -120);

    s64lt test_3 = unstdinttypes_signed_minimumM(s64lt);
    assert(test_3 < -1024);
    assert(test_3 != -100);

    _notify("[+]", "`unstdinttypes_signed_minimumM()` passed");
}

//! [signed_maximumM]
void test_unstdinttypes_signed_maximumM(void) {
    // [Succeeds]
    u8t test_1 = unstdinttypes_signed_maximumM(u8t);
    assert(test_1 > 0);
    assert(test_1 != 100);

    s8t test_2 = unstdinttypes_signed_maximumM(s8t);
    assert(test_2 > 100);
    assert(test_2 != 120);

    s64lt test_3 = unstdinttypes_signed_maximumM(s64lt);
    assert(test_3 > 1024);
    assert(test_3 != 100);

    _notify("[+]", "`unstdinttypes_signed_maximumM()` passed");
}


void test_unstdinttypes(void) {
    test_unstdinttypes_unsigned_maximumM();
    test_unstdinttypes_signed_minimumM();
    test_unstdinttypes_signed_maximumM();

    _notify("[+]", "`unstdinttypes` passed");
}
