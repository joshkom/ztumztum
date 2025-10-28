#include "../../_test_common.h"

//! [TODO] rewrite all tests and remove comments.

// Headers
#include "../../../src/unstdbitutils.h"
#include "../../../src/unstdinttypes.h"

//! [fitsinbits8]
void test_unstdbitutils_fitsinbits8(void) {
    // [Succeeds]
    assert(unstdbitutils_fitsinbits(0, 8) == true);
    assert(unstdbitutils_fitsinbits(255, 8) == true);
    assert(unstdbitutils_fitsinbits(15, 4) == true);

    // [Fails]
    assert(unstdbitutils_fitsinbits(256, 8) == false);
    assert(unstdbitutils_fitsinbits(16, 4) == false);
    assert(unstdbitutils_fitsinbits(0xFF, 7) == false);

    _notify("[+]", "`unstdbitutils_fitsinbits()` passed");
}

//! [8-bit]
//! [packltrmulti8]
void test_unstdbitutils_packltrmulti8(void) {
    u8t error;

    // [Succeeds]
    assert(unstdbitutils_packltrmulti8(2, &error, 3, 5, 5, 15) == 0b10101111);
    assert(unstdbitutils_packltrmulti8(2, &error, 3, 5, 4, 2) == 0b10100100);
    assert(unstdbitutils_packltrmulti8(1, &error, 4, 7) == 0b01110000);
    assert(unstdbitutils_packltrmulti8(3, &error, 2, 1, 3, 6, 3, 7) == 0b01110111);
    assert(unstdbitutils_packltrmulti8(3, &error, 4, 7, 2, 0, 2, 3) == 0b01110011);

    // [Fails]
    assert(unstdbitutils_packltrmulti8(2, &error, 4, 16, 4, 16) == 0);
    assert(error == 3);
    assert(unstdbitutils_packltrmulti8(2, &error, 3, 7, 5, 64) == 0); // 8 does not fit in 5 bits
    assert(error == 3);
    assert(unstdbitutils_packltrmulti8(2, &error, 4, 7, 5, 31) == 0);
    assert(error == 2);

    _notify("[+]", "`unstdbitutils_packrtlmulti8()` passed");
}

//! [packrtlmulti8]
void test_unstdbitutils_packrtlmulti8(void) {
    u8t error;

    // [Succeeds]
    assert(unstdbitutils_packrtlmulti8(2, &error, 3, 5, 5, 15) == 0b01111101);
    assert(unstdbitutils_packrtlmulti8(1, &error, 4, 7) == 0b00000111);
    assert(unstdbitutils_packrtlmulti8(3, &error, 2, 1, 3, 6, 3, 7) == 0b11111001);
    assert(unstdbitutils_packrtlmulti8(3, &error, 4, 7, 2, 0, 2, 3) == 0b11000111);
    // [Fails]
    assert(unstdbitutils_packrtlmulti8(2, &error, 4, 16, 4, 16) == 0);
    assert(error == 3);
    assert(unstdbitutils_packrtlmulti8(2, &error, 5, 7, 5, 32) == 0);
    assert(error == 2);
    assert(unstdbitutils_packrtlmulti8(2, &error, 4, 7, 5, 31) == 0);
    assert(error == 2);

    // [Edge cases]
    assert(unstdbitutils_packrtlmulti8(0, &error) == 0);
    assert(error == 1);
    assert(unstdbitutils_packrtlmulti8(2, NULL, 3, 5, 5, 15) == 0b01111101);
    assert(unstdbitutils_packrtlmulti8(1, &error, 0, 0) == 0);
    assert(error == 1);

    _notify("[+]", "`unstdbitutils_packrtlmulti8()` passed");
}

