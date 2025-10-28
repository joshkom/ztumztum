#include "../../_test_common.h"

// Headers
#include "../../../src/unstdinttypes.h"
#include "../../../src/unstdbytebuffer.h"

//! [new]
void test_unstdbytebuffer_new(void) {
    u8t err;

    // [1] Success case with preallocation within limits
    unstdbytebuffer *b1 = unstdbytebuffer_new(16, 32, &err);
    assert(b1 != NULL);
    assert(err == 1);
    assert(b1->buffer.capacity == 16);
    assert(b1->buffer.data != NULL);
    free(b1->buffer.data);
    free(b1);

    // [1] Success with preallocation > max_capacity → capped
    unstdbytebuffer *b2 = unstdbytebuffer_new(64, 32, &err);
    assert(b2 != NULL);
    assert(err == 1);
    assert(b2->buffer.capacity == 32);
    assert(b2->buffer.data != NULL);
    free(b2->buffer.data);
    free(b2);

    // [1] Success with preallocation = 0 → data should be NULL
    unstdbytebuffer *b3 = unstdbytebuffer_new(0, 128, &err);
    assert(b3 != NULL);
    assert(err == 1);
    assert(b3->buffer.data == NULL);
    free(b3);

    _notify("[+]", "`unstdbytebuffer_new()` passed");
}

//! [new_overload_0]
void test_unstdbytebuffer_new_overload_0(void) {
    u8t err;

    // [1] Success case with preallocation
    unstdbytebuffer *b1 = unstdbytebuffer_new_overload_0(16, &err);
    assert(b1 != NULL);
    assert(err == 1);
    assert(b1->buffer.capacity == b1->limit);
    assert(b1->buffer.capacity == 16);
    assert(b1->limit == 16);
    assert(b1->buffer.data != NULL);
    free(b1->buffer.data);
    free(b1);

    // [1] Success with preallocation = 0 → data should be NULL
    unstdbytebuffer *b3 = unstdbytebuffer_new_overload_0(0, &err);
    assert(b3 != NULL);
    assert(err == 1);
    assert(b3->buffer.data == NULL);
    free(b3);

    _notify("[+]", "`unstdbytebuffer_new_overload_0()` passed");
}

//! [new_from_data]
void test_unstdbytebuffer_new_from_data(void) {
    u8t error = 0;

    // [2] buffer_arg is NULL
    unstdbytebuffer *bb_null_data = unstdbytebuffer_new_from_data(NULL, 10, &error);
    assert(bb_null_data == NULL && error == 2);

    // [3] length_arg is 0
    u8t dummy[4] = {1, 2, 3, 4};
    unstdbytebuffer *bb_zero_len = unstdbytebuffer_new_from_data(dummy, 0, &error);
    assert(bb_zero_len == NULL && error == 3);

    // [1] Success case
    u8t data[5] = {10, 20, 30, 40, 50};
    unstdbytebuffer *bb_valid = unstdbytebuffer_new_from_data(data, sizeof(data), &error);
    assert(bb_valid != NULL && error == 1);
    assert(bb_valid->buffer.data == data);
    assert(bb_valid->buffer.length == sizeof(data));
    assert(bb_valid->buffer.capacity == sizeof(data));
    assert(bb_valid->limit == sizeof(data));
    free(bb_valid);

    _notify("[+]", "`unstdbytebuffer_new_from_data()` passed");
}

//! [new_from_data_copy]
void test_unstdbytebuffer_new_from_data_copy(void) {
    u8t error = 0;

    // [2] buffer_arg is NULL
    unstdbytebuffer *bb_null = unstdbytebuffer_new_from_data_copy(NULL, 5, &error);
    assert(bb_null == NULL && error == 2);

    // [3] length_arg is 0
    u8t dummy[1] = {42};
    unstdbytebuffer *bb_zero_len = unstdbytebuffer_new_from_data_copy(dummy, 0, &error);
    assert(bb_zero_len == NULL && error == 3);

    // [1] Success
    u8t src[4] = {10, 20, 30, 40};
    unstdbytebuffer *bb = unstdbytebuffer_new_from_data_copy(src, 4, &error);
    assert(bb != NULL && error == 1);
    assert(bb->buffer.length == 4);
    assert(bb->buffer.capacity == 4);
    assert(bb->limit == 4);
    assert(memcmp(bb->buffer.data, src, 4) == 0);
    free(bb->buffer.data);
    free(bb);

    _notify("[+]", "`unstdbytebuffer_new_from_data_copy()` passed");
}


//! [evaluatefit]
void test_unstdbytebuffer_evaluatefit(void) {
    // Setup buffer with capacity = 64, limit = 100, length = 32
    unstdbytebuffer bb;
    bb.buffer.length = 32;
    bb.buffer.capacity = 64;
    bb.limit = 100;

    // [1] Success: fits within capacity and limit
    u8t result1 = unstdbytebuffer_evaluatefit(&bb, 16); // 32 + 16 = 48 <= 64 & <= 100
    assert(result1 == 1);

    // [2] Reallocation needed: fits within limit, but not capacity
    u8t result2 = unstdbytebuffer_evaluatefit(&bb, 40); // 32 + 40 = 72 > 64 (capacity) but < 100
    assert(result2 == 2);

    // [0] Failure: exceeds limit
    u8t result3 = unstdbytebuffer_evaluatefit(&bb, 80); // 32 + 80 = 112 > 100 (limit)
    assert(result3 == 0);

    // [1] Success: exactly at capacity
    bb.buffer.length = 40;
    bb.buffer.capacity = 60;
    bb.limit = 100;
    u8t result4 = unstdbytebuffer_evaluatefit(&bb, 20); // 40 + 20 = 60 == capacity
    assert(result4 == 1);

    // [0] Failure: limit is tight
    bb.buffer.length = 80;
    bb.buffer.capacity = 200;
    bb.limit = 100;
    u8t result5 = unstdbytebuffer_evaluatefit(&bb, 25); // 80 + 25 = 105 > 100
    assert(result5 == 0);

    _notify("[+]", "`unstdbytebuffer_evaluatefit()` passed");
}

//! [destroy]
void test_unstdbytebuffer_destroy(void) {
    u8t error = 0;

    // [1] Case: NULL context_arg
    unstdbytebuffer_destroy(NULL, &error);
    assert(error == 2);

    // [2] Case: valid buffer, directly modify fields and destroy
    u8t err = 0;
    unstdbytebuffer *bb = unstdbytebuffer_new(16, 32, &err);
    assert(bb && err == 1);

    // Directly manipulate buffer memory (no _set or write functions)
    bb->buffer.data[0] = 42;
    bb->buffer.data[1] = 99;
    bb->buffer.data[2] = 7;
    bb->buffer.length = 3;
    bb->position = 3;
    bb->limit = 3;

    assert(bb->buffer.data[0] == 42);
    assert(bb->buffer.data[1] == 99);
    assert(bb->buffer.data[2] == 7);
    assert(bb->position == 3);
    assert(bb->limit == 3);

    // Destroy the buffer
    unstdbytebuffer_destroy(bb, &error);
    assert(error == 1);

    _notify("[+]", "`unstdbytebuffer_destroy()` passed");
}

//! [position]
void test_unstdbytebuffer_position(void) {
    u8t error = 0;

    // Case 1: NULL input
    sizet pos = unstdbytebuffer_position(NULL, &error);
    assert(pos == 0);
    assert(error == 2);

    // Case 2: Valid buffer with manual position
    u8t err = 0;
    unstdbytebuffer *bb = unstdbytebuffer_new(8, 16, &err);
    assert(bb && err == 1);

    bb->position = 5;

    pos = unstdbytebuffer_position(bb, &error);
    assert(pos == 5);
    assert(error == 1);

    unstdbytebuffer_destroy(bb, NULL);
    _notify("[+]", "`unstdbytebuffer_position()` passed");
}

//! [set_position]
void test_unstdbytebuffer_set_position(void) {
    u8t error = 0;

    // Case 1: NULL context
    unstdbytebuffer_set_position(NULL, 3, &error);
    assert(error == 2);

    // Case 2: Invalid data pointer
    unstdbytebuffer bb_invalid = {0};
    unstdbytebuffer_set_position(&bb_invalid, 1, &error);
    assert(error == 3);

    // Case 3: Exceeds limit
    u8t err = 0;
    unstdbytebuffer *bb = unstdbytebuffer_new(8, 16, &err);
    assert(bb && err == 1);

    bb->limit = 4;
    unstdbytebuffer_set_position(bb, 5, &error);
    assert(error == 4);

    // Case 4: Exceeds capacity
    bb->limit = 0; // limit not enforced
    unstdbytebuffer_set_position(bb, 20, &error);
    assert(error == 5);

    // Case 5: Valid
    unstdbytebuffer_set_position(bb, 3, &error);
    assert(error == 1);
    assert(bb->position == 3);

    unstdbytebuffer_destroy(bb, NULL);
    _notify("[+]", "`unstdbytebuffer_set_position()` passed");
}

//! [limit]
void test_unstdbytebuffer_limit(void) {
    u8t err = 0;

    // Case 1: NULL context
    sizet result = unstdbytebuffer_limit(NULL, &err);
    assert(result == 0);
    assert(err == 2);

    // Case 2: Valid buffer
    unstdbytebuffer *bb = unstdbytebuffer_new(16, 64, &err);
    assert(bb && err == 1);

    result = unstdbytebuffer_limit(bb, &err);
    assert(result == 64);
    assert(err == 1);

    unstdbytebuffer_destroy(bb, NULL);
    _notify("[+]", "`unstdbytebuffer_limit()` passed");
}

