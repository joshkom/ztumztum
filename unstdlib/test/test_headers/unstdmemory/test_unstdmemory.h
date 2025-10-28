#include "../../_test_common.h"

// Headers
#include "../../../src/unstdinttypes.h"
#include "../../../src/unstdmemory.h"


//! [findbytesindex]
void test_unstdmemory_findbytesindex(void) {
    u8t error;
    sizet index;

    u8t haystack[] = "hello world, this is a test";
    u8t needle[] = "world";
    u8t empty_needle[] = "";

    // [Succeeds] Find "world" in the haystack starting at position 0
    index = unstdmemory_findbytesindex(haystack, sizeof(haystack) - 1, needle, sizeof(needle) - 1, 0, &error);
    assert(index == 6);
    assert(error == 1);

    // [Succeeds] Find "test" in the haystack starting at position 18
    u8t needle2[] = "test";
    index = unstdmemory_findbytesindex(haystack, sizeof(haystack) - 1, needle2, sizeof(needle2) - 1, 18, &error);
    assert(index == 23);
    assert(error == 1);

    // [Fails] Find "test" starting after the haystack's end
    index = unstdmemory_findbytesindex(haystack, sizeof(haystack) - 1, needle2, sizeof(needle2) - 1, 30, &error);
    assert(index == 0);
    assert(error == 4);

    // [Fails] Needle not found
    u8t needle_not_found[] = "missing";
    index = unstdmemory_findbytesindex(haystack, sizeof(haystack) - 1, needle_not_found, sizeof(needle_not_found) - 1,
                                       0, &error);
    assert(index == 0);
    assert(error == 6);

    // [Succeeds] Empty needle returns the position (within bounds)
    index = unstdmemory_findbytesindex(haystack, sizeof(haystack) - 1, empty_needle, 0, 5, &error);
    assert(index == 5);
    assert(error == 3);

    // [Fails] Empty haystack
    u8t empty_haystack[] = "";
    index = unstdmemory_findbytesindex(empty_haystack, 0, needle, sizeof(needle) - 1, 0, &error);
    assert(index == 0);
    assert(error == 2);

    // **Byte Stream Tests**
    // Test using raw byte streams instead of strings
    u8t haystack_bytes[] = {0xDE, 0xAD, 0xBE, 0xEF, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66};
    u8t needle_bytes[] = {0x11, 0x22, 0x33};

    // [Succeeds] Find byte sequence {0x11, 0x22, 0x33} in byte stream starting at 0
    index = unstdmemory_findbytesindex(haystack_bytes, sizeof(haystack_bytes), needle_bytes, sizeof(needle_bytes), 0,
                                       &error);
    assert(index == 4);
    assert(error == 1);

    u8t haystack_bytes2[] = {0xDE, 0xAD, 0xBE, 0xEF, 0x11, 0x22, 0x33, 0x44, 0xB0, 0x0B, 0xFA};
    u8t needle_bytes2[] = {0xB0, 0x0B};

    // [Succeeds] Find byte sequence {0x11, 0x22, 0x33} in byte stream starting at 0
    index = unstdmemory_findbytesindex(haystack_bytes2, sizeof(haystack_bytes2), (const u8t *) &needle_bytes2,
                                       sizeof(needle_bytes2), 0,
                                       &error);
    assert(index == 8);
    assert(error == 1);

    // [Fails] Find byte sequence starting at position 7 (beyond range)
    index = unstdmemory_findbytesindex(haystack_bytes, sizeof(haystack_bytes), needle_bytes, sizeof(needle_bytes), 10,
                                       &error);
    assert(index == 0);
    assert(error == 4);

    // [Fails] Byte dont fit
    u8t needle_wont_fit_bytes[] = {0xFF, 0xAA, 0xBB};
    index = unstdmemory_findbytesindex(haystack_bytes, sizeof(haystack_bytes), needle_wont_fit_bytes,
                                       sizeof(needle_wont_fit_bytes), 8, &error);
    assert(index == 0);
    assert(error == 5);

    // [Fails] Byte sequence not found
    u8t needle_not_found_bytes[] = {0xFF, 0xAA};
    index = unstdmemory_findbytesindex(haystack_bytes, sizeof(haystack_bytes), needle_not_found_bytes,
                                       sizeof(needle_not_found_bytes), 0, &error);
    assert(index == 0);
    assert(error == 6);

    _notify("[+]", "`unstdmemory_findbytesindex()` passed");
}

//! [erasebytes]
void test_unstdmemory_erasebytes(void) {
    u8t error;

    // Initial buffer and len
    u8t *buffer = malloc(25);
    strcpy((char *) buffer, "This is a test string.");
    sizet buffer_length = strlen((char *) buffer);

    // [Succeeds] Erase 5 bytes starting at position 10
    error = unstdmemory_erasebytes(buffer, buffer_length, 10, 5);
    buffer[10 + buffer_length - 10 - 5] = 0;
    buffer_length -= 5;
    assert(error == 1);
    assert(strcmp((char *) buffer, "This is a string.") == 0);

    // [Succeeds] Erase from position 0, effectively removing "This"
    error = unstdmemory_erasebytes(buffer, buffer_length, 0, 4);
    buffer[0 + buffer_length - 0 - 4] = 0;
    buffer_length -= 4;
    assert(error == 1);
    assert(strcmp((char *) buffer, " is a string.") == 0);

    // [Fails] Attempt to erase beyond the buffer len
    error = unstdmemory_erasebytes(buffer, buffer_length, 20, 5);
    assert(error == 2);

    // [Fails] Zero bytes to erase
    error = unstdmemory_erasebytes(buffer, buffer_length, 0, 0);
    assert(error == 3);

    // [Succeeds] Erase more bytes than remaining in the buffer (should erase until the end)
    error = unstdmemory_erasebytes(buffer, buffer_length, 5, 100); // Request to erase 100 bytes
    buffer[5] = 0;
    buffer_length = 5;
    assert(error == 1);
    assert(strcmp((char *) buffer, " is a") == 0);

    free(buffer);

    // **Byte Stream Tests**
    // Test byte erasure on raw byte buffers
    u8t *byte_buffer = malloc(10);
    memcpy(byte_buffer, (u8t[]) {0xDE, 0xAD, 0xBE, 0xEF, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66}, 10);
    sizet byte_buffer_length = 10;

    // [Succeeds] Erase 3 bytes starting at position 4
    error = unstdmemory_erasebytes(byte_buffer, byte_buffer_length, 4, 3);
    assert(error == 1);
    assert(memcmp(byte_buffer, (u8t[]) {0xDE, 0xAD, 0xBE, 0xEF, 0x44, 0x55, 0x66}, 7) == 0);

    // [Fails] Attempt to erase beyond the buffer len
    error = unstdmemory_erasebytes(byte_buffer, byte_buffer_length, 10, 2);
    assert(error == 2);

    free(byte_buffer);

    _notify("[+]", "`unstdmemory_erasebytes()` passed");
}


void test_unstdmemory(void) {
    test_unstdmemory_findbytesindex();
    test_unstdmemory_erasebytes();

    _notify("[+]", "`unstdmemory` passed");
}