//! [unpackltrmulti8]
void test_unstdbitutils_unpackltrmulti8(void) {
    u8t error;
    u8t var1, var2;

    // [Succeeds]
    u8t packed_byte = 0b10101111;
    unstdbitutils_unpackltrmulti8(packed_byte, 2, &error, 3, &var1, 5, &var2);
    assert(var1 == 5);
    assert(var2 == 15);
    unstdbitutils_unpackltrmulti8(packed_byte, 1, &error, 3, &var1);
    assert(var1 == 5);
    unstdbitutils_unpackltrmulti8(packed_byte, 1, &error, 4, &var1);
    assert(var1 == 10);
    unstdbitutils_unpackltrmulti8(packed_byte, 4, &error, 4, NULL, 0, NULL, 1, &var1, 3, &var2);
    assert(var1 == 1);
    assert(var2 == 7);

    // [Fails] Error cases
    packed_byte = 0b01111111;
    unstdbitutils_unpackltrmulti8(packed_byte, 2, &error, 4, &var1, 5, &var2);
    assert(error == 2);
    unstdbitutils_unpackltrmulti8(packed_byte, 2, &error, 9, &var1, 4, &var2);
    assert(error == 2);
    unstdbitutils_unpackltrmulti8(packed_byte, 2, &error, 0, &var1, 9, &var2);
    assert(error == 2);

    // [Edge cases]
    packed_byte = 0b00000000;
    unstdbitutils_unpackltrmulti8(packed_byte, 0, &error);
    assert(error == 1);
    unstdbitutils_unpackltrmulti8(packed_byte, 1, NULL, 3, &var1);
    assert(var1 == 0);

    _notify("[+]", "`unstdbitutils_unpackltrmulti8()` passed");
}

//! [unpackrtlmulti8]
void test_unstdbitutils_unpackrtlmulti8(void) {
    u8t error;
    u8t var1, var2;

    // [Succeeds]
    u8t packed_byte = 0b10101111;
    unstdbitutils_unpackrtlmulti8(packed_byte, 2, &error, 3, &var1, 5, &var2);
    assert(var1 == 7);
    assert(var2 == 21);
    unstdbitutils_unpackrtlmulti8(packed_byte, 1, &error, 4, &var1);
    assert(var1 == 15);
    unstdbitutils_unpackrtlmulti8(packed_byte, 4, &error, 4, NULL, 0, NULL, 1, &var1, 3, &var2);
    assert(var1 == 0);
    assert(var2 == 5);

    // [Fails]
    packed_byte = 0b01111111;
    unstdbitutils_unpackrtlmulti8(packed_byte, 2, &error, 4, &var1, 5, &var2);
    assert(error == 2);
    unstdbitutils_unpackrtlmulti8(packed_byte, 2, &error, 9, &var1, 4, &var2);
    assert(error == 2);
    unstdbitutils_unpackrtlmulti8(packed_byte, 2, &error, 0, &var1, 9, &var2);
    assert(error == 2);

    // [Edge cases]
    packed_byte = 0b00000000;
    unstdbitutils_unpackrtlmulti8(packed_byte, 0, &error);
    assert(error == 1);
    unstdbitutils_unpackrtlmulti8(packed_byte, 1, NULL, 3, &var1);
    assert(var1 == 0);

    _notify("[+]", "`unstdbitutils_unpackrtlmulti8()` passed");
}

//! [16-bit]
//! [packltrmulti16]
void test_unstdbitutils_packltrmulti16(void) {
    u8t error;

    // [Succeeds] Test cases where values fit within the given bits
    assert(unstdbitutils_packltrmulti16(2, &error, 4, 15, 4, 7) ==
        0b1111011100000000); // 4 bits for 15 (1111), 4 bits for 7 (0111)
    assert(unstdbitutils_packltrmulti16(1, &error, 8, 255) == 0b1111111100000000); // 8 bits for 255 (11111111)
    assert(unstdbitutils_packltrmulti16(3, &error, 5, 19, 6, 47, 5, 31) ==
        0b1001110111111111); // 5 bits for 19 (10011), 6 bits for 47 (101111), 5 bits for 31 (11111)

    // [Fails] Error cases
    assert(unstdbitutils_packltrmulti16(2, &error, 5, 31, 12, 4095) == 0); // 12 bits for 4095 exceeds 16 bits
    assert(error == 2);

    assert(unstdbitutils_packltrmulti16(2, &error, 9, 511, 9, 511) == 0); // 9 bits per field exceeds 16 bits total
    assert(error == 2);

    _notify("[+]", "`unstdbitutils_packltrmulti16()` passed");
}