//! [set_limit]
void test_unstdbytebuffer_set_limit(void) {
    u8t error = 0;

    // Case 1: NULL context
    unstdbytebuffer_set_limit(NULL, 10, &error);
    assert(error == 2);

    // Case 2: Invalid data pointer
    unstdbytebuffer bb_invalid = {0};
    unstdbytebuffer_set_limit(&bb_invalid, 10, &error);
    assert(error == 3);

    // Case 3: limit less than capacity (reject)
    u8t err = 0;
    unstdbytebuffer *bb = unstdbytebuffer_new(8, 16, &err);
    assert(bb && err == 1);

    // try to set limit less than capacity (8)
    unstdbytebuffer_set_limit(bb, 4, &error);
    assert(error == 4);

    // Case 4: limit equal to capacity (accept)
    unstdbytebuffer_set_limit(bb, 8, &error);
    assert(error == 1);
    assert(bb->limit == 8);

    // Case 5: limit greater than capacity (accept)
    unstdbytebuffer_set_limit(bb, 32, &error);
    assert(error == 1);
    assert(bb->limit == 32);

    unstdbytebuffer_destroy(bb, NULL);
    _notify("[+]", "`unstdbytebuffer_set_limit()` passed");
}

//! [remaining]
void test_unstdbytebuffer_remaining(void) {
    u8t error = 0;

    // Case 1: NULL context
    sizet rem = unstdbytebuffer_remaining(NULL, &error);
    assert(rem == 0);
    assert(error == 2);

    // Case 2: Valid buffer, limit > position
    u8t err = 0;
    unstdbytebuffer *bb = unstdbytebuffer_new(8, 16, &err);
    assert(bb && err == 1);

    bb->position = 4;
    bb->limit = 12;

    rem = unstdbytebuffer_remaining(bb, &error);
    assert(error == 1);
    assert(rem == 8); // 12 - 4

    // Case 3: position == limit (no remaining)
    bb->position = bb->limit;
    rem = unstdbytebuffer_remaining(bb, &error);
    assert(error == 1);
    assert(rem == 0);

    // Case 4: position > limit (negative, but sizet unsigned, so large number)
    // This is technically a misuse, but let's see what happens
    bb->position = 20;
    rem = unstdbytebuffer_remaining(bb, &error);
    assert(error == 1);
    // rem = limit - position => 12 - 20 => underflow; this may wrap to large sizet
    // So, rem will be very large. Just check error success here.

    unstdbytebuffer_destroy(bb, NULL);
    _notify("[+]", "`unstdbytebuffer_remaining()` passed");
}

//! [has_remaining]
void test_unstdbytebuffer_has_remaining(void) {
    u8t error = 0;

    // Case 1: NULL context
    bool has_rem = unstdbytebuffer_has_remaining(NULL, &error);
    assert(has_rem == false);
    assert(error == 2);

    // Case 2: limit == 0 (unset), always true
    u8t err = 0;
    unstdbytebuffer *bb = unstdbytebuffer_new(8, 0, &err);
    assert(bb && err == 1);

    bb->position = 100;
    has_rem = unstdbytebuffer_has_remaining(bb, &error);
    assert(error == 1);
    assert(has_rem == true);

    // Case 3: position < limit
    bb->limit = 10;
    bb->position = 5;
    has_rem = unstdbytebuffer_has_remaining(bb, &error);
    assert(error == 1);
    assert(has_rem == true);

    // Case 4: position == limit
    bb->position = 10;
    has_rem = unstdbytebuffer_has_remaining(bb, &error);
    assert(error == 1);
    assert(has_rem == false);

    // Case 5: position > limit
    bb->position = 15;
    has_rem = unstdbytebuffer_has_remaining(bb, &error);
    assert(error == 1);
    assert(has_rem == false);

    unstdbytebuffer_destroy(bb, NULL);
    _notify("[+]", "`unstdbytebuffer_has_remaining()` passed");
}

//! [length]
void test_unstdbytebuffer_length(void) {
    u8t error = 0;

    // Case 1: NULL context
    sizet len = unstdbytebuffer_length(NULL, &error);
    assert(len == 0);
    assert(error == 2);

    // Case 2: NULL data pointer
    unstdbytebuffer bb_invalid = {0};
    len = unstdbytebuffer_length(&bb_invalid, &error);
    assert(len == 0);
    assert(error == 3);

    // Case 3: Valid buffer with non-zero length
    u8t err = 0;
    unstdbytebuffer *bb = unstdbytebuffer_new(16, 32, &err);
    assert(bb && err == 1);

    bb->buffer.length = 10;
    len = unstdbytebuffer_length(bb, &error);
    assert(error == 1);
    assert(len == 10);

    unstdbytebuffer_destroy(bb, NULL);
    _notify("[+]", "`unstdbytebuffer_length()` passed");
}

//! [bytes]
void test_unstdbytebuffer_bytes(void) {
    u8t error = 0;

    // Case 1: NULL context
    u8t *ptr = unstdbytebuffer_bytes(NULL, &error);
    assert(ptr == NULL);
    assert(error == 2);

    // Case 2: NULL data pointer
    unstdbytebuffer bb_invalid = {0};
    ptr = unstdbytebuffer_bytes(&bb_invalid, &error);
    assert(ptr == NULL);
    assert(error == 3);

    // Case 3: Valid buffer with allocated data
    u8t err = 0;
    unstdbytebuffer *bb = unstdbytebuffer_new(16, 32, &err);
    assert(bb && err == 1);

    ptr = unstdbytebuffer_bytes(bb, &error);
    assert(error == 1);
    assert(ptr == bb->buffer.data);

    unstdbytebuffer_destroy(bb, NULL);

    _notify("[+]", "`unstdbytebuffer_bytes()` passed");
}

//! [rewind]
void test_unstdbytebuffer_rewind(void) {
    u8t error = 0;

    // Case 1: NULL context
    unstdbytebuffer_rewind(NULL, &error);
    assert(error == 2);

    // Case 2: Valid buffer resets position to zero
    u8t err = 0;
    unstdbytebuffer *bb = unstdbytebuffer_new(16, 32, &err);
    assert(bb && err == 1);

    bb->position = 10; // move position forward
    unstdbytebuffer_rewind(bb, &error);
    assert(error == 1);
    assert(bb->position == 0);

    unstdbytebuffer_destroy(bb, NULL);
    _notify("[+]", "`unstdbytebuffer_rewind()` passed");
}

//! [compact]
void test_unstdbytebuffer_compact(void) {
    u8t error = 0;

    // Case 1: NULL context
    unstdbytebuffer_compact(NULL, &error);
    assert(error == 2);

    // Case 2: Invalid data pointer
    unstdbytebuffer bb_invalid = {0};
    unstdbytebuffer_compact(&bb_invalid, &error);
    assert(error == 3);

    // Case 3: Position beyond length
    u8t err = 0;
    unstdbytebuffer *bb = unstdbytebuffer_new(10, 20, &err);
    assert(bb && err == 1);

    bb->buffer.length = 10;
    bb->position = 11;  // position > length, invalid
    unstdbytebuffer_compact(bb, &error);
    assert(error == 4);

    // Case 4: Normal compaction with some data before position
    bb->position = 4;
    bb->buffer.length = 10;

    // Fill buffer with dummy data: 0..9
    for (sizet i = 0; i < 10; i++) {
        bb->buffer.data[i] = (u8t) i;
    }

    unstdbytebuffer_compact(bb, &error);
    assert(error == 1);

    // After compaction, length should be 6 (10 - 4)
    assert(bb->buffer.length == 6);
    // Position should be set to 6 (remaining bytes)
    assert(bb->position == 6);

    // Data should be shifted left by 4 bytes: old bytes [4..9] now at [0..5]
    for (sizet i = 0; i < 6; i++) {
        assert(bb->buffer.data[i] == (u8t) (i + 4));
    }

    // Case 5: Compact when position == length (no remaining bytes)
    bb->position = bb->buffer.length;
    unstdbytebuffer_compact(bb, &error);
    assert(error == 1);
    assert(bb->buffer.length == 0);
    assert(bb->position == 0);

    unstdbytebuffer_destroy(bb, NULL);

    _notify("[+]", "`unstdbytebuffer_compact()` passed");
}

//! [skip]
void test_unstdbytebuffer_skip(void) {
    u8t error = 0;

    // Case 1: NULL context
    unstdbytebuffer_skip(NULL, 5, &error);
    assert(error == 2);

    // Case 2: NULL data pointer
    unstdbytebuffer bb_invalid = {0};
    unstdbytebuffer_skip(&bb_invalid, 5, &error);
    assert(error == 3);

    // Prepare a valid buffer with capacity 10
    u8t err = 0;
    unstdbytebuffer *bb = unstdbytebuffer_new(8, 10, &err);
    assert(bb != NULL);
    assert(err == 1);

    // Case 3: skip beyond limit (limit set to 8)
    bb->limit = 8;
    bb->position = 5;
    unstdbytebuffer_skip(bb, 4, &error); // 5 + 4 = 9 > 8 limit
    assert(error == 4);

    // Case 4: skip beyond capacity (capacity 10, position 8, skip 3 = 11 > 10)
    bb->limit = 0; // no limit
    bb->position = 8;
    unstdbytebuffer_skip(bb, 3, &error);
    assert(error == 5);

    // Case 6: Successful skip, no limit
    bb->position = 0;
    bb->buffer.length = 0;
    unstdbytebuffer_skip(bb, 5, &error);
    assert(error == 1);
    assert(bb->position == 5);
    assert(bb->buffer.length >= 5);

    // Case 7: Successful skip with limit large enough
    bb->limit = 10;
    bb->position = 2;
    bb->buffer.length = 5;
    unstdbytebuffer_skip(bb, 3, &error); // 2+3=5 <= 10 limit
    assert(error == 1);
    assert(bb->position == 5);
    assert(bb->buffer.length >= 5);

    unstdbytebuffer_destroy(bb, NULL);
    _notify("[+]", "`unstdbytebuffer_skip()` passed");
}

