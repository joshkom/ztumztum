#include "../../_test_common.h"

// Headers
#include "../../../src/unstdinttypes.h"
#include "../../../src/unstdubytes.h"

//! [grow]
void test_unstdubytes_grow(void) {
    u8t result;

    // Initialize buffer with 10 bytes of allocated_len
    ubytes buffer;
    buffer.data = malloc(10);
    buffer.length = 0;
    buffer.capacity = 10;

    // [Succeeds] Grow buffer by 10 bytes
    result = unstdubytes_grow(&buffer, 10);
    assert(result == 1);
    assert(buffer.capacity == 20);

    // [Fails] Null buffer
    result = unstdubytes_grow(NULL, 10);
    assert(result == 2);

    // [Fails] Zero size to grow
    result = unstdubytes_grow(&buffer, 0);
    assert(result == 3);

    // [Fails] Grow operation fails due to memory allocation error (this is hard to force in tests, so omitted)

    // **Byte Stream Tests**
    // Fill buffer with initial data (5 bytes of data)
    memcpy(buffer.data, "ABCDE", 5);
    buffer.length = 5;

    // [Succeeds] Grow buffer by another 20 bytes
    result = unstdubytes_grow(&buffer, 20);
    assert(result == 1);
    assert(buffer.capacity == 40);

    // Make sure the data is still intact after growing
    assert(memcmp(buffer.data, "ABCDE", 5) == 0);

    free(buffer.data);
    _notify("[+]", "`unstdubytes_grow()` passed");
}

//! [writebytes]
void test_unstdubytes_writebytes(void) {
    u8t result;

    // Create destination buffer with a allocated_len of 20 bytes
    ubytes destination_buffer;
    destination_buffer.data = malloc(20);
    destination_buffer.length = 0;
    destination_buffer.capacity = 20;

    // Create source buffer to write
    const u8t source_buffer[] = "Hello";

    // [Succeeds] Write "Hello" into the destination buffer starting at offset 0
    result = unstdubytes_writebytes(&destination_buffer, source_buffer, strlen((const char *) source_buffer), 0);
    assert(result == 1);
    assert(destination_buffer.length == 5);
    assert(memcmp(destination_buffer.data, "Hello", 5) == 0);

    // [Succeeds] Write "World" at offset 5 (appending to "Hello")
    const u8t source_buffer2[] = "World";
    result = unstdubytes_writebytes(&destination_buffer, source_buffer2, strlen((const char *) source_buffer2), 5);
    assert(result == 1);
    assert(destination_buffer.length == 10);
    assert(memcmp(destination_buffer.data, "HelloWorld", 10) == 0);

    // [Fails] Source buffer is NULL
    result = unstdubytes_writebytes(&destination_buffer, NULL, 5, 0);
    assert(result == 3);

    // [Fails] Source buffer size is 0
    result = unstdubytes_writebytes(&destination_buffer, source_buffer, 0, 0);
    assert(result == 4);

    // [Fails] Writing with an offset that exceeds destination buffer's allocated_len
    result = unstdubytes_writebytes(&destination_buffer, source_buffer, 5, 17);
    assert(result == 5);

    // [Fails] Destination buffer is NULL
    result = unstdubytes_writebytes(NULL, source_buffer, 5, 0);
    assert(result == 2);

    // **Byte Stream Tests**
    // Create a byte buffer to write
    const u8t byte_source_buffer[] = {0xDE, 0xAD, 0xBE, 0xEF};

    // [Succeeds] Write the byte stream {0xDE, 0xAD, 0xBE, 0xEF} into destination buffer at offset 0
    result = unstdubytes_writebytes(&destination_buffer, byte_source_buffer, sizeof(byte_source_buffer), 0);
    assert(result == 1);
    assert(destination_buffer.length == 4);
    assert(memcmp(destination_buffer.data, byte_source_buffer, 4) == 0);

    // [Fails] Writing byte stream at offset exceeding allocated_len
    result = unstdubytes_writebytes(&destination_buffer, byte_source_buffer, sizeof(byte_source_buffer), 18);
    assert(result == 5);

    free(destination_buffer.data);
    _notify("[+]", "`unstdubytes_writebytes()` passed");
}

//! [appendbytes]
void test_unstdubytes_appendbytes(void) {
    u8t result;

    // Initialize buffer with 10 bytes of allocated_len
    ubytes destination_buffer;
    destination_buffer.data = malloc(10);
    destination_buffer.length = 0;
    destination_buffer.capacity = 10;

    // Test data to append
    const u8t source_data[] = "Hello";
    sizet source_data_size = sizeof(source_data) - 1; // Exclude null terminator

    // [Succeeds] Append data that fits within the current allocated_len
    result = unstdubytes_appendbytes(&destination_buffer, source_data, source_data_size);
    assert(result == 1);
    assert(destination_buffer.length == source_data_size);
    assert(memcmp(destination_buffer.data, source_data, source_data_size) == 0);

    // [Succeeds] Append data that requires growing the buffer
    const u8t more_data[] = " World!";
    sizet more_data_size = sizeof(more_data) - 1; // Exclude null terminator
    result = unstdubytes_appendbytes(&destination_buffer, more_data, more_data_size);
    assert(result == 1);
    assert(destination_buffer.length == source_data_size + more_data_size);
    assert(destination_buffer.capacity == source_data_size + more_data_size);
    assert(memcmp(destination_buffer.data, "Hello World!", destination_buffer.length) == 0);

    // [Fails] Null destination buffer
    result = unstdubytes_appendbytes(NULL, source_data, source_data_size);
    assert(result == 2);

    // [Fails] Null source buffer
    result = unstdubytes_appendbytes(&destination_buffer, NULL, source_data_size);
    assert(result == 3);

    // [Fails] Zero size to append
    result = unstdubytes_appendbytes(&destination_buffer, source_data, 0);
    assert(result == 4);

    // Clean up
    free(destination_buffer.data);

    // Notify test passed
    _notify("[+]", "`unstdubytes_appendbytes()` passed");
}


void test_unstdubytes(void) {
    test_unstdubytes_grow();
    test_unstdubytes_writebytes();
    test_unstdubytes_appendbytes();

    _notify("[+]", "`unstdubytes` passed");
}