//! [packrtlmulti16]
void test_unstdbitutils_packrtlmulti16(void) {
    u8t error;

    // [Succeeds] Test cases where values fit within the given bits
    assert(unstdbitutils_packrtlmulti16(2, &error, 4, 15, 4, 7) ==
        0b0000000001111111); // 4 bits for 15 (1111), 4 bits for 7 (0111)
    assert(unstdbitutils_packrtlmulti16(1, &error, 8, 255) == 0b0000000011111111); // 8 bits for 255 (11111111)
    assert(unstdbitutils_packrtlmulti16(3, &error, 5, 19, 6, 47, 5, 31) ==
        0b1111110111110011); // 5 bits for 19 (10011), 6 bits for 47 (101111), 5 bits for 31 (11111)
    assert(unstdbitutils_packrtlmulti16(2, &error, 0, 255, 5, 7) == 7); // Bit size of 0 is invalid
    assert(error == 1);

    // [Fails] Error cases
    assert(unstdbitutils_packrtlmulti16(2, &error, 5, 31, 12, 4095) == 0); // 12 bits for 4095 exceeds 16 bits
    assert(error == 2);

    assert(unstdbitutils_packrtlmulti16(2, &error, 9, 511, 9, 511) == 0); // 9 bits per field exceeds 16 bits total
    assert(error == 2);

    _notify("[+]", "`unstdbitutils_packrtlmulti16()` passed");
}

//! [unpackltrmulti16]
void test_unstdbitutils_unpackltrmulti16(void) {
    u8t error;
    u16t var1, var2, var3;

    // [Succeeds]
    u16t packed_value = 0b1111011100000000; // 4 bits for 15 (1111), 4 bits for 7 (0111)
    unstdbitutils_unpackltrmulti16(packed_value, 2, &error, 4, &var1, 4, &var2);
    assert(var1 == 15); // Last 4 bits
    assert(var2 == 7); // Next 4 bits

    packed_value = 0b1111111100000000; // 8 bits for 255
    unstdbitutils_unpackltrmulti16(packed_value, 1, &error, 8, &var1);
    assert(var1 == 255); // Last 8 bits

    packed_value = 0b1001110111111111; // 5 bits for 19, 6 bits for 47, 5 bits for 31
    unstdbitutils_unpackltrmulti16(packed_value, 3, &error, 5, &var1, 6, &var2, 5, &var3);
    assert(var1 == 19); // Last 5 bits
    assert(var2 == 47); // Next 6 bits
    assert(var3 == 31); // Next 5 bits

    // [Fails] Error cases
    packed_value = 0b1111011100000000;

    unstdbitutils_unpackltrmulti16(packed_value, 2, &error, 9, &var1, 8, &var2);
    assert(error == 2); // Not enough bits for the first field

    unstdbitutils_unpackltrmulti16(packed_value, 2, &error, 4, &var1, 17, &var2);
    assert(error == 2); // Bit field size of 17 is invalid

    unstdbitutils_unpackltrmulti16(packed_value, 2, &error, 0, &var1, 5, &var2);
    assert(error == 1);

    // [Edge cases]
    packed_value = 0x0000; // All bits are 0
    unstdbitutils_unpackltrmulti16(packed_value, 0, &error);
    assert(error == 1); // No pairs provided

    unstdbitutils_unpackltrmulti16(packed_value, 1, NULL, 16, &var1);
    assert(var1 == 0); // No error output pointer, should default to 0

    _notify("[+]", "`unstdbitutils_unpackltrmulti16()` passed");
}

//! [unpackrtlmulti16]
void test_unstdbitutils_unpackrtlmulti16(void) {
    u8t error;
    u16t var1, var2, var3;

    // [Succeeds]
    u16t packed_value = 0b0000000001111111; // 4 bits for 15 (1111), 4 bits for 7 (0111)
    unstdbitutils_unpackrtlmulti16(packed_value, 2, &error, 4, &var1, 4, &var2);
    assert(var1 == 15); // First 4 bits
    assert(var2 == 7); // Next 4 bits

    packed_value = 0b0000000011111111; // 8 bits for 255
    unstdbitutils_unpackrtlmulti16(packed_value, 1, &error, 8, &var1);
    assert(var1 == 255); // First 8 bits

    packed_value = 0b1001110111111111; // 5 bits for 19, 6 bits for 47, 5 bits for 31
    unstdbitutils_unpackrtlmulti16(packed_value, 3, &error, 5, &var1, 6, &var2, 5, &var3);
    assert(var1 == 31); // First 5 bits
    assert(var2 == 47); // Next 6 bits
    assert(var3 == 19); // Next 5 bits

    // [Fails] Error cases
    packed_value = 0b1111011100000000;

    unstdbitutils_unpackrtlmulti16(packed_value, 2, &error, 5, &var1, 16, &var2);
    assert(error == 2); // Not enough bits for the second field

    unstdbitutils_unpackrtlmulti16(packed_value, 2, &error, 16, &var1, 33, &var2);
    assert(error == 2); // Bit field size of 33 is invalid

    // [Edge cases]
    packed_value = 0x0000; // All bits are 0
    unstdbitutils_unpackrtlmulti16(packed_value, 0, &error);
    assert(error == 1); // No pairs provided

    unstdbitutils_unpackrtlmulti16(packed_value, 1, NULL, 16, &var1);
    assert(var1 == 0); // No error output pointer, should default to 0

    _notify("[+]", "`unstdbitutils_unpackrtlmulti16()` passed");
}