//! [reuse]
void test_unstdbytebuffer_reuse(void) {
    u8t error = 0;

    // Case 1: NULL context
    unstdbytebuffer_reuse(NULL, &error);
    assert(error == 2);

    // Case 2: NULL data pointer in buffer
    unstdbytebuffer bb_invalid = {0};
    unstdbytebuffer_reuse(&bb_invalid, &error);
    assert(error == 3);

    // Prepare a valid buffer for reuse testing
    u8t err = 0;
    unstdbytebuffer *bb = unstdbytebuffer_new(2, 10, &err);
    assert(bb != NULL);
    assert(err == 1);

    // Case 4: Success case
    bb->position = 5;
    unstdbytebuffer_reuse(bb, &error);
    assert(error == 1);
    assert(bb->position == 0);
    assert(bb->buffer.length == 0);

    unstdbytebuffer_destroy(bb, NULL);
    _notify("[+]", "`unstdbytebuffer_reuse()` passed");
}

//! [write_uint32]
void test_unstdbytebuffer_write_uint32(void) {
    u8t err;

    // [2] NULL context_arg
    bool res = unstdbytebuffer_write_uint32(NULL, 0x12345678, &err);
    assert(res == false);
    assert(err == 2);

    // [3] buffer.data == NULL
    unstdbytebuffer bb_null_data = {0};
    bb_null_data.buffer.capacity = 4;
    res = unstdbytebuffer_write_uint32(&bb_null_data, 0x12345678, &err);
    assert(res == false);
    assert(err == 3);

    // [4] limit exceeded
    u8t buffer_data[10] = {0};
    unstdbytebuffer bb_limit = {
            .buffer = {.data = buffer_data, .capacity = 10, .length = 10},
            .position = 8,
            .limit = 10
    };
    res = unstdbytebuffer_write_uint32(&bb_limit, 0x12345678, &err);
    assert(res == false);
    assert(err == 4);

    // [1] Success case
    u8t buffer_data_success[8] = {0};
    unstdbytebuffer bb_success = {
            .buffer = {.data = buffer_data_success, .capacity = 8, .length = 0},
            .position = 0,
            .limit = 0
    };

    res = unstdbytebuffer_write_uint32(&bb_success, 0x78563412, &err);
    assert(res == true);
    assert(err == 1);
    assert(bb_success.buffer.data[0] == 0x12);
    assert(bb_success.buffer.data[1] == 0x34);
    assert(bb_success.buffer.data[2] == 0x56);
    assert(bb_success.buffer.data[3] == 0x78);
    assert(bb_success.position == 4);

    _notify("[+]", "`unstdbytebuffer_write_uint32()` passed");
}

//! [write_int32]
void test_unstdbytebuffer_write_int32(void) {
    u8t err;

    // [2] NULL context_arg
    bool res = unstdbytebuffer_write_int32(NULL, -12345678, &err);
    assert(res == false);
    assert(err == 2);

    // [3] buffer.data == NULL — create with 0 capacity to get NULL data pointer
    unstdbytebuffer *bb_null_data = unstdbytebuffer_new(0, 128, &err);
    assert(bb_null_data != NULL);
    assert(err == 1);
    assert(bb_null_data->buffer.data == NULL);

    bb_null_data->buffer.capacity = 1;
    res = unstdbytebuffer_write_int32(bb_null_data, -12345678, &err);
    assert(res == false);
    assert(err == 3);

    free(bb_null_data);

    // [4] limit exceeded — create buffer with limit smaller than needed
    unstdbytebuffer *bb_limit = unstdbytebuffer_new(4, 128, &err);
    assert(bb_limit != NULL);
    assert(err == 1);

    bb_limit->position = 2;
    bb_limit->limit = 4;  // limit == capacity, but position + 4 > limit (2 + 4 > 4)

    res = unstdbytebuffer_write_int32(bb_limit, -12345678, &err);
    assert(res == false);
    assert(err == 4);

    free(bb_limit->buffer.data);
    free(bb_limit);

    // [1] Success case
    unstdbytebuffer *bb_success = unstdbytebuffer_new(16, 32, &err);
    assert(bb_success != NULL);
    assert(err == 1);

    res = unstdbytebuffer_write_int32(bb_success, -0x12345678, &err);
    assert(res == true);
    assert(err == 1);

    u8t *data = bb_success->buffer.data;
    assert(data[0] == (u8t) (-0x12345678 >> 0));
    assert(data[1] == (u8t) (-0x12345678 >> 8));
    assert(data[2] == (u8t) (-0x12345678 >> 16));
    assert(data[3] == (u8t) (-0x12345678 >> 24));

    assert(bb_success->position == 4);

    free(bb_success->buffer.data);
    free(bb_success);

    _notify("[+]", "`unstdbytebuffer_write_int32()` passed");
}


//! [write_uint64]
void test_unstdbytebuffer_write_uint64(void) {
    u8t err;

    // [2] NULL context_arg
    bool res = unstdbytebuffer_write_uint64(NULL, 0x123456789ABCDEF0ULL, &err);
    assert(res == false);
    assert(err == 2);

    // [3] buffer.data == NULL (capacity=0)
    unstdbytebuffer *bb_null_data = unstdbytebuffer_new(0, 128, &err);
    assert(bb_null_data != NULL);
    assert(err == 1);
    assert(bb_null_data->buffer.data == NULL);

    bb_null_data->buffer.capacity = 1;
    res = unstdbytebuffer_write_uint64(bb_null_data, 0x123456789ABCDEF0ULL, &err);
    assert(res == false);
    assert(err == 3);

    free(bb_null_data);

    // [4] limit exceeded (position + 8 > limit)
    unstdbytebuffer *bb_limit = unstdbytebuffer_new(8, 128, &err);
    assert(bb_limit != NULL);
    assert(err == 1);

    bb_limit->position = 4;
    bb_limit->limit = 8; // 4 + 8 > 8 → fail

    res = unstdbytebuffer_write_uint64(bb_limit, 0x123456789ABCDEF0ULL, &err);
    assert(res == false);
    assert(err == 4);

    free(bb_limit->buffer.data);
    free(bb_limit);

    // [1] Success case
    unstdbytebuffer *bb_success = unstdbytebuffer_new(16, 32, &err);
    assert(bb_success != NULL);
    assert(err == 1);

    u64t val = 0x123456789ABCDEF0ULL;

    res = unstdbytebuffer_write_uint64(bb_success, val, &err);
    assert(res == true);
    assert(err == 1);

    u8t *data = bb_success->buffer.data;

    assert(data[0] == (u8t) (val >> 0));
    assert(data[1] == (u8t) (val >> 8));
    assert(data[2] == (u8t) (val >> 16));
    assert(data[3] == (u8t) (val >> 24));
    assert(data[4] == (u8t) (val >> 32));
    assert(data[5] == (u8t) (val >> 40));
    assert(data[6] == (u8t) (val >> 48));
    assert(data[7] == (u8t) (val >> 56));

    assert(bb_success->position == 8);

    free(bb_success->buffer.data);
    free(bb_success);

    _notify("[+]", "`unstdbytebuffer_write_uint64()` passed");
}

//! [write_int64]
void test_unstdbytebuffer_write_int64(void) {
    u8t err;

    // [2] NULL context_arg
    bool res = unstdbytebuffer_write_int64(NULL, -1234567890123456789LL, &err);
    assert(res == false);
    assert(err == 2);

    // [3] buffer.data == NULL (capacity=0)
    unstdbytebuffer *bb_null_data = unstdbytebuffer_new(0, 128, &err);
    assert(bb_null_data != NULL);
    assert(err == 1);
    assert(bb_null_data->buffer.data == NULL);

    bb_null_data->buffer.capacity = 1;
    res = unstdbytebuffer_write_int64(bb_null_data, -1234567890123456789LL, &err);
    assert(res == false);
    assert(err == 3);

    free(bb_null_data);

    // [4] limit exceeded (position + 8 > limit)
    unstdbytebuffer *bb_limit = unstdbytebuffer_new(8, 128, &err);
    assert(bb_limit != NULL);
    assert(err == 1);

    bb_limit->position = 4;
    bb_limit->limit = 8; // 4 + 8 > 8 → fail

    res = unstdbytebuffer_write_int64(bb_limit, -1234567890123456789LL, &err);
    assert(res == false);
    assert(err == 4);

    free(bb_limit->buffer.data);
    free(bb_limit);

    // [1] Success case
    unstdbytebuffer *bb_success = unstdbytebuffer_new(16, 32, &err);
    assert(bb_success != NULL);
    assert(err == 1);

    s64t val = -1234567890123456789LL;

    res = unstdbytebuffer_write_int64(bb_success, val, &err);
    assert(res == true);
    assert(err == 1);

    u8t *data = bb_success->buffer.data;

    // Validate little-endian bytes of val
    assert(data[0] == (u8t) (val >> 0));
    assert(data[1] == (u8t) (val >> 8));
    assert(data[2] == (u8t) (val >> 16));
    assert(data[3] == (u8t) (val >> 24));
    assert(data[4] == (u8t) (val >> 32));
    assert(data[5] == (u8t) (val >> 40));
    assert(data[6] == (u8t) (val >> 48));
    assert(data[7] == (u8t) (val >> 56));

    assert(bb_success->position == 8);

    free(bb_success->buffer.data);
    free(bb_success);

    _notify("[+]", "`unstdbytebuffer_write_int64()` passed");
}

//! [write_byte]
void test_unstdbytebuffer_write_byte(void) {
    u8t err;

    // [2] NULL context_arg
    bool res = unstdbytebuffer_write_byte(NULL, 0xAB, &err);
    assert(res == false);
    assert(err == 2);

    // [3] buffer.data == NULL (capacity=0)
    unstdbytebuffer *bb_null_data = unstdbytebuffer_new(0, 128, &err);
    assert(bb_null_data != NULL);
    assert(err == 1);
    assert(bb_null_data->buffer.data == NULL);

    bb_null_data->buffer.capacity = 1;
    res = unstdbytebuffer_write_byte(bb_null_data, 0xAB, &err);
    assert(res == false);
    assert(err == 3);

    free(bb_null_data);

    // [4] limit exceeded (position + 1 > limit)
    unstdbytebuffer *bb_limit = unstdbytebuffer_new(1, 128, &err);
    assert(bb_limit != NULL);
    assert(err == 1);

    bb_limit->position = 1;
    bb_limit->limit = 1; // 1 + 1 > 1 → fail

    res = unstdbytebuffer_write_byte(bb_limit, 0xAB, &err);
    assert(res == false);
    assert(err == 4);

    free(bb_limit->buffer.data);
    free(bb_limit);

    // [1] Success case
    unstdbytebuffer *bb_success = unstdbytebuffer_new(8, 32, &err);
    assert(bb_success != NULL);
    assert(err == 1);

    u8t val = 0xAB;

    res = unstdbytebuffer_write_byte(bb_success, val, &err);
    assert(res == true);
    assert(err == 1);

    u8t *data = bb_success->buffer.data;

    assert(data[0] == val);
    assert(bb_success->position == 1);

    u8t val2 = 0xCD;

    res = unstdbytebuffer_write_byte(bb_success, val2, &err);
    assert(res == true);
    assert(err == 1);

    data = bb_success->buffer.data;

    assert(data[1] == val2);
    assert(bb_success->position == 2);

    free(bb_success->buffer.data);
    free(bb_success);

    _notify("[+]", "`unstdbytebuffer_write_byte()` passed");
}

//! [write_vuint]
void test_unstdbytebuffer_write_vuint(void) {
    u8t err;

    // [2] NULL context_arg
    bool res = unstdbytebuffer_write_vuint(NULL, 12345, &err);
    assert(res == false);
    assert(err == 2);

    // [3] unstdbytebuffer_write_byte fails internally
    // Create a buffer with capacity but NULL data to force a write failure
    unstdbytebuffer *bb_fail = unstdbytebuffer_new(0, 128, &err);
    assert(bb_fail != NULL);
    assert(err == 1);
    assert(bb_fail->buffer.data == NULL);

    bb_fail->buffer.capacity = 1;
    res = unstdbytebuffer_write_vuint(bb_fail, 12345, &err);
    assert(res == false);
    assert(err == 3);

    free(bb_fail);

    // [1] Success case — writing small number (e.g., 127)
    unstdbytebuffer *bb_small = unstdbytebuffer_new(8, 32, &err);
    assert(bb_small != NULL);
    assert(err == 1);

    u64t val_small = 127;
    res = unstdbytebuffer_write_vuint(bb_small, val_small, &err);
    assert(res == true);
    assert(err == 1);

    assert(bb_small->buffer.data[0] == 0x7F); // 127 = 0x7F
    assert(bb_small->position == 1);

    free(bb_small->buffer.data);
    free(bb_small);

    // [1] Success case — writing large number (e.g., 300)
    unstdbytebuffer *bb_large = unstdbytebuffer_new(8, 32, &err);
    assert(bb_large != NULL);
    assert(err == 1);

    u64t val_large = 300;
    res = unstdbytebuffer_write_vuint(bb_large, val_large, &err);
    assert(res == true);
    assert(err == 1);

    // 300 in variable-length encoding: 0xAC 0x02
    assert(bb_large->buffer.data[0] == 0xAC); // 0b10101100
    assert(bb_large->buffer.data[1] == 0x02); // 0b00000010
    assert(bb_large->position == 2);

    free(bb_large->buffer.data);
    free(bb_large);

    _notify("[+]", "`unstdbytebuffer_write_vuint()` passed");
}

//! [write_bool]
void test_unstdbytebuffer_write_bool(void) {
    u8t err;

    // [2] NULL context_arg
    bool res = unstdbytebuffer_write_bool(NULL, true, &err);
    assert(res == false);
    assert(err == 2); // Should be passed through from write_uint32

    // [1] Success: true → 0xBADBABE5
    unstdbytebuffer *bb_true = unstdbytebuffer_new(8, 32, &err);
    assert(bb_true != NULL);
    assert(err == 1);

    res = unstdbytebuffer_write_bool(bb_true, true, &err);
    assert(res == true);
    assert(err == 1);

    u8t *data_true = bb_true->buffer.data;
    assert(data_true[0] == 0xE5);  // LSB
    assert(data_true[1] == 0xAB);
    assert(data_true[2] == 0xDB);
    assert(data_true[3] == 0xBA);  // MSB
    assert(bb_true->position == 4);

    free(bb_true->buffer.data);
    free(bb_true);

    // [1] Success: false → 0xBADBEEF5
    unstdbytebuffer *bb_false = unstdbytebuffer_new(8, 32, &err);
    assert(bb_false != NULL);
    assert(err == 1);

    res = unstdbytebuffer_write_bool(bb_false, false, &err);
    assert(res == true);
    assert(err == 1);

    u8t *data_false = bb_false->buffer.data;
    assert(data_false[0] == 0xF5);  // LSB
    assert(data_false[1] == 0xEE);
    assert(data_false[2] == 0xDB);
    assert(data_false[3] == 0xBA);  // MSB
    assert(bb_false->position == 4);

    free(bb_false->buffer.data);
    free(bb_false);

    _notify("[+]", "`unstdbytebuffer_write_bool()` passed");
}

//! [write_double]
void test_unstdbytebuffer_write_double(void) {
    u8t err;

    // [2] NULL context_arg
    bool res = unstdbytebuffer_write_double(NULL, 3.14159265358979, &err);
    assert(res == false);
    assert(err == 2);

    // [3] NULL buffer data
    unstdbytebuffer bb_invalid = {0};
    bb_invalid.buffer.data = NULL;
    bb_invalid.buffer.capacity = 1;
    res = unstdbytebuffer_write_double(&bb_invalid, 3.14159, &err);
    assert(res == false);
    assert(err == 3);

    // [4] limit exceeded
    unstdbytebuffer *bb_limit = unstdbytebuffer_new(2, 7, &err); // only 7 bytes allowed
    assert(bb_limit->buffer.data != NULL);
    assert(bb_limit != NULL);
    assert(err == 1);
    res = unstdbytebuffer_write_double(bb_limit, 0.5, &err);
    assert(res == false);
    assert(err == 4);
    free(bb_limit->buffer.data);
    free(bb_limit);

    // [1] Success + verify correctness
    unstdbytebuffer *bb = unstdbytebuffer_new(0, 64, &err);
    assert(bb != NULL);
    assert(err == 1);

    double input = -123456.789;
    res = unstdbytebuffer_write_double(bb, input, &err);
    assert(res == true);
    assert(err == 1);
    assert(bb->position == 8);

    // Read it back
    double output;
    memcpy(&output, bb->buffer.data, sizeof(double));

    assert(input == output);

    free(bb->buffer.data);
    free(bb);

    _notify("[+]", "`unstdbytebuffer_write_double()` passed");
}

//! [write_bytes]
void test_unstdbytebuffer_write_bytes(void) {
    u8t err;

    // [2] NULL context_arg
    u8t src[] = {1, 2, 3};
    bool res = unstdbytebuffer_write_bytes(NULL, src, sizeof(src), &err);
    assert(res == false);
    assert(err == 2);

    // [3] NULL buffer data
    unstdbytebuffer bb_invalid = {0};
    bb_invalid.buffer.capacity = 4;
    bb_invalid.buffer.data = NULL;
    res = unstdbytebuffer_write_bytes(&bb_invalid, src, sizeof(src), &err);
    assert(res == false);
    assert(err == 3);



    // [4] Zero length, nothing to write.
    unstdbytebuffer *bb_nullsrc = unstdbytebuffer_new(0, 16, &err);
    assert(bb_nullsrc != NULL);
    assert(err == 1);

    res = unstdbytebuffer_write_bytes(bb_nullsrc, src, 0, &err);
    assert(res == false);
    assert(err == 4);

    // [5] NULL src with non-zero length
    res = unstdbytebuffer_write_bytes(bb_nullsrc, NULL, 4, &err);
    assert(res == false);
    assert(err == 5);
    free(bb_nullsrc->buffer.data);
    free(bb_nullsrc);

    // [6] limit exceeded
    unstdbytebuffer *bb_limit = unstdbytebuffer_new(0, 4, &err);
    assert(bb_limit != NULL);
    assert(err == 1);
    bb_limit->limit = 4;
    bb_limit->position = 3;
    res = unstdbytebuffer_write_bytes(bb_limit, src, 2, &err);
    assert(res == false);
    assert(err == 6);
    free(bb_limit->buffer.data);
    free(bb_limit);

    // [1] success + check correctness
    unstdbytebuffer *bb = unstdbytebuffer_new(0, 32, &err);
    assert(bb != NULL);
    assert(err == 1);

    u8t data[] = {10, 20, 30, 40, 50};
    sizet len = sizeof(data);
    res = unstdbytebuffer_write_bytes(bb, data, len, &err);
    assert(res == true);
    assert(err == 1);
    assert(bb->position == len);
    assert(memcmp(bb->buffer.data, data, len) == 0);

    free(bb->buffer.data);
    free(bb);

    _notify("[+]", "`unstdbytebuffer_write_bytes()` passed");
}