//! [packltrmulti32]
void test_unstdbitutils_packltrmulti32(void) {
    u8t error;

    // [Succeeds] Test cases where values fit within the given bits
    assert(unstdbitutils_packltrmulti32(2, &error, 8, 255, 8, 127) ==
        0b1111111101111111); // 8 bits for 255 (11111111), 8 bits for 127 (01111111)
    assert(unstdbitutils_packltrmulti32(1, &error, 16, 65535) == 0b1111111111111111);
    // 16 bits for 65535 (1111111111111111)
    assert(unstdbitutils_packltrmulti32(3, &error, 10, 1023, 11, 2047, 8, 255) ==
        0b1111111111111111111111111111);
    // 10 bits for 1023 (1111111111), 11 bits for 2047 (011111111111), 8 bits for 255 (11111111)

    // [Fails] Error cases
    assert(unstdbitutils_packltrmulti32(2, &error, 16, 65535, 16, 65535) == 0);
    // 16 bits for each field, exceeds 32 bits total
    assert(error == 2);

    assert(unstdbitutils_packltrmulti32(2, &error, 17, 131071, 17, 131071) == 0);
    // 17 bits per field exceeds 32 bits total
    assert(error == 2);

    assert(unstdbitutils_packltrmulti32(2, &error, 0, 255, 8, 7) == 0); // Bit size of 0 is invalid
    assert(error == 2);

    _notify("[+]", "`unstdbitutils_packltrmulti32()` passed");
}

//! [packrtlmulti32]
void test_unstdbitutils_packrtlmulti32(void) {
    u8t error;

    // [Succeeds] Test cases where values fit within the given bits
    assert(unstdbitutils_packrtlmulti32(2, &error, 8, 255, 8, 127) ==
        0b1111111101111111); // 8 bits for 255 (11111111), 8 bits for 127 (01111111)
    assert(unstdbitutils_packrtlmulti32(1, &error, 16, 65535) == 0b1111111111111111);
    // 16 bits for 65535 (1111111111111111)
    assert(unstdbitutils_packrtlmulti32(3, &error, 10, 1023, 11, 2047, 8, 255) ==
        0b1111111111111111111111111111);
    // 10 bits for 1023 (1111111111), 11 bits for 2047 (011111111111), 8 bits for 255 (11111111)

    // [Fails] Error cases
    assert(unstdbitutils_packrtlmulti32(2, &error, 16, 65535, 16, 65535) == 0);
    // 16 bits for each field, exceeds 32 bits total
    assert(error == 2);

    assert(unstdbitutils_packrtlmulti32(2, &error, 17, 131071, 17, 131071) == 0);
    // 17 bits per field exceeds 32 bits total
    assert(error == 2);

    assert(unstdbitutils_packrtlmulti32(2, &error, 0, 255, 8, 7) == 0); // Bit size of 0 is invalid
    assert(error == 2);

    _notify("[+]", "`unstdbitutils_packrtlmulti32()` passed");
}