//! [write_bytes_offset]
void test_unstdbytebuffer_write_bytes_offset(void) {
    u8t err;

    // [2] NULL context_arg
    u8t src[] = {1, 2, 3, 4, 5};
    bool res = unstdbytebuffer_write_bytes_offset(NULL, src, 0, 3, &err);
    assert(res == false);
    assert(err == 2);

    // [3] buffer capacity but data is NULL
    unstdbytebuffer bb_invalid = {0};
    bb_invalid.buffer.capacity = 8;
    bb_invalid.buffer.data = NULL;
    res = unstdbytebuffer_write_bytes_offset(&bb_invalid, src, 0, 3, &err);
    assert(res == false);
    assert(err == 3);

    // [4] length_arg is zero
    unstdbytebuffer *bb_zero_len = unstdbytebuffer_new(0, 16, &err);
    assert(bb_zero_len != NULL);
    assert(err == 1);
    res = unstdbytebuffer_write_bytes_offset(bb_zero_len, src, 0, 0, &err);
    assert(res == false);
    assert(err == 4);
    free(bb_zero_len->buffer.data);
    free(bb_zero_len);

    // [5] buffer_arg is NULL while length > 0
    unstdbytebuffer *bb_nullsrc = unstdbytebuffer_new(0, 16, &err);
    assert(bb_nullsrc != NULL);
    assert(err == 1);
    res = unstdbytebuffer_write_bytes_offset(bb_nullsrc, NULL, 0, 4, &err);
    assert(res == false);
    assert(err == 5);
    free(bb_nullsrc->buffer.data);
    free(bb_nullsrc);

    // [6] limit exceeded
    unstdbytebuffer *bb_limit = unstdbytebuffer_new(0, 8, &err);
    assert(bb_limit != NULL);
    bb_limit->limit = 6;
    bb_limit->position = 5;
    res = unstdbytebuffer_write_bytes_offset(bb_limit, src, 0, 2, &err);
    assert(res == false);
    assert(err == 6);
    free(bb_limit->buffer.data);
    free(bb_limit);

    // [1] Success + verify correctness
    unstdbytebuffer *bb = unstdbytebuffer_new(0, 64, &err);
    assert(bb != NULL);
    assert(err == 1);

    u8t full_data[] = {9, 8, 7, 6, 5, 4, 3, 2, 1};
    sizet offset = 2;
    sizet len = 5;
    res = unstdbytebuffer_write_bytes_offset(bb, full_data, offset, len, &err);
    assert(res == true);
    assert(err == 1);
    assert(bb->position == len);
    assert(memcmp(bb->buffer.data, full_data + offset, len) == 0);

    free(bb->buffer.data);
    free(bb);

    _notify("[+]", "`unstdbytebuffer_write_bytes_offset()` passed");
}

//! [write_bytearray]
void test_unstdbytebuffer_write_bytearray(void) {
    u8t err;

    // [2] NULL context_arg
    u8t data[] = {10, 20, 30, 40, 50};
    bool res = unstdbytebuffer_write_bytearray(NULL, data, 0, 3, &err);
    assert(res == false);
    assert(err == 2);

    // [3] context has capacity but data is NULL
    unstdbytebuffer bb_invalid = {0};
    bb_invalid.buffer.capacity = 10;
    bb_invalid.buffer.data = NULL;
    res = unstdbytebuffer_write_bytearray(&bb_invalid, data, 0, 3, &err);
    assert(res == false);
    assert(err == 3);

    // [1] Success + verify
    unstdbytebuffer *bb = unstdbytebuffer_new(0, 64, &err);
    assert(bb != NULL);
    assert(err == 1);
    u8t full_data[] = {0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7};
    sizet offset = 2;
    sizet length = 4;

    res = unstdbytebuffer_write_bytearray(bb, full_data, offset, length, &err);
    assert(res == true);
    assert(err == 1);
    // Verify vuint encoding of 4 bytes + the actual 4 bytes
    assert(bb->position == 1 + 4); // assuming length fits in 1 byte vuint
    assert(bb->buffer.data[0] == 4); // length_arg written as vuint
    assert(memcmp(bb->buffer.data + 1, full_data + offset, length) == 0);

    free(bb->buffer.data);
    free(bb);

    _notify("[+]", "`unstdbytebuffer_write_bytearray()` passed");
}

//! [read_uint32]
void test_unstdbytebuffer_read_uint32(void) {
    u8t err;

    // [2] NULL context_arg
    u32t v = unstdbytebuffer_read_uint32(NULL, &err);
    assert(v == 0);
    assert(err == 2);

    // [3] buffer.data is NULL
    unstdbytebuffer bb_invalid = {0};
    bb_invalid.buffer.capacity = 8;
    bb_invalid.buffer.length = 8;
    bb_invalid.buffer.data = NULL;
    v = unstdbytebuffer_read_uint32(&bb_invalid, &err);
    assert(v == 0);
    assert(err == 3);

    // [4] position + 4 > limit
    u8t data_limit[] = {1, 2, 3, 4, 5};
    unstdbytebuffer bb_limit = {
            .buffer = {.data = data_limit, .capacity = 5, .length = 5},
            .position = 3,
            .limit = 5,
    };
    v = unstdbytebuffer_read_uint32(&bb_limit, &err);
    assert(v == 0);
    assert(err == 4);

    // [5] position + 4 > capacity
    u8t data_cap[] = {1, 2, 3, 4, 5};
    unstdbytebuffer bb_cap = {
            .buffer = {.data = data_cap, .capacity = 5, .length = 5},
            .position = 2,
            .limit = 0,
    };
    v = unstdbytebuffer_read_uint32(&bb_cap, &err);
    assert(v == 0);
    assert(err == 5);

    // [6] position + 4 > length
    u8t data_len[] = {1, 2, 3, 4, 5};
    unstdbytebuffer bb_len = {
            .buffer = {.data = data_len, .capacity = 8, .length = 5},
            .position = 3,
            .limit = 0,
    };
    v = unstdbytebuffer_read_uint32(&bb_len, &err);
    assert(v == 0);
    assert(err == 6);

    // [1] Success: write and read back value
    unstdbytebuffer *bb = unstdbytebuffer_new_overload_0(16, &err);
    assert(err == 1);
    assert(bb != NULL);

    u32t expected = 0xDEADBEEF;
    bool wres = unstdbytebuffer_write_uint32(bb, expected, &err);
    assert(err == 1);
    assert(wres == true);
    assert(bb->position == 4);

    bb->position = 0; // rewind for reading
    v = unstdbytebuffer_read_uint32(bb, &err);
    assert(v == expected);
    assert(err == 1);
    assert(bb->position == 4);

    free(bb->buffer.data);
    free(bb);

    _notify("[+]", "`unstdbytebuffer_read_uint32()` passed");
}

//! [read_int32]
void test_unstdbytebuffer_read_int32(void) {
    u8t err;

    // [2] NULL context_arg
    s32t v = unstdbytebuffer_read_int32(NULL, &err);
    assert(v == 0);
    assert(err == 2);

    // [3] buffer.data is NULL
    unstdbytebuffer bb_invalid = {0};
    bb_invalid.buffer.capacity = 8;
    bb_invalid.buffer.length = 8;
    bb_invalid.buffer.data = NULL;
    v = unstdbytebuffer_read_int32(&bb_invalid, &err);
    assert(v == 0);
    assert(err == 3);

    // [4] position + 4 > limit
    u8t data_limit[] = {1, 2, 3, 4, 5};
    unstdbytebuffer bb_limit = {
            .buffer = {.data = data_limit, .capacity = 5, .length = 5},
            .position = 3,
            .limit = 5,
    };
    v = unstdbytebuffer_read_int32(&bb_limit, &err);
    assert(v == 0);
    assert(err == 4);

    // [5] position + 4 > capacity
    u8t data_cap[] = {1, 2, 3, 4, 5};
    unstdbytebuffer bb_cap = {
            .buffer = {.data = data_cap, .capacity = 5, .length = 5},
            .position = 2,
            .limit = 0,
    };
    v = unstdbytebuffer_read_int32(&bb_cap, &err);
    assert(v == 0);
    assert(err == 5);

    // [6] position + 4 > length
    u8t data_len[] = {1, 2, 3, 4, 5};
    unstdbytebuffer bb_len = {
            .buffer = {.data = data_len, .capacity = 8, .length = 5},
            .position = 3,
            .limit = 0,
    };
    v = unstdbytebuffer_read_int32(&bb_len, &err);
    assert(v == 0);
    assert(err == 6);

    // [1] Success: write and read back value
    unstdbytebuffer *bb = unstdbytebuffer_new_overload_0(16, &err);
    assert(err == 1);
    assert(bb != NULL);

    s32t expected = -123456789;
    bool wres = unstdbytebuffer_write_int32(bb, expected, &err);
    assert(err == 1);
    assert(wres == true);
    assert(bb->position == 4);

    bb->position = 0;
    v = unstdbytebuffer_read_int32(bb, &err);
    assert(v == expected);
    assert(err == 1);
    assert(bb->position == 4);

    free(bb->buffer.data);
    free(bb);

    _notify("[+]", "`unstdbytebuffer_read_int32()` passed");
}