//! [unpackltrmulti32]
void test_unstdbitutils_unpackltrmulti32(void) {
    u8t error;
    u32t var1, var2, var3;

    // [Succeeds]
    u32t packed_value = 0b1111111101111111; // 8 bits for 255 (11111111), 8 bits for 127 (01111111)
    unstdbitutils_unpackltrmulti32(packed_value, 2, &error, 8, &var1, 8, &var2);
    assert(var1 == 255); // Last 8 bits
    assert(var2 == 127); // Next 8 bits

    packed_value = 0b1111111111111111; // 16 bits for 65535
    unstdbitutils_unpackltrmulti32(packed_value, 1, &error, 16, &var1);
    assert(var1 == 65535); // Last 16 bits

    packed_value = 0b1111111111111111111111111111; // 10 bits for 1023, 11 bits for 2047, 8 bits for 255
    unstdbitutils_unpackltrmulti32(packed_value, 3, &error, 10, &var1, 11, &var2, 8, &var3);
    assert(var1 == 1023); // Last 10 bits
    assert(var2 == 2047); // Next 11 bits
    assert(var3 == 255); // Next 8 bits

    // [Fails] Error cases
    packed_value = 0b1111111101111111;

    unstdbitutils_unpackltrmulti32(packed_value, 2, &error, 9, &var1, 8, &var2);
    assert(error == 2); // Not enough bits for the first field

    unstdbitutils_unpackltrmulti32(packed_value, 2, &error, 16, &var1, 33, &var2);
    assert(error == 2); // Bit field size of 33 is invalid

    unstdbitutils_unpackltrmulti32(packed_value, 2, &error, 0, &var1, 8, &var2);
    assert(error == 2); // Bit field size of 0 is invalid

    // [Edge cases]
    packed_value = 0x00000000; // All bits are 0
    unstdbitutils_unpackltrmulti32(packed_value, 0, &error);
    assert(error == 1); // No pairs provided

    unstdbitutils_unpackltrmulti32(packed_value, 1, NULL, 32, &var1);
    assert(var1 == 0); // No error output pointer, should default to 0

    _notify("[+]", "`unstdbitutils_unpackltrmulti32()` passed");
}

//! [unpackrtlmulti32]
void test_unstdbitutils_unpackrtlmulti32(void) {
    u8t error;
    u32t var1, var2, var3;

    // [Succeeds]
    u32t packed_value = 0b1111111101111111; // 8 bits for 255 (11111111), 8 bits for 127 (01111111)
    unstdbitutils_unpackrtlmulti32(packed_value, 2, &error, 8, &var1, 8, &var2);
    assert(var1 == 255); // First 8 bits
    assert(var2 == 127); // Next 8 bits

    packed_value = 0b1111111111111111; // 16 bits for 65535
    unstdbitutils_unpackrtlmulti32(packed_value, 1, &error, 16, &var1);
    assert(var1 == 65535); // First 16 bits

    packed_value = 0b1111111111111111111111111111; // 10 bits for 1023, 11 bits for 2047, 8 bits for 255
    unstdbitutils_unpackrtlmulti32(packed_value, 3, &error, 10, &var1, 11, &var2, 8, &var3);
    assert(var1 == 255); // First 10 bits
    assert(var2 == 2047); // Next 11 bits
    assert(var3 == 1023); // Next 8 bits

    // [Fails] Error cases
    packed_value = 0b1111111101111111;

    unstdbitutils_unpackrtlmulti32(packed_value, 2, &error, 11, &var1, 16, &var2);
    assert(error == 2); // Not enough bits for the second field

    unstdbitutils_unpackrtlmulti32(packed_value, 2, &error, 16, &var1, 33, &var2);
    assert(error == 2); // Bit field size of 33 is invalid

    unstdbitutils_unpackrtlmulti32(packed_value, 2, &error, 0, &var1, 8, &var2);
    assert(error == 2); // Bit field size of 0 is invalid

    // [Edge cases]
    packed_value = 0x00000000; // All bits are 0
    unstdbitutils_unpackrtlmulti32(packed_value, 0, &error);
    assert(error == 1); // No pairs provided

    unstdbitutils_unpackrtlmulti32(packed_value, 1, NULL, 32, &var1);
    assert(var1 == 0); // No error output pointer, should default to 0

    _notify("[+]", "`unstdbitutils_unpackrtlmulti32()` passed");
}


void test_unstdbitutils(void) {
    //! [8-bit]
    test_unstdbitutils_fitsinbits8();
    test_unstdbitutils_packltrmulti8();
    test_unstdbitutils_packrtlmulti8();
    test_unstdbitutils_unpackltrmulti8();
    test_unstdbitutils_unpackrtlmulti8();

    //! [16-bit]
    test_unstdbitutils_packltrmulti16();
    test_unstdbitutils_packrtlmulti16();
    test_unstdbitutils_unpackltrmulti16();
    test_unstdbitutils_unpackrtlmulti16();


    _notify("[+]", "`unstdbitutils` passed");
}