//! [read_uint64]
void test_unstdbytebuffer_read_uint64(void) {
    u8t err;

    // [2] context_arg is NULL
    u64t v = unstdbytebuffer_read_uint64(NULL, &err);
    assert(v == 0);
    assert(err == 2);

    // [3] buffer.data is NULL
    unstdbytebuffer bb_invalid = {0};
    bb_invalid.buffer.capacity = 8;
    bb_invalid.buffer.length = 8;
    v = unstdbytebuffer_read_uint64(&bb_invalid, &err);
    assert(v == 0);
    assert(err == 3);

    // [4] position + 8 > limit
    u8t data_limit[] = {0};
    unstdbytebuffer bb_limit = {
            .buffer = {.data = data_limit, .capacity = 16, .length = 16},
            .position = 10,
            .limit = 16,
    };
    v = unstdbytebuffer_read_uint64(&bb_limit, &err);
    assert(v == 0);
    assert(err == 4);

    // [5] position + 8 > capacity
    u8t data_cap[10] = {0};
    unstdbytebuffer bb_cap = {
            .buffer = {.data = data_cap, .capacity = 10, .length = 10},
            .position = 4,
            .limit = 0,
    };
    v = unstdbytebuffer_read_uint64(&bb_cap, &err);
    assert(v == 0);
    assert(err == 5);

    // [6] position + 8 > length
    u8t data_len[16] = {0};
    unstdbytebuffer bb_len = {
            .buffer = {.data = data_len, .capacity = 16, .length = 12},
            .position = 6,
            .limit = 0,
    };
    v = unstdbytebuffer_read_uint64(&bb_len, &err);
    assert(v == 0);
    assert(err == 6);

    // [1] Success: write and read back value
    unstdbytebuffer *bb = unstdbytebuffer_new_overload_0(16, &err);
    assert(err == 1);
    assert(bb != NULL);

    u64t expected = 0x1122334455667788ULL;

    bool wres = unstdbytebuffer_write_uint64(bb, expected, &err);
    assert(err == 1);
    assert(wres == true);
    assert(bb->position == 8);

    bb->position = 0;
    v = unstdbytebuffer_read_uint64(bb, &err);
    assert(v == expected);
    assert(err == 1);
    assert(bb->position == 8);

    unstdbytebuffer_destroy(bb, &err);
    assert(err == 1);

    _notify("[+]", "`unstdbytebuffer_read_uint64()` passed");
}

//! [read_int64]
void test_unstdbytebuffer_read_int64(void) {
    u8t err;

    // [2] context_arg is NULL
    s64t v = unstdbytebuffer_read_int64(NULL, &err);
    assert(v == 0);
    assert(err == 2);

    // [3] buffer.data is NULL
    unstdbytebuffer bb_invalid = {0};
    bb_invalid.buffer.capacity = 8;
    bb_invalid.buffer.length = 8;
    v = unstdbytebuffer_read_int64(&bb_invalid, &err);
    assert(v == 0);
    assert(err == 3);

    // [4] position + 8 > limit
    u8t data_limit[] = {0};
    unstdbytebuffer bb_limit = {
            .buffer = {.data = data_limit, .capacity = 16, .length = 16},
            .position = 10,
            .limit = 16,
    };
    v = unstdbytebuffer_read_int64(&bb_limit, &err);
    assert(v == 0);
    assert(err == 4);

    // [5] position + 8 > capacity
    u8t data_cap[10] = {0};
    unstdbytebuffer bb_cap = {
            .buffer = {.data = data_cap, .capacity = 10, .length = 10},
            .position = 4,
            .limit = 0,
    };
    v = unstdbytebuffer_read_int64(&bb_cap, &err);
    assert(v == 0);
    assert(err == 5);

    // [6] position + 8 > length
    u8t data_len[16] = {0};
    unstdbytebuffer bb_len = {
            .buffer = {.data = data_len, .capacity = 16, .length = 12},
            .position = 6,
            .limit = 0,
    };
    v = unstdbytebuffer_read_int64(&bb_len, &err);
    assert(v == 0);
    assert(err == 6);

    // [1] Success case with write + read
    unstdbytebuffer *bb = unstdbytebuffer_new_overload_0(32, &err);
    assert(err == 1);
    assert(bb != NULL);

    s64t expected = -0x112233445566778LL;

    bool wres = unstdbytebuffer_write_int64(bb, expected, &err);
    assert(err == 1);
    assert(wres == true);
    assert(bb->position == 8);

    bb->position = 0;
    v = unstdbytebuffer_read_int64(bb, &err);
    assert(v == expected);
    assert(err == 1);
    assert(bb->position == 8);

    unstdbytebuffer_destroy(bb, &err);
    assert(err == 1);

    _notify("[+]", "`unstdbytebuffer_read_int64()` passed");
}

//! [read_bool]
void test_unstdbytebuffer_read_bool(void) {
    u8t err;
    bool b;
    bool res;

    // [2] Invalid argument: NULL buffer
    b = unstdbytebuffer_read_bool(NULL, &err);
    assert(b == false);
    assert(err == 2);

    // [3] Invalid buffer state: data == NULL
    unstdbytebuffer bb_invalid = {0};
    bb_invalid.buffer.length = 4;
    bb_invalid.buffer.capacity = 4;
    bb_invalid.position = 0;
    b = unstdbytebuffer_read_bool(&bb_invalid, &err);
    assert(b == false);
    assert(err == 3);

    // [4] Exceeds limit
    u8t data_limit[] = {0};
    unstdbytebuffer bb_limit = {
            .buffer = {.data = data_limit, .capacity = 16, .length = 16},
            .position = 13,
            .limit = 16
    };
    b = unstdbytebuffer_read_bool(&bb_limit, &err);
    assert(b == false);
    assert(err == 4);

    // [5] Exceeds capacity
    u8t data_cap[5] = {0};
    unstdbytebuffer bb_cap = {
            .buffer = {.data = data_cap, .capacity = 5, .length = 5},
            .position = 2
    };
    b = unstdbytebuffer_read_bool(&bb_cap, &err);
    assert(b == false);
    assert(err == 5);

    // [5] Exceeds length
    u8t data_len[8] = {0};
    unstdbytebuffer bb_len = {
            .buffer = {.data = data_len, .capacity = 8, .length = 5},
            .position = 2
    };
    b = unstdbytebuffer_read_bool(&bb_len, &err);
    assert(b == false);
    assert(err == 6);

    // [1] Success: write false → read false
    unstdbytebuffer *bb_false = unstdbytebuffer_new(8, 32, &err);
    assert(bb_false != NULL);
    assert(err == 1);

    res = unstdbytebuffer_write_bool(bb_false, false, &err);
    assert(res == true);
    assert(err == 1);

    bb_false->position = 0;
    b = unstdbytebuffer_read_bool(bb_false, &err);
    assert(b == false);
    assert(err == 1);
    assert(bb_false->position == 4);
    unstdbytebuffer_destroy(bb_false, &err);
    assert(err == 1);

    // [1] Success: write true → read true
    unstdbytebuffer *bb_true = unstdbytebuffer_new(8, 32, &err);
    assert(bb_true != NULL);
    assert(err == 1);

    res = unstdbytebuffer_write_bool(bb_true, true, &err);
    assert(res == true);
    assert(err == 1);

    bb_true->position = 0;
    b = unstdbytebuffer_read_bool(bb_true, &err);
    assert(b == true);
    assert(err == 1);
    assert(bb_true->position == 4);
    unstdbytebuffer_destroy(bb_true, &err);
    assert(err == 1);

    _notify("[+]", "`unstdbytebuffer_read_bool()` passed");
}

//! [read_double]
void test_unstdbytebuffer_read_double(void) {
    u8t err;
    double d;
    bool res;

    // [2] Invalid argument: NULL buffer
    d = unstdbytebuffer_read_double(NULL, &err);
    assert(d == 0.0);
    assert(err == 2);

    // [3] Invalid buffer state: data == NULL
    unstdbytebuffer bb_invalid = {0};
    bb_invalid.buffer.length = 8;
    bb_invalid.buffer.capacity = 8;
    bb_invalid.position = 0;
    d = unstdbytebuffer_read_double(&bb_invalid, &err);
    assert(d == 0.0);
    assert(err == 3);

    // [4] Exceeds limit
    u8t data_limit[8] = {0};
    unstdbytebuffer bb_limit = {
            .buffer = {.data = data_limit, .capacity = 8, .length = 8},
            .position = 2,
            .limit = 8
    };
    bb_limit.position = 2;
    d = unstdbytebuffer_read_double(&bb_limit, &err);
    assert(d == 0.0);
    assert(err == 4);

    // [5] Exceeds capacity
    u8t data_cap[12] = {0};
    unstdbytebuffer bb_cap = {
            .buffer = {.data = data_cap, .capacity = 8, .length = 12},
            .position = 2
    };
    d = unstdbytebuffer_read_double(&bb_cap, &err);
    assert(d == 0.0);
    assert(err == 5);

    // [6] Exceeds length
    u8t data_len[16] = {0};
    unstdbytebuffer bb_len = {
            .buffer = {.data = data_len, .capacity = 16, .length = 4},
            .position = 0
    };
    d = unstdbytebuffer_read_double(&bb_len, &err);
    assert(d == 0.0);
    assert(err == 6);

    // [1] Success: write and read double
    unstdbytebuffer *bb_ok = unstdbytebuffer_new(8, 32, &err);
    assert(bb_ok != NULL);
    assert(err == 1);

    double test_value = 3.141592653589793;
    res = unstdbytebuffer_write_double(bb_ok, test_value, &err);
    assert(res == true);
    assert(err == 1);

    bb_ok->position = 0;
    d = unstdbytebuffer_read_double(bb_ok, &err);
    assert(err == 1);
    assert(d == test_value);  // direct comparison works since it's written-read in binary

    assert(bb_ok->position == 8);
    unstdbytebuffer_destroy(bb_ok, &err);
    assert(err == 1);

    _notify("[+]", "`unstdbytebuffer_read_double()` passed");
}

//! [read_byte]
void test_unstdbytebuffer_read_byte(void) {
    u8t err;
    u8t byte;
    bool res;

    // [2] Invalid argument: NULL context_arg
    byte = unstdbytebuffer_read_byte(NULL, &err);
    assert(byte == 0);
    assert(err == 2);

    // [3] Invalid buffer: data == NULL
    unstdbytebuffer bb_null_data = {0};
    bb_null_data.buffer.length = 1;
    bb_null_data.buffer.capacity = 1;
    bb_null_data.position = 0;
    byte = unstdbytebuffer_read_byte(&bb_null_data, &err);
    assert(byte == 0);
    assert(err == 3);

    // [4] Read would exceed limit
    u8t data_limit[1] = {0xAA};
    unstdbytebuffer bb_limit = {
            .buffer = {.data = data_limit, .length = 1, .capacity = 1},
            .position = 1,
            .limit = 1
    };
    byte = unstdbytebuffer_read_byte(&bb_limit, &err);
    assert(byte == 0);
    assert(err == 4);

    // [5] Read would exceed capacity
    u8t data_capacity[1] = {0xBB};
    unstdbytebuffer bb_capacity = {
            .buffer = {.data = data_capacity, .length = 1, .capacity = 1},
            .position = 1,
            .limit = 8
    };
    byte = unstdbytebuffer_read_byte(&bb_capacity, &err);
    assert(byte == 0);
    assert(err == 5);

    // [6] Read would exceed length
    u8t data_length[1] = {0xCC};
    unstdbytebuffer bb_length = {
            .buffer = {.data = data_length, .length = 1, .capacity = 8},
            .position = 1,
            .limit = 8
    };
    byte = unstdbytebuffer_read_byte(&bb_length, &err);
    assert(byte == 0);
    assert(err == 6);

    // [1] Success: write then read byte
    unstdbytebuffer *bb = unstdbytebuffer_new(8, 32, &err);
    assert(bb != NULL);
    assert(err == 1);

    res = unstdbytebuffer_write_byte(bb, 0x5A, &err);
    assert(res == true);
    assert(err == 1);

    bb->position = 0;
    byte = unstdbytebuffer_read_byte(bb, &err);
    assert(err == 1);
    assert(byte == 0x5A);
    assert(bb->position == 1);

    unstdbytebuffer_destroy(bb, &err);
    assert(err == 1);

    _notify("[+]", "`unstdbytebuffer_read_byte()` passed");
}

//! [read_vuint]
void test_unstdbytebuffer_read_vuint(void) {
    u8t err;
    u64t value;
    bool res;

    // [2] Invalid argument: NULL buffer
    value = unstdbytebuffer_read_vuint(NULL, 0, &err);
    assert(value == 0);
    assert(err == 2);

    // Setup buffer for tests
    unstdbytebuffer *bb = unstdbytebuffer_new(8, 32, &err);
    assert(bb != NULL);
    assert(err == 1);

    // [3] Read error: underlying read_byte fails (simulate by setting length to 0 so read_byte fails)
    bb->buffer.length = 0;
    bb->position = 0;
    value = unstdbytebuffer_read_vuint(bb, 0, &err);
    assert(value == 0);
    assert(err == 3);

    // Reset for next tests
    bb->buffer.capacity = 8;
    bb->position = 0;

    // Write single-byte varuint 0x7F (no continuation bit)
    res = unstdbytebuffer_write_byte(bb, 0x7F, &err);
    assert(res == true);
    assert(err == 1);
    bb->position = 0;

    // [1] Success: single-byte varuint
    value = unstdbytebuffer_read_vuint(bb, 0, &err);
    assert(value == 0x7F);
    assert(err == 1);

    // Reset buffer to write multi-byte varuint: 0x80 0x01 (128)
    bb->position = 0;
    bb->buffer.length = 0;

    res = unstdbytebuffer_write_byte(bb, 0x80, &err);
    assert(res == true);
    res = unstdbytebuffer_write_byte(bb, 0x01, &err);
    assert(res == true);
    bb->position = 0;

    // [1] Success: multi-byte varuint
    value = unstdbytebuffer_read_vuint(bb, 0, &err);
    assert(value == 128);
    assert(err == 1);

    // Clear for next test
    bb->position = 0;

    // [1] Success: write and read single-byte varuint (0x7F)
    res = unstdbytebuffer_write_vuint(bb, 0x7F, &err);
    assert(res == true);
    assert(err == 1);
    bb->position = 0;

    value = unstdbytebuffer_read_vuint(bb, 0, &err);
    assert(value == 0x7F);
    assert(err == 1);

    // Clear for next test
    bb->position = 0;

    // [1] Success: write and read multi-byte varuint (128 = 0x80 0x01)
    res = unstdbytebuffer_write_vuint(bb, 0x80, &err);
    assert(res == true);
    assert(err == 1);

    bb->position = 0;

    value = unstdbytebuffer_read_vuint(bb, 2, &err);
    assert(value == 0x80);
    assert(err == 1);


    // Reset buffer for malformed varuint test (maxv_arg = 1, first byte has continuation bit)
    bb->position = 0;
    bb->buffer.length = 0;

    res = unstdbytebuffer_write_byte(bb, 0x80, &err);
    assert(res == true);
    bb->position = 0;

    // [4] Malformed varuint (too long for maxv_arg)
    value = unstdbytebuffer_read_vuint(bb, 1, &err);
    assert(value == 0);
    assert(err == 4);

    unstdbytebuffer_destroy(bb, &err);
    assert(err == 1);

    _notify("[+]", "`unstdbytebuffer_read_vuint()` passed");
}

//! [read_bytes]
void test_unstdbytebuffer_read_bytes(void) {
    u8t err;
    u8t *ptr;

    // [2] Invalid argument: NULL buffer
    ptr = unstdbytebuffer_read_bytes(NULL, 4, &err);
    assert(ptr == NULL);
    assert(err == 2);

    // [3] Invalid buffer: data == NULL
    unstdbytebuffer bb_invalid = {0};
    bb_invalid.buffer.length = 8;
    bb_invalid.buffer.capacity = 8;
    bb_invalid.position = 0;
    ptr = unstdbytebuffer_read_bytes(&bb_invalid, 4, &err);
    assert(ptr == NULL);
    assert(err == 3);

    // [4] Read exceeds limit
    u8t data_limit[8] = {0};
    unstdbytebuffer bb_limit = {
            .buffer = {.data = data_limit, .capacity = 8, .length = 8},
            .position = 6,
            .limit = 8
    };
    ptr = unstdbytebuffer_read_bytes(&bb_limit, 4, &err);
    assert(ptr == NULL);
    assert(err == 4);

    // [5] Read exceeds capacity
    u8t data_cap[10] = {0};
    unstdbytebuffer bb_cap = {
            .buffer = {.data = data_cap, .capacity = 8, .length = 10},
            .position = 6,
            .limit = 10
    };
    ptr = unstdbytebuffer_read_bytes(&bb_cap, 4, &err);
    assert(ptr == NULL);
    assert(err == 5);

    // [6] Read exceeds current length
    u8t data_len[16] = {0};
    unstdbytebuffer bb_len = {
            .buffer = {.data = data_len, .capacity = 16, .length = 8},
            .position = 4,
            .limit = 16
    };
    ptr = unstdbytebuffer_read_bytes(&bb_len, 6, &err);
    assert(ptr == NULL);
    assert(err == 6);

    // [1] Success: read valid bytes
    u8t data_ok[16] = {1, 2, 3, 4, 5, 6, 7, 8};
    unstdbytebuffer bb_ok = {
            .buffer = {.data = data_ok, .capacity = 16, .length = 8},
            .position = 2,
            .limit = 16
    };
    ptr = unstdbytebuffer_read_bytes(&bb_ok, 4, &err);
    assert(ptr != NULL);
    assert(err == 1);
    assert(ptr[0] == 3);
    assert(ptr[1] == 4);
    assert(ptr[2] == 5);
    assert(ptr[3] == 6);
    assert(&ptr[0] == &data_ok[2]);
    assert(&ptr[1] == &data_ok[3]);
    assert(&ptr[2] == &data_ok[4]);
    assert(&ptr[3] == &data_ok[5]);
    assert(bb_ok.position == 6);

    _notify("[+]", "`unstdbytebuffer_read_bytes()` passed");
}

//! [read_bytes_copy]
void test_unstdbytebuffer_read_bytes_copy(void) {
    u8t err;
    u8t *copy;

    // [2] Invalid argument: NULL buffer
    copy = unstdbytebuffer_read_bytes_copy(NULL, 4, &err);
    assert(copy == NULL);
    assert(err == 2);

    // [3] Invalid buffer: data == NULL
    unstdbytebuffer bb_invalid = {0};
    bb_invalid.buffer.length = 8;
    bb_invalid.buffer.capacity = 8;
    bb_invalid.position = 0;
    copy = unstdbytebuffer_read_bytes_copy(&bb_invalid, 4, &err);
    assert(copy == NULL);
    assert(err == 3);

    // [4] Read exceeds limit
    u8t data_limit[8] = {0};
    unstdbytebuffer bb_limit = {
            .buffer = {.data = data_limit, .capacity = 8, .length = 8},
            .position = 6,
            .limit = 8
    };
    copy = unstdbytebuffer_read_bytes_copy(&bb_limit, 4, &err);
    assert(copy == NULL);
    assert(err == 4);

    // [5] Read exceeds capacity
    u8t data_cap[10] = {0};
    unstdbytebuffer bb_cap = {
            .buffer = {.data = data_cap, .capacity = 8, .length = 10},
            .position = 6,
            .limit = 10
    };
    copy = unstdbytebuffer_read_bytes_copy(&bb_cap, 4, &err);
    assert(copy == NULL);
    assert(err == 5);

    // [6] Read exceeds current length
    u8t data_len[16] = {0};
    unstdbytebuffer bb_len = {
            .buffer = {.data = data_len, .capacity = 16, .length = 8},
            .position = 4,
            .limit = 16
    };
    copy = unstdbytebuffer_read_bytes_copy(&bb_len, 6, &err);
    assert(copy == NULL);
    assert(err == 6);

    // [1] Success: read and copy bytes
    u8t data_ok[16] = {10, 20, 30, 40, 50, 60, 70, 80};
    unstdbytebuffer bb_ok = {
            .buffer = {.data = data_ok, .capacity = 16, .length = 8},
            .position = 2,
            .limit = 16
    };
    copy = unstdbytebuffer_read_bytes_copy(&bb_ok, 4, &err);
    assert(copy != NULL);
    assert(err == 1);
    assert(copy[0] == 30);
    assert(copy[1] == 40);
    assert(copy[2] == 50);
    assert(copy[3] == 60);
    assert(&copy[0] != &data_ok[2]);
    assert(&copy[1] != &data_ok[3]);
    assert(&copy[2] != &data_ok[4]);
    assert(&copy[3] != &data_ok[5]);
    assert(bb_ok.position == 6);
    free(copy);

    _notify("[+]", "`unstdbytebuffer_read_bytes_copy()` passed");
}


//! [read_bytearray]
void test_unstdbytebuffer_read_bytearray(void) {
    u8t err;
    ubytes result;

    // [2] Invalid argument: NULL buffer
    result = unstdbytebuffer_read_bytearray(NULL, &err);
    assert(result.data == NULL);
    assert(result.length == 0);
    assert(err == 2);

    // [3] Failed to read length prefix
    {
        u8t malformed[1] = {0xFF}; // invalid varuint (unterminated)
        unstdbytebuffer bb = {
                .buffer = {.data = malformed, .capacity = 1, .length = 1},
                .position = 0,
                .limit = 1
        };
        result = unstdbytebuffer_read_bytearray(&bb, &err);
        assert(result.data == NULL);
        assert(result.length == 0);
        assert(err == 3);
    }

    // [4] Failed to read slice: not enough data
    {
        unstdbytebuffer *bb = unstdbytebuffer_new_overload_0(16, &err);
        assert(unstdbytebuffer_write_bytearray(bb, (u8t[]) {0xA1, 0xB2}, 0, 2, &err));
        bb->limit = bb->buffer.length - 1; // artificially reduce available data
        bb->position = 0;
        result = unstdbytebuffer_read_bytearray(bb, &err);
        assert(result.data == NULL);
        assert(result.length == 0);
        assert(err == 4);
        unstdbytebuffer_destroy(bb, &err);
    }

    // [1] Success
    {
        u8t payload[4] = {0xDE, 0xAD, 0xBE, 0xEF};
        unstdbytebuffer *bb = unstdbytebuffer_new_overload_0(16, &err);
        assert(err == 1);
        assert(unstdbytebuffer_write_bytearray(bb, payload, 0, 4, &err));
        assert(err == 1);
        bb->position = 0;

        result = unstdbytebuffer_read_bytearray(bb, &err);
        assert(result.data != NULL);
        assert(result.length == 4);
        assert(result.capacity == 4);
        assert(result.data[0] == 0xDE);
        assert(result.data[1] == 0xAD);
        assert(result.data[2] == 0xBE);
        assert(result.data[3] == 0xEF);
        assert(&result.data[0] == &bb->buffer.data[1]); // skip varuint header
        assert(bb->position == 5);
        assert(err == 1);
        unstdbytebuffer_destroy(bb, &err);
    }

    _notify("[+]", "`unstdbytebuffer_read_bytearray()` passed");
}

//! [read_bytearray_copy]
void test_unstdbytebuffer_read_bytearray_copy(void) {
    u8t err;
    ubytes result;

    // [2] Invalid argument: NULL buffer
    result = unstdbytebuffer_read_bytearray_copy(NULL, &err);
    assert(result.data == NULL);
    assert(result.length == 0);
    assert(err == 2);

    // [3] Invalid buffer: data == NULL
    unstdbytebuffer bb_invalid = {0};
    bb_invalid.buffer.length = 8;
    bb_invalid.buffer.capacity = 8;
    bb_invalid.position = 0;
    result = unstdbytebuffer_read_bytearray_copy(&bb_invalid, &err);
    assert(result.data == NULL);
    assert(result.length == 0);
    assert(err == 3);

    // [4] Failed to read length prefix
    {
        u8t malformed[1] = {0xFF}; // invalid varuint (unterminated)
        unstdbytebuffer bb = {
                .buffer = {.data = malformed, .capacity = 1, .length = 1},
                .position = 0,
                .limit = 1
        };
        result = unstdbytebuffer_read_bytearray_copy(&bb, &err);
        assert(result.data == NULL);
        assert(result.length == 0);
        assert(err == 4);
    }

    // [5] Failed to read byte array copy (not enough data)
    {
        unstdbytebuffer *bb = unstdbytebuffer_new_overload_0(16, &err);
        assert(unstdbytebuffer_write_bytearray(bb, (u8t[]) {1, 2, 3}, 0, 3, &err));
        bb->buffer.length -= 1; // simulate truncation
        bb->position = 0;

        result = unstdbytebuffer_read_bytearray_copy(bb, &err);
        assert(result.data == NULL);
        assert(result.length == 0);
        assert(err == 5);

        unstdbytebuffer_destroy(bb, &err);
    }

    // [1] Success: copy valid array
    {
        u8t payload[4] = {0xCA, 0xFE, 0xBA, 0xBE};
        unstdbytebuffer *bb = unstdbytebuffer_new_overload_0(16, &err);
        assert(unstdbytebuffer_write_bytearray(bb, payload, 0, 4, &err));
        bb->position = 0;

        result = unstdbytebuffer_read_bytearray_copy(bb, &err);
        assert(result.data != NULL);
        assert(result.length == 4);
        assert(result.capacity == 4);
        assert(result.data[0] == 0xCA);
        assert(result.data[1] == 0xFE);
        assert(result.data[2] == 0xBA);
        assert(result.data[3] == 0xBE);
        assert(result.data != &bb->buffer.data[1]); // must be a separate copy
        assert(bb->position == 5);
        assert(err == 1);

        free(result.data); // cleanup
        unstdbytebuffer_destroy(bb, &err);
    }

    _notify("[+]", "`unstdbytebuffer_read_bytearray_copy()` passed");
}


void test_unstdbytebuffer(void) {
    //! [Constructors]
    test_unstdbytebuffer_new();
    test_unstdbytebuffer_new_overload_0();
    test_unstdbytebuffer_new_from_data();
    test_unstdbytebuffer_new_from_data_copy();

    test_unstdbytebuffer_evaluatefit();

    //! [Destructors]
    test_unstdbytebuffer_destroy();

    //! [Getter/Setters]
    //! [position]
    //! [get]
    test_unstdbytebuffer_position();
    //! [set]
    test_unstdbytebuffer_set_position();

    //! [position]
    //! [get]
    test_unstdbytebuffer_limit();
    //! [set]
    test_unstdbytebuffer_set_limit();

    //! [remaining]
    //! [get]
    test_unstdbytebuffer_remaining();
    test_unstdbytebuffer_has_remaining();

    //! [length]
    //! [get]
    test_unstdbytebuffer_length();

    //! [bytes]
    //! [get]
    test_unstdbytebuffer_bytes();

    test_unstdbytebuffer_rewind();
    test_unstdbytebuffer_compact();
    test_unstdbytebuffer_skip();
    test_unstdbytebuffer_reuse();

    //! [Writes]
    test_unstdbytebuffer_write_uint32();
    test_unstdbytebuffer_write_int32();
    test_unstdbytebuffer_write_uint64();
    test_unstdbytebuffer_write_int64();
    test_unstdbytebuffer_write_byte();
    test_unstdbytebuffer_write_vuint();
    test_unstdbytebuffer_write_bool();
    test_unstdbytebuffer_write_double();
    test_unstdbytebuffer_write_bytes();
    test_unstdbytebuffer_write_bytes_offset();
    test_unstdbytebuffer_write_bytearray();

    //! [Reads]
    test_unstdbytebuffer_read_uint32();
    test_unstdbytebuffer_read_int32();
    test_unstdbytebuffer_read_uint64();
    test_unstdbytebuffer_read_int64();
    test_unstdbytebuffer_read_bool();
    test_unstdbytebuffer_read_double();
    test_unstdbytebuffer_read_byte();
    test_unstdbytebuffer_read_vuint();
    test_unstdbytebuffer_read_bytes();
    test_unstdbytebuffer_read_bytes_copy();
    test_unstdbytebuffer_read_bytearray();
    test_unstdbytebuffer_read_bytearray_copy();

    _notify("[+]", "`unstdbytebuffer` passed");
}
