#include "../../_test_common.h"

// Headers
#include "../../../src/unstdinttypes.h"
#include "../../../src/unstdstring.h"


//! [new]
void test_unstdstring_new(void) {
    u8t result;

    // [1] Create a new string with a valid buffer and sufficient limit
    const i8 *valid_buffer = "Hello, World!";
    unstdstring *string = unstdstring_new(valid_buffer, 50, &result);
    assert(string != NULL);
    assert(result == 1);
    assert(string->length == 13);
    assert(string->limit == 50);
    assert(memcmp(string->data, valid_buffer, 13) == 0);
    assert(string->data[13] == '\0'); // Ensure null-terminated
    assert(string->capacity == strlen((i8 *) string->data) + 1);
    free(string->data);
    free(string);

    // [2] Create a new string with a NULL buffer and valid limit
    string = unstdstring_new(NULL, 20, &result);
    assert(string != NULL);
    assert(result == 1);
    assert(string->length == 0);
    assert(string->limit == 20);
    assert(string->data[0] == '\0'); // Empty buffer
    assert(string->capacity == 2);
    free(string->data);
    free(string);

    // [3] Create a new string with an empty buffer and valid limit
    const i8 *empty_buffer = "";
    string = unstdstring_new(empty_buffer, 10, &result);
    assert(string != NULL);
    assert(result == 1);
    assert(string->length == 0);
    assert(string->limit == 10);
    assert(string->data[0] == '\0'); // Empty buffer
    assert(string->capacity == 2);
    free(string->data);
    free(string);

    // [3] Create a new string with an empty buffer and infinite limit
    string = unstdstring_new(NULL, 0, &result);
    assert(string != NULL);
    assert(result == 1);
    assert(string->length == 0);
    assert(string->limit == 0);
    assert(string->data[0] == '\0'); // Empty buffer
    assert(string->capacity == 2);
    free(string->data);
    free(string);

    // [4] Fail due to insufficient limit for buffer
    string = unstdstring_new(valid_buffer, 5, &result);
    assert(string == NULL);
    assert(result == 3);
    free(string);

    _notify("[+]", "`unstdstring_new()` passed");
}

//! [set]
void test_unstdstring_set(void) {
    u8t result;

    // [1] Set a valid non-empty string into a buffer with sufficient capacity and limit
    unstdstring *s1 = unstdstring_new(NULL, 100, &result);
    assert(s1 != NULL);
    assert(result == 1);
    result = unstdstring_set(s1, "Hello, World!");
    assert(result == 1);
    assert(s1->length == 13);
    assert(strcmp(s1->data, "Hello, World!") == 0);
    assert(s1->data[13] == '\0');
    free(s1->data);
    free(s1);


    // [2] Set a valid non-empty string into a buffer with sufficient capacity and limit (no realloc)
    s1 = unstdstring_new("Hello!", 100, &result);
    assert(s1 != NULL);
    assert(result == 1);
    sizet old_capacity = s1->capacity;
    result = unstdstring_set(s1, "Hi");  // shorter string, fits in existing capacity
    assert(result == 1);
    assert(s1->length == 2);
    assert(strcmp(s1->data, "Hi") == 0);
    assert(s1->data[2] == '\0');
    assert(s1->capacity == old_capacity); // capacity unchanged, no realloc
    free(s1->data);
    free(s1);

    // [3] Set a string that requires realloc (smaller initial capacity)
    unstdstring *s2 = unstdstring_new("abc", 50, &result);
    assert(s2 != NULL);
    assert(result == 1);
    s2->capacity = 4; // force small capacity
    result = unstdstring_set(s2, "Expanded!");
    assert(result == 1);
    assert(s2->length == 9);
    assert(strcmp(s2->data, "Expanded!") == 0);
    assert(s2->data[9] == '\0');
    assert(s2->capacity >= 10); // capacity should have grown
    free(s2->data);
    free(s2);

    // [4] Fail due to NULL `buffer_arg`
    result = unstdstring_set(NULL, "Hello");
    assert(result == 2);

    // [5] Fail due to NULL `new_string`
    unstdstring *s3 = unstdstring_new("Init", 30, &result);
    assert(s3 != NULL);
    assert(result == 1);
    result = unstdstring_set(s3, NULL);
    assert(result == 3);
    free(s3->data);
    free(s3);

    // [6] Fail due to exceeding `limit`
    unstdstring *s4 = unstdstring_new("abc", 5, &result);
    assert(s4 != NULL);
    assert(result == 1);
    result = unstdstring_set(s4, "StringTooLong");
    assert(result == 4);
    free(s4->data);
    free(s4);

    _notify("[+]", "`unstdstring_set()` passed");
}

//! [free]
void test_unstdstring_free(void) {
    u8t result;

    // [1] Free a valid unstdstring with allocated memory
    const i8 *valid_buffer = "Sample String";
    unstdstring *string = unstdstring_new(valid_buffer, 50, &result);
    assert(string != NULL);
    assert(result == 1);
    assert(string->length == 13);
    assert(string->limit == 50);

    // Free the string and check for success
    result = unstdstring_free(string);
    assert(result == 1); // Success

    // [2] Try to free a NULL string (should return 2)
    result = unstdstring_free(NULL);
    assert(result == 2); // NULL buffer argument

    // [3] Try to free a string with capacity 0 (should return 3)
    unstdstring *empty_string = malloc(sizeof(unstdstring));
    empty_string->data = NULL;
    empty_string->capacity = 0;
    empty_string->limit = 0;
    empty_string->length = 0;

    result = unstdstring_free(empty_string);
    assert(result == 3); // No allocated memory to free
    free(empty_string);

    _notify("[+]", "`unstdstring_free()` passed");
}

//! [reset]
void test_unstdstring_reset(void) {
    u8t result;

    // Test case 1: Reset a valid string
    unstdstring *string = unstdstring_new("Reset This String", 50, &result);
    assert(string != NULL);
    assert(result == 1);
    assert(string->length == 17); // "Reset This String"
    assert(string->limit == 50);

    // Reset the string and check its properties
    result = unstdstring_reset(string);
    assert(result == 1); // Success
    assert(string->length == 0); // Length should be 0 after reset
    assert(string->capacity == 2); // Allocated size should reset
    assert(memcmp(string->data, "", 1) == 0); // Data should be empty
    free(string->data);
    free(string);

    // Test case 2: Reset an empty string (no data)
    unstdstring *empty_string = unstdstring_new("", 50, &result);
    assert(empty_string != NULL);
    assert(result == 1);
    assert(empty_string->length == 0);

    result = unstdstring_reset(empty_string);
    assert(result == 1);
    free(empty_string->data);
    free(empty_string);

    // Test case 3: Try to reset a NULL string (should return 2)
    result = unstdstring_reset(NULL);
    assert(result == 2); // NULL string argument

    // Notify that all tests passed
    _notify("[+]", "`unstdstring_reset()` passed");
}


//! [clear]
void test_unstdstring_clear(void) {
    u8t result;

    // Test case 1: Clear a valid string
    const i8 *valid_buffer = "Clear This String";
    unstdstring *string = unstdstring_new(valid_buffer, 50, &result);
    sizet old_capacity = string->capacity;
    assert(string != NULL);
    assert(result == 1);
    assert(string->length == 17); // "Clear This String"
    assert(string->limit == 50);

    // Clear the string and check its properties
    result = unstdstring_clear(string);
    assert(result == 1); // Success
    assert(string->length == 0); // Length should be 0 after clear
    assert(string->capacity == old_capacity); // Allocated size shall not change
    assert(memcmp(string->data, "", 1) == 0); // Data should be empty
    free(string->data);
    free(string);

    // Test case 2: Clear an empty string (no data)
    const i8 *empty_buffer = "";
    unstdstring *empty_string = unstdstring_new(empty_buffer, 50, &result);
    assert(empty_string != NULL);
    assert(result == 1);
    assert(empty_string->length == 0);

    result = unstdstring_clear(empty_string);
    assert(result == 1);

    // Test case 3: Try to clear a NULL string (should return 2)
    result = unstdstring_clear(NULL);
    assert(result == 2); // NULL string argument

    free(empty_string->data);
    free(empty_string);

    // Notify that all tests passed
    _notify("[+]", "`unstdstring_clear()` passed");
}

//! [equal]
void test_unstdstring_equal(void) {
    u8t result;

    // Test 1: Equal strings
    unstdstring *str1 = unstdstring_new("EqualTest", 32, &result);
    assert(str1 && result == 1);

    unstdstring *str2 = unstdstring_new("EqualTest", 32, &result);
    assert(str2 && result == 1);

    bool is_equal = unstdstring_equal(str1, str2, &result);
    assert(is_equal == true);
    assert(result == 1);

    // Test 2: Different strings with same length
    unstdstring *str3 = unstdstring_new("Different", 32, &result);
    assert(str3 && result == 1);

    is_equal = unstdstring_equal(str1, str3, &result);
    assert(is_equal == false);
    assert(result == 1);

    // Test 3: One empty, one non-empty
    unstdstring *empty_str = unstdstring_new("", 32, &result);
    assert(empty_str && result == 1);

    is_equal = unstdstring_equal(str1, empty_str, &result);
    assert(is_equal == false);
    assert(result == 4);

    // Test 4: Both empty
    unstdstring *empty_str2 = unstdstring_new("", 32, &result);
    assert(empty_str2 && result == 1);

    is_equal = unstdstring_equal(empty_str, empty_str2, &result);
    assert(is_equal == true);
    assert(result == 1);

    // Test 5: NULL inputs
    is_equal = unstdstring_equal(NULL, str1, &result);
    assert(is_equal == false);
    assert(result == 2);

    is_equal = unstdstring_equal(str1, NULL, &result);
    assert(is_equal == false);
    assert(result == 2);

    // Test 6: NULL data field
    unstdstring *corrupt_str = unstdstring_new("X", 32, &result);
    assert(corrupt_str && result == 1);
    free(corrupt_str->data); // simulate corruption
    corrupt_str->data = NULL;

    is_equal = unstdstring_equal(str1, corrupt_str, &result);
    assert(is_equal == false);
    assert(result == 3);

    // Cleanup
    unstdstring_free(str1);
    unstdstring_free(str2);
    unstdstring_free(str3);
    unstdstring_free(empty_str);
    unstdstring_free(empty_str2);
    unstdstring_free(corrupt_str);

    _notify("[+]", "`unstdstring_equal()` passed");
}

//! [equal_overload_0]
void test_unstdstring_equal_overload_0(void) {
    u8t result;

    // Test 1: Equal strings
    unstdstring *ustr = unstdstring_new("HelloWorld", 32, &result);
    assert(ustr && result == 1);

    bool eq = unstdstring_equal_overload_0(ustr, "HelloWorld", &result);
    assert(eq == true);
    assert(result == 1);

    // Test 2: Different string content
    eq = unstdstring_equal_overload_0(ustr, "Different", &result);
    assert(eq == false);
    assert(result == 1);

    // Test 3: Different length, same prefix
    eq = unstdstring_equal_overload_0(ustr, "Hello", &result);
    assert(eq == false);
    assert(result == 1);

    // Test 4: NULL unstdstring pointer
    eq = unstdstring_equal_overload_0(NULL, "HelloWorld", &result);
    assert(eq == false);
    assert(result == 2);

    // Test 5: NULL data in unstdstring
    unstdstring *corrupt = unstdstring_new("Corrupt", 32, &result);
    assert(corrupt && result == 1);
    free(corrupt->data);
    corrupt->data = NULL;

    eq = unstdstring_equal_overload_0(corrupt, "Corrupt", &result);
    assert(eq == false);
    assert(result == 3);

    // Test 6: NULL C-string pointer
    eq = unstdstring_equal_overload_0(ustr, NULL, &result);
    assert(eq == false);
    assert(result == 4);

    // Test 7: One empty, one not
    unstdstring *empty = unstdstring_new("", 32, &result);
    assert(empty && result == 1);

    eq = unstdstring_equal_overload_0(empty, "NonEmpty", &result);
    assert(eq == false);
    assert(result == 5);

    eq = unstdstring_equal_overload_0(ustr, "", &result);
    assert(eq == false);
    assert(result == 5);

    // Test 8: Both empty
    eq = unstdstring_equal_overload_0(empty, "", &result);
    assert(eq == true);
    assert(result == 1);

    // Cleanup
    unstdstring_free(ustr);
    unstdstring_free(corrupt);
    unstdstring_free(empty);

    _notify("[+]", "`unstdstring_equal_overload_0()` passed");
}

//! [equal_ignorecase]
void test_unstdstring_equal_ignorecase(void) {
    u8t result;

    // Test 1: Identical strings, same case
    unstdstring *a = unstdstring_new("CaseTest", 32, &result);
    unstdstring *b = unstdstring_new("CaseTest", 32, &result);
    assert(a && b && result == 1);

    bool eq = unstdstring_equal_ignorecase(a, b, &result);
    assert(eq == true);
    assert(result == 1);
    unstdstring_free(b);

    // Test 2: Identical strings, different case
    b = unstdstring_new("casetest", 32, &result);
    assert(b && result == 1);
    eq = unstdstring_equal_ignorecase(a, b, &result);
    assert(eq == true);
    assert(result == 1);
    unstdstring_free(b);

    // Test 3: Different content
    b = unstdstring_new("casefail", 32, &result);
    assert(b && result == 1);
    eq = unstdstring_equal_ignorecase(a, b, &result);
    assert(eq == false);
    assert(result == 1);
    unstdstring_free(b);

    // Test 4: Different length
    b = unstdstring_new("casetesting", 32, &result);
    assert(b && result == 1);
    eq = unstdstring_equal_ignorecase(a, b, &result);
    assert(eq == false);
    assert(result == 1);
    unstdstring_free(b);

    // Test 5: One empty, one not
    unstdstring *empty = unstdstring_new("", 16, &result);
    assert(empty && result == 1);
    eq = unstdstring_equal_ignorecase(empty, a, &result);
    assert(eq == false);
    assert(result == 4);

    eq = unstdstring_equal_ignorecase(empty, empty, &result);
    assert(eq == true);
    assert(result == 1);

    // Test 6: NULL parameters
    eq = unstdstring_equal_ignorecase(NULL, a, &result);
    assert(eq == false);
    assert(result == 2);

    eq = unstdstring_equal_ignorecase(a, NULL, &result);
    assert(eq == false);
    assert(result == 2);

    // Test 7: NULL `data` inside unstdstring
    unstdstring *corrupt = unstdstring_new("X", 4, &result);
    free(corrupt->data);
    corrupt->data = NULL;

    eq = unstdstring_equal_ignorecase(corrupt, a, &result);
    assert(eq == false);
    assert(result == 3);

    // Cleanup
    unstdstring_free(a);
    unstdstring_free(empty);
    unstdstring_free(corrupt);

    _notify("[+]", "`unstdstring_equal_ignorecase()` passed");
}

//! [equal_ignorecase_overload_0]
void test_unstdstring_equal_ignorecase_overload_0(void) {
    u8t result;

    // Test 1: Identical strings, same case
    unstdstring *a = unstdstring_new("BufferTest", 32, &result);
    assert(a && result == 1);

    bool eq = unstdstring_equal_ignorecase_overload_0(a, "BufferTest", &result);
    assert(eq == true);
    assert(result == 1);

    // Test 2: Identical strings, different case
    eq = unstdstring_equal_ignorecase_overload_0(a, "buffertest", &result);
    assert(eq == true);
    assert(result == 1);

    // Test 3: Different content
    eq = unstdstring_equal_ignorecase_overload_0(a, "bufferfail", &result);
    assert(eq == false);
    assert(result == 1);

    // Test 4: Different length
    eq = unstdstring_equal_ignorecase_overload_0(a, "BufferTesting", &result);
    assert(eq == false);
    assert(result == 1);

    // Test 5: One empty, one not
    unstdstring *empty = unstdstring_new("", 16, &result);
    assert(empty && result == 1);

    eq = unstdstring_equal_ignorecase_overload_0(empty, "nonempty", &result);
    assert(eq == false);
    assert(result == 5);

    eq = unstdstring_equal_ignorecase_overload_0(a, "", &result);
    assert(eq == false);
    assert(result == 5);

    // Test 6: Both empty
    eq = unstdstring_equal_ignorecase_overload_0(empty, "", &result);
    assert(eq == true);
    assert(result == 1);

    // Test 7: NULL string argument
    eq = unstdstring_equal_ignorecase_overload_0(a, NULL, &result);
    assert(eq == false);
    assert(result == 4);

    // Test 8: NULL unstdstring argument
    eq = unstdstring_equal_ignorecase_overload_0(NULL, "test", &result);
    assert(eq == false);
    assert(result == 2);

    // Test 9: NULL data inside unstdstring
    unstdstring *corrupt = unstdstring_new("Z", 4, &result);
    free(corrupt->data);
    corrupt->data = NULL;

    eq = unstdstring_equal_ignorecase_overload_0(corrupt, "Z", &result);
    assert(eq == false);
    assert(result == 3);

    // Cleanup
    unstdstring_free(a);
    unstdstring_free(empty);
    unstdstring_free(corrupt);

    _notify("[+]", "`unstdstring_equal_ignorecase_overload_0()` passed");
}

//! [startswith_char]
void test_unstdstring_startswith_char(void) {
    u8t result;

    // Test 1: String starts with target char
    unstdstring *s1 = unstdstring_new("Apple", 16, &result);
    assert(s1 && result == 1);

    bool starts = unstdstring_startswith_char(s1, 'A', &result);
    assert(starts == true);
    assert(result == 1);

    // Test 2: String does not start with target char
    starts = unstdstring_startswith_char(s1, 'B', &result);
    assert(starts == false);
    assert(result == 1);

    // Test 3: Empty string
    unstdstring *empty = unstdstring_new("", 8, &result);
    assert(empty && result == 1);

    starts = unstdstring_startswith_char(empty, 'X', &result);
    assert(starts == false);
    assert(result == 4);  // Error code for empty string

    // Test 4: NULL buffer_arg
    starts = unstdstring_startswith_char(NULL, 'A', &result);
    assert(starts == false);
    assert(result == 2);  // NULL buffer_arg

    // Test 5: NULL data in buffer
    unstdstring *corrupt = unstdstring_new("Zebra", 8, &result);
    assert(corrupt && result == 1);
    free(corrupt->data);
    corrupt->data = NULL;

    starts = unstdstring_startswith_char(corrupt, 'Z', &result);
    assert(starts == false);
    assert(result == 3);  // NULL data

    // Cleanup
    unstdstring_free(s1);
    unstdstring_free(empty);
    unstdstring_free(corrupt);

    _notify("[+]", "`unstdstring_startswith_char()` passed");
}

//! [startswith_char_ignorecase]
void test_unstdstring_startswith_char_ignorecase(void) {
    u8t result;

    // Test 1: Matches ignoring case (e.g., 'A' vs 'a')
    unstdstring *s1 = unstdstring_new("Apple", 16, &result);
    assert(s1 && result == 1);

    bool starts = unstdstring_startswith_char_ignorecase(s1, 'a', &result);
    assert(starts == true);
    assert(result == 1);

    // Test 2: Mismatch even after ignoring case
    starts = unstdstring_startswith_char_ignorecase(s1, 'b', &result);
    assert(starts == false);
    assert(result == 1);

    // Test 3: Exact match same case
    starts = unstdstring_startswith_char_ignorecase(s1, 'A', &result);
    assert(starts == true);
    assert(result == 1);

    // Test 4: Empty string
    unstdstring *empty = unstdstring_new("", 8, &result);
    assert(empty && result == 1);

    starts = unstdstring_startswith_char_ignorecase(empty, 'X', &result);
    assert(starts == false);
    assert(result == 4);  // Empty string

    // Test 5: NULL buffer_arg
    starts = unstdstring_startswith_char_ignorecase(NULL, 'A', &result);
    assert(starts == false);
    assert(result == 2);  // NULL buffer_arg

    // Test 6: NULL data
    unstdstring *corrupt = unstdstring_new("Hello", 8, &result);
    assert(corrupt && result == 1);
    free(corrupt->data);
    corrupt->data = NULL;

    starts = unstdstring_startswith_char_ignorecase(corrupt, 'H', &result);
    assert(starts == false);
    assert(result == 3);  // NULL data field

    // Cleanup
    unstdstring_free(s1);
    unstdstring_free(empty);
    unstdstring_free(corrupt);

    _notify("[+]", "`unstdstring_startswith_char_ignorecase()` passed");
}

//! [endswith_char]
void test_unstdstring_endswith_char(void) {
    u8t result;

    // Test 1: Ends with expected char
    unstdstring *s1 = unstdstring_new("Hello", 16, &result);
    assert(s1 && result == 1);

    bool ends = unstdstring_endswith_char(s1, 'o', &result);
    assert(ends == true);
    assert(result == 1);

    // Test 2: Does not end with expected char
    ends = unstdstring_endswith_char(s1, 'H', &result);
    assert(ends == false);
    assert(result == 1);

    // Test 3: Empty string
    unstdstring *empty = unstdstring_new("", 8, &result);
    assert(empty && result == 1);

    ends = unstdstring_endswith_char(empty, 'x', &result);
    assert(ends == false);
    assert(result == 4); // empty string

    // Test 4: NULL buffer_arg
    ends = unstdstring_endswith_char(NULL, 'x', &result);
    assert(ends == false);
    assert(result == 2); // buffer_arg is NULL

    // Test 5: NULL data inside struct
    unstdstring *corrupt = unstdstring_new("Corrupt", 8, &result);
    assert(corrupt && result == 1);
    free(corrupt->data);
    corrupt->data = NULL;

    ends = unstdstring_endswith_char(corrupt, 't', &result);
    assert(ends == false);
    assert(result == 3); // data field is NULL

    // Test 6: length is less than 1 (simulate manually)
    unstdstring *invalid_len = unstdstring_new("X", 8, &result);
    assert(invalid_len && result == 1);
    invalid_len->length = 0; // simulate invalid internal state

    ends = unstdstring_endswith_char(invalid_len, 'X', &result);
    assert(ends == false);
    assert(result == 5); // length < 1

    // Cleanup
    unstdstring_free(s1);
    unstdstring_free(empty);
    unstdstring_free(corrupt);
    unstdstring_free(invalid_len);

    _notify("[+]", "`unstdstring_endswith_char()` passed");
}

//! [endswith_char_ignorecase]
void test_unstdstring_endswith_char_ignorecase(void) {
    u8t result;

    // Test 1: Ends with expected char, matching case
    unstdstring *s1 = unstdstring_new("World", 16, &result);
    assert(s1 && result == 1);

    bool ends = unstdstring_endswith_char_ignorecase(s1, 'd', &result);
    assert(ends == true);
    assert(result == 1);

    // Test 2: Ends with expected char, different case
    ends = unstdstring_endswith_char_ignorecase(s1, 'D', &result);
    assert(ends == true);
    assert(result == 1);

    // Test 3: Does not end with the char
    ends = unstdstring_endswith_char_ignorecase(s1, 'W', &result);
    assert(ends == false);
    assert(result == 1);

    // Test 4: Empty string
    unstdstring *empty = unstdstring_new("", 8, &result);
    assert(empty && result == 1);

    ends = unstdstring_endswith_char_ignorecase(empty, 'x', &result);
    assert(ends == false);
    assert(result == 4); // empty string

    // Test 5: NULL buffer_arg
    ends = unstdstring_endswith_char_ignorecase(NULL, 'x', &result);
    assert(ends == false);
    assert(result == 2); // buffer_arg is NULL

    // Test 6: NULL data
    unstdstring *corrupt = unstdstring_new("Fail", 8, &result);
    assert(corrupt && result == 1);
    free(corrupt->data);
    corrupt->data = NULL;

    ends = unstdstring_endswith_char_ignorecase(corrupt, 'l', &result);
    assert(ends == false);
    assert(result == 3); // data is NULL

    // Test 7: length < 1, simulate manually
    unstdstring *zero_len = unstdstring_new("Short", 8, &result);
    assert(zero_len && result == 1);
    zero_len->length = 0; // simulate invalid state

    ends = unstdstring_endswith_char_ignorecase(zero_len, 't', &result);
    assert(ends == false);
    assert(result == 5); // length < 1

    // Cleanup
    unstdstring_free(s1);
    unstdstring_free(empty);
    unstdstring_free(corrupt);
    unstdstring_free(zero_len);

    _notify("[+]", "`unstdstring_endswith_char_ignorecase()` passed");
}

//! [startswith_str]
void test_unstdstring_startswith_str(void) {
    u8t result;

    // Test 1: Valid prefix (exact match at start)
    unstdstring *s1 = unstdstring_new("unstandard", 32, &result);
    unstdstring *prefix = unstdstring_new("unstand", 16, &result);
    assert(s1 && prefix && result == 1);

    bool res = unstdstring_startswith_str(s1, prefix, &result);
    assert(res == true);
    assert(result == 1);

    // Test 2: Not a prefix (valid strings, not matching)
    unstdstring *wrong = unstdstring_new("standard", 16, &result);
    assert(wrong && result == 1);

    res = unstdstring_startswith_str(s1, wrong, &result);
    assert(res == false);
    assert(result == 1);

    // Test 3: Prefix longer than main string
    unstdstring *longer = unstdstring_new("unstandardized", 32, &result);
    assert(longer && result == 1);

    res = unstdstring_startswith_str(s1, longer, &result);
    assert(res == false);
    assert(result == 5); // checkon_arg is longer

    // Test 4: One empty, one not
    unstdstring *empty = unstdstring_new("", 8, &result);
    assert(empty && result == 1);

    res = unstdstring_startswith_str(s1, empty, &result);
    assert(res == false);
    assert(result == 4); // one string empty

    res = unstdstring_startswith_str(empty, s1, &result);
    assert(res == false);
    assert(result == 4); // one string empty

    // Test 5: Both empty
    unstdstring *also_empty = unstdstring_new("", 8, &result);
    assert(also_empty && result == 1);

    res = unstdstring_startswith_str(empty, also_empty, &result);
    assert(res == true);
    assert(result == 1); // both empty is OK

    // Test 6: buffer_arg == NULL
    res = unstdstring_startswith_str(NULL, prefix, &result);
    assert(res == false);
    assert(result == 2);

    // Test 7: checkon_arg == NULL
    res = unstdstring_startswith_str(s1, NULL, &result);
    assert(res == false);
    assert(result == 2);

    // Test 8: data field == NULL
    unstdstring *corrupt = unstdstring_new("test", 8, &result);
    assert(corrupt && result == 1);
    free(corrupt->data);
    corrupt->data = NULL;

    res = unstdstring_startswith_str(corrupt, s1, &result);
    assert(res == false);
    assert(result == 3);

    res = unstdstring_startswith_str(s1, corrupt, &result);
    assert(res == false);
    assert(result == 3);

    // Cleanup
    unstdstring_free(s1);
    unstdstring_free(prefix);
    unstdstring_free(wrong);
    unstdstring_free(longer);
    unstdstring_free(empty);
    unstdstring_free(also_empty);
    unstdstring_free(corrupt);

    _notify("[+]", "`unstdstring_startswith_str()` passed");
}

void test_unstdstring_endswith_str(void) {
    u8t result;

    // Test 1: Valid suffix (exact match at end)
    unstdstring *s1 = unstdstring_new("unstandard", 32, &result);
    unstdstring *suffix = unstdstring_new("dard", 16, &result);
    assert(s1 && suffix && result == 1);

    bool res = unstdstring_endswith_str(s1, suffix, &result);
    assert(res == true);
    assert(result == 1);

    // Test 2: Not a suffix (valid strings, not matching)
    unstdstring *wrong = unstdstring_new("stand", 16, &result);
    assert(wrong && result == 1);

    res = unstdstring_endswith_str(s1, wrong, &result);
    assert(res == false);
    assert(result == 1);

    // Test 3: Suffix longer than main string
    unstdstring *longer = unstdstring_new("unstandardized", 32, &result);
    assert(longer && result == 1);

    res = unstdstring_endswith_str(s1, longer, &result);
    assert(res == false);
    assert(result == 5);

    // Test 4: One empty, one not
    unstdstring *empty = unstdstring_new("", 8, &result);
    assert(empty && result == 1);

    res = unstdstring_endswith_str(s1, empty, &result);
    assert(res == false);
    assert(result == 4);

    res = unstdstring_endswith_str(empty, s1, &result);
    assert(res == false);
    assert(result == 4);

    // Test 5: Both empty
    unstdstring *also_empty = unstdstring_new("", 8, &result);
    assert(also_empty && result == 1);

    res = unstdstring_endswith_str(empty, also_empty, &result);
    assert(res == true);
    assert(result == 1);

    // Test 6: buffer_arg == NULL
    res = unstdstring_endswith_str(NULL, suffix, &result);
    assert(res == false);
    assert(result == 2);

    // Test 7: checkon_arg == NULL
    res = unstdstring_endswith_str(s1, NULL, &result);
    assert(res == false);
    assert(result == 2);

    // Test 8: data field == NULL
    unstdstring *corrupt = unstdstring_new("test", 8, &result);
    assert(corrupt && result == 1);
    free(corrupt->data);
    corrupt->data = NULL;

    res = unstdstring_endswith_str(corrupt, s1, &result);
    assert(res == false);
    assert(result == 3);

    res = unstdstring_endswith_str(s1, corrupt, &result);
    assert(res == false);
    assert(result == 3);

    // Cleanup
    unstdstring_free(s1);
    unstdstring_free(suffix);
    unstdstring_free(wrong);
    unstdstring_free(longer);
    unstdstring_free(empty);
    unstdstring_free(also_empty);
    unstdstring_free(corrupt);

    _notify("[+]", "`unstdstring_endswith_str()` passed");
}

//! [tolower_str]
void test_unstdstring_tolower_str(void) {
    u8t err;

    // [1] Normal ASCII uppercase -> lowercase conversion
    unstdstring *s1 = unstdstring_new("HeLLo WORLD!", 50, &err);
    assert(s1 != NULL && err == 1);
    unstdstring_tolower_str(s1, &err);
    assert(err == 1);
    assert(strcmp(s1->data, "hello world!") == 0);
    free(s1->data);
    free(s1);

    // [2] Already lowercase string (should stay the same)
    unstdstring *s2 = unstdstring_new("already lowercase", 50, &err);
    assert(s2 != NULL && err == 1);
    unstdstring_tolower_str(s2, &err);
    assert(err == 1);
    assert(strcmp(s2->data, "already lowercase") == 0);
    free(s2->data);
    free(s2);

    // [3] Empty string (should return error 4)
    unstdstring *s3 = unstdstring_new("", 10, &err);
    assert(s3 != NULL && err == 1);
    unstdstring_tolower_str(s3, &err);
    assert(err == 4);
    free(s3->data);
    free(s3);

    // [4] buffer_arg is NULL (should return error 2)
    unstdstring_tolower_str(NULL, &err);
    assert(err == 2);

    // [5] buffer_arg->data is NULL (should return error 3)
    unstdstring s4 = {.data = NULL, .length = 5, .capacity = 10, .limit = 0};
    unstdstring_tolower_str(&s4, &err);
    assert(err == 3);

    // [6] NULL out_error_arg (should not crash, but cannot verify error)
    unstdstring *s5 = unstdstring_new("TESTING", 20, &err);
    assert(s5 != NULL && err == 1);
    unstdstring_tolower_str(s5, NULL);
    // Can't assert on error, but ensure string converted anyway:
    assert(strcmp(s5->data, "testing") == 0);
    free(s5->data);
    free(s5);

    _notify("[+]", "`unstdstring_tolower_str()` passed");
}

//! [tolower_str_copy]
void test_unstdstring_tolower_str_copy(void) {
    u8t err;

    // [1] Normal case: valid string with mixed case
    unstdstring *src1 = unstdstring_new("HeLLo WORLD!", 50, &err);
    assert(src1 != NULL && err == 1);
    unstdstring *copy1 = unstdstring_tolower_str_copy(src1, &err);
    assert(copy1 != NULL && err == 1);
    assert(strcmp(copy1->data, "hello world!") == 0);
    assert(copy1->limit == src1->limit);
    free(src1->data);
    free(src1);
    free(copy1->data);
    free(copy1);

    // [2] Already lowercase input (output should match input)
    unstdstring *src2 = unstdstring_new("already lowercase", 50, &err);
    assert(src2 != NULL && err == 1);
    unstdstring *copy2 = unstdstring_tolower_str_copy(src2, &err);
    assert(copy2 != NULL && err == 1);
    assert(strcmp(copy2->data, "already lowercase") == 0);
    free(src2->data);
    free(src2);
    free(copy2->data);
    free(copy2);

    // [3] Empty string input (should fail with error 4)
    unstdstring *src3 = unstdstring_new("", 10, &err);
    assert(src3 != NULL && err == 1);
    unstdstring *copy3 = unstdstring_tolower_str_copy(src3, &err);
    assert(copy3 == NULL);
    assert(err == 4);
    free(src3->data);
    free(src3);

    // [4] NULL buffer_arg input (should fail with error 2)
    unstdstring *copy4 = unstdstring_tolower_str_copy(NULL, &err);
    assert(copy4 == NULL);
    assert(err == 2);

    // [5] buffer_arg->data is NULL (should fail with error 3)
    unstdstring s5 = {.data = NULL, .length = 5, .capacity = 10, .limit = 0};
    unstdstring *copy5 = unstdstring_tolower_str_copy(&s5, &err);
    assert(copy5 == NULL);
    assert(err == 3);

    _notify("[+]", "`unstdstring_tolower_str_copy()` passed");
}

//! [toupper_str]
void test_unstdstring_toupper_str(void) {
    u8t err;

    // [1] Normal case: mixed case string converted to uppercase
    unstdstring *str1 = unstdstring_new("Hello, World!", 50, &err);
    assert(str1 != NULL && err == 1);
    unstdstring_toupper_str(str1, &err);
    assert(err == 1);
    assert(strcmp(str1->data, "HELLO, WORLD!") == 0);
    free(str1->data);
    free(str1);

    // [2] Already uppercase input (should remain unchanged)
    unstdstring *str2 = unstdstring_new("UPPERCASE", 50, &err);
    assert(str2 != NULL && err == 1);
    unstdstring_toupper_str(str2, &err);
    assert(err == 1);
    assert(strcmp(str2->data, "UPPERCASE") == 0);
    free(str2->data);
    free(str2);

    // [3] Empty string input (should fail with error 4)
    unstdstring *str3 = unstdstring_new("", 10, &err);
    assert(str3 != NULL && err == 1);
    unstdstring_toupper_str(str3, &err);
    assert(err == 4);
    free(str3->data);
    free(str3);

    // [4] NULL buffer_arg input (should fail with error 2)
    unstdstring_toupper_str(NULL, &err);
    assert(err == 2);

    // [5] buffer_arg->data is NULL (should fail with error 3)
    unstdstring s5 = {.data = NULL, .length = 3, .capacity = 10, .limit = 0};
    unstdstring_toupper_str(&s5, &err);
    assert(err == 3);

    _notify("[+]", "`unstdstring_toupper_str()` passed");
}

//! [toupper_str_copy]
void test_unstdstring_toupper_str_copy(void) {
    u8t err;

    // [1] Normal case: mixed case string copied and converted to uppercase
    unstdstring *orig1 = unstdstring_new("Hello, World!", 50, &err);
    assert(orig1 != NULL && err == 1);
    unstdstring *copy1 = unstdstring_toupper_str_copy(orig1, &err);
    assert(copy1 != NULL);
    assert(err == 1);
    assert(strcmp(copy1->data, "HELLO, WORLD!") == 0);
    // Original remains unchanged
    assert(strcmp(orig1->data, "Hello, World!") == 0);
    free(orig1->data);
    free(orig1);
    free(copy1->data);
    free(copy1);

    // [2] Already uppercase input (copy should be the same uppercase string)
    unstdstring *orig2 = unstdstring_new("UPPERCASE", 50, &err);
    assert(orig2 != NULL && err == 1);
    unstdstring *copy2 = unstdstring_toupper_str_copy(orig2, &err);
    assert(copy2 != NULL);
    assert(err == 1);
    assert(strcmp(copy2->data, "UPPERCASE") == 0);
    free(orig2->data);
    free(orig2);
    free(copy2->data);
    free(copy2);

    // [3] Empty string input (should fail with error 4 and return NULL)
    unstdstring *orig3 = unstdstring_new("", 10, &err);
    assert(orig3 != NULL && err == 1);
    unstdstring *copy3 = unstdstring_toupper_str_copy(orig3, &err);
    assert(copy3 == NULL);
    assert(err == 4);
    free(orig3->data);
    free(orig3);

    // [4] NULL buffer_arg input (should fail with error 2 and return NULL)
    unstdstring *copy4 = unstdstring_toupper_str_copy(NULL, &err);
    assert(copy4 == NULL);
    assert(err == 2);

    // [5] buffer_arg->data is NULL (should fail with error 3 and return NULL)
    unstdstring s5 = {.data = NULL, .length = 3, .capacity = 10, .limit = 0};
    unstdstring *copy5 = unstdstring_toupper_str_copy(&s5, &err);
    assert(copy5 == NULL);
    assert(err == 3);

    _notify("[+]", "`unstdstring_toupper_str_copy()` passed");
}

//! [isasciicontrolchar]
void test_unstdstring_isasciicontrolchar(void) {
    // [Succeeds]
    for (unsigned char ascii_buffer = 0x0; ascii_buffer <= 0x1F; ascii_buffer++) {
        assert(unstdstring_isasciicontrol_char(ascii_buffer));
    }
    assert(unstdstring_isasciicontrol_char(0x7F));

    _notify("[+]", "`unstdstring_isasciicontrol_char()` passed");
}

//! [isasciiprintablechar]
void test_unstdstring_isasciiprintablechar(void) {
    // [Succeeds]
    for (unsigned char ascii_buffer = 0x20; ascii_buffer <= 0x7E; ascii_buffer++) {
        assert(unstdstring_isasciiprintable_char(ascii_buffer));
    }

    _notify("[+]", "`unstdstring_isasciiprintable_char()` passed");
}

//! [isasciiextendedchar]
void test_unstdstring_isasciiextendedchar(void) {
    // [Succeeds]
    for (unsigned char ascii_buffer = 0x80; ascii_buffer >= 0x80 && ascii_buffer <= 0xFF; ascii_buffer++) {
        assert(unstdstring_isasciiextended_char(ascii_buffer));
    }

    _notify("[+]", "`unstdstring_isasciiextended_char()` passed");
}

//! [isasciivisiblechar]
void test_unstdstring_isasciivisiblechar(void) {
    // [Succeeds]
    for (unsigned char ascii_buffer = 0x20; ascii_buffer <= 0x7E; ascii_buffer++) {
        assert(unstdstring_isasciivisible_char(ascii_buffer));
    }

    for (unsigned char ascii_buffer = 0x80; ascii_buffer >= 0x80 && ascii_buffer <= 0xFF; ascii_buffer++) {
        assert(unstdstring_isasciivisible_char(ascii_buffer));
    }

    // [Fails]
    for (unsigned char ascii_buffer = 0x0; ascii_buffer <= 0x1F; ascii_buffer++) {
        assert(!unstdstring_isasciivisible_char(ascii_buffer));
    }

    _notify("[+]", "`unstdstring_isasciivisible_char()` passed");
}

//! [isasciichar]
void test_unstdstring_isasciichar(void) {
    // [Succeeds]
    for (unsigned char ascii_buffer = 0x00; ascii_buffer <= 0xFE; ++ascii_buffer) {
        assert(unstdstring_isascii_char(ascii_buffer));
    }

    _notify("[+]", "`unstdstring_isascii_char()` passed");
}

//! [isalphabeticchar]
void test_unstdstring_isalphabeticchar(void) {
    // [Succeeds]
    // [A-Z]
    for (unsigned char char_buffer = 'A'; char_buffer <= (unsigned char) 'Z'; char_buffer++) {
        assert(unstdstring_isalphabetic_char(char_buffer));
    }

    // [a-z]
    for (unsigned char char_buffer = 'a'; char_buffer <= (unsigned char) 'z'; char_buffer++) {
        assert(unstdstring_isalphabetic_char(char_buffer));
    }

    // [Fails]
    assert(!unstdstring_isalphabetic_char('0'));

    _notify("[+]", "`unstdstring_isalphabetic_char()` passed");
}

//! [isalphanumericchar]
void test_unstdstring_isalphanumericchar(void) {
    // [Succeeds]
    // [A-Z]
    for (unsigned char char_buffer = 'A'; char_buffer <= (unsigned char) 'Z'; char_buffer++) {
        assert(unstdstring_isalphanumeric_char(char_buffer));
    }

    // [a-z]
    for (unsigned char char_buffer = 'a'; char_buffer <= (unsigned char) 'z'; char_buffer++) {
        assert(unstdstring_isalphanumeric_char(char_buffer));
    }

    // [0-9]
    for (unsigned char char_buffer = '0'; char_buffer <= (unsigned char) '9'; char_buffer++) {
        assert(unstdstring_isalphanumeric_char(char_buffer));
    }

    // [Fails]
    assert(!unstdstring_isalphanumeric_char('-'));

    _notify("[+]", "`unstdstring_isalphanumeric_char()` passed");
}

//! [isdigitchar]
void test_unstdstring_isdigitchar(void) {
    // [Succeeds]
    // [0-9]
    for (unsigned char char_buffer = '0'; char_buffer <= (unsigned char) '9'; char_buffer++) {
        assert(unstdstring_isdigit_char(char_buffer));
    }

    // [Fails]
    assert(!unstdstring_isdigit_char('a'));
    assert(!unstdstring_isdigit_char(2));
    assert(!unstdstring_isdigit_char(0));
    assert(!unstdstring_isdigit_char(255));

    _notify("[+]", "`unstdstring_isdigit_char()` passed");
}

//! [ishexchar]
void test_unstdstring_ishexchar(void) {
    // [Succeeds]
    // [A-F]
    for (unsigned char char_buffer = 'A'; char_buffer <= (unsigned char) 'F'; char_buffer++) {
//        printf("%c\n", char_buffer);
        assert(unstdstring_ishex_char(char_buffer));
    }

    // [a-f]
    for (unsigned char char_buffer = 'a'; char_buffer <= (unsigned char) 'f'; char_buffer++) {
        assert(unstdstring_ishex_char(char_buffer));
    }

    // [0-9]
    for (unsigned char char_buffer = '0'; char_buffer <= (unsigned char) '9'; char_buffer++) {
        assert(unstdstring_ishex_char(char_buffer));
    }

    // [Fails]
    assert(!unstdstring_ishex_char('g'));
    assert(!unstdstring_ishex_char('h'));
    assert(!unstdstring_ishex_char(255));
    assert(!unstdstring_ishex_char(0));

    _notify("[+]", "`unstdstring_ishex_char()` passed");
}

//! [iswhitespace]
void test_unstdstring_iswhitespace(void) {
    // [Succeeds]
    assert(unstdstring_iswhitespace_char(' '));
    assert(unstdstring_iswhitespace_char('\t'));
    assert(unstdstring_iswhitespace_char('\r'));
    assert(unstdstring_iswhitespace_char('\n'));
    assert(unstdstring_iswhitespace_char(10));
    assert(unstdstring_iswhitespace_char(11));
    assert(unstdstring_iswhitespace_char(12));
    assert(unstdstring_iswhitespace_char(13));

    // [Fails]
    assert(!unstdstring_iswhitespace_char('\b'));
    assert(!unstdstring_iswhitespace_char('a'));
    assert(!unstdstring_iswhitespace_char('\2'));

    _notify("[+]", "`unstdstring_iswhitespace_char()` passed");
}

//! [pushback_char]
void test_unstdstring_pushback_char(void) {
    u8t err;

    // [1] Normal: append '!' to "Hello"
    unstdstring *s1 = unstdstring_new("Hello", 20, &err);
    assert(s1 != NULL && err == 1);
    err = unstdstring_pushback_char(s1, '!');
    assert(err == 1);
    assert(strcmp(s1->data, "Hello!") == 0);
    assert(s1->length == 6);
    assert(s1->capacity == 7);
    free(s1->data);
    free(s1);

    // [2] NULL buffer (to_buffer_arg) -> returns 2
    err = unstdstring_pushback_char(NULL, 'a');
    assert(err == 2);

    // [3] buffer->data is NULL -> returns 3
    unstdstring s3 = {.data = NULL, .length = 0, .capacity = 10};
    err = unstdstring_pushback_char(&s3, 'a');
    assert(err == 3);

    // [4] Insufficient capacity triggers _unstdstring_grow failure (simulate grow failure)
    unstdstring *s4 = unstdstring_new("Hi", 2, &err); // capacity too small for appended char + '\0'
    assert(s4 != NULL && err == 1);

    free(s4->data);
    free(s4);

    _notify("[+]", "`unstdstring_pushback_char()` passed");
}

//! [pushback_str]
void test_unstdstring_pushback_str(void) {
    u8t err;

    // [1] Normal case: append " World" to "Hello"
    unstdstring *s1 = unstdstring_new("Hello", 32, &err);
    assert(s1 && err == 1);
    err = unstdstring_pushback_str(s1, " World");
    assert(err == 1);
    assert(strcmp(s1->data, "Hello World") == 0);
    assert(s1->length == strlen("Hello World"));
    free(s1->data);
    free(s1);

    // [2] to_buffer_arg is NULL
    err = unstdstring_pushback_str(NULL, "test");
    assert(err == 2);

    // [3] to_buffer_arg->data is NULL
    unstdstring s3 = {.data = NULL, .length = 0, .capacity = 16};
    err = unstdstring_pushback_str(&s3, "abc");
    assert(err == 3);

    // [4] from_buffer_arg is NULL
    unstdstring *s4 = unstdstring_new("start", 16, &err);
    assert(s4 && err == 1);
    err = unstdstring_pushback_str(s4, NULL);
    assert(err == 4);
    free(s4->data);
    free(s4);

    // [5] from_buffer_arg is empty
    unstdstring *s5 = unstdstring_new("base", 16, &err);
    assert(s5 && err == 1);
    err = unstdstring_pushback_str(s5, "");
    assert(err == 5);
    free(s5->data);
    free(s5);

    _notify("[+]", "`unstdstring_pushback_str()` passed");
}

//! [popback_str]
void test_unstdstring_popback_char(void) {
    u8t err;

    // [1] Success: pop 'o' from "Hello"
    unstdstring *s1 = unstdstring_new("Hello", 16, &err);
    assert(s1 && err == 1);
    i8 c = unstdstring_popback_char(s1, &err);
    assert(err == 1);
    assert(c == 'o');
    assert(strcmp(s1->data, "Hell") == 0);
    assert(s1->length == strlen("Hell"));
    assert(s1->capacity == strlen("Hell") + 1);
    free(s1->data);
    free(s1);

    // [2] to_buffer_arg is NULL
    c = unstdstring_popback_char(NULL, &err);
    assert(err == 2);
    assert(c == 0);

    // [3] to_buffer_arg->data is NULL
    unstdstring s3 = { .data = NULL, .length = 0, .capacity = 16 };
    c = unstdstring_popback_char(&s3, &err);
    assert(err == 3);
    assert(c == 0);

    // [4] buffer is empty (data[0] == 0)
    unstdstring *s4 = unstdstring_new("", 16, &err);
    assert(s4 && err == 1);
    c = unstdstring_popback_char(s4, &err);
    assert(err == 4);
    assert(c == 0);
    free(s4->data);
    free(s4);

    _notify("[+]", "`unstdstring_popback_char()` passed");
}

//! [substr_copy]
void test_unstdstring_substr_copy(void) {
    u8t err;

    // [1] Success: copy "llo" from "Hello" (start = 2, span = 3)
    unstdstring *s1 = unstdstring_new("Hello", 0, &err);
    assert(s1 && err == 1);
    unstdstring *sub1 = unstdstring_substr_copy(s1, 2, 3, &err);
    assert(err == 1);
    assert(sub1 && strcmp(sub1->data, "llo") == 0);
    free(sub1->data);
    free(sub1);
    free(s1->data);
    free(s1);

    // [2] buffer_arg is NULL
    unstdstring *sub2 = unstdstring_substr_copy(NULL, 0, 1, &err);
    assert(err == 2);
    assert(sub2 == NULL);

    // [3] buffer_arg->data is NULL
    unstdstring s3 = { .data = NULL, .length = 0, .capacity = 0 };
    unstdstring *sub3 = unstdstring_substr_copy(&s3, 0, 1, &err);
    assert(err == 3);
    assert(sub3 == NULL);

    // [4] buffer_arg->data is empty
    unstdstring *s4 = unstdstring_new("", 0, &err);
    assert(s4 && err == 1);
    unstdstring *sub4 = unstdstring_substr_copy(s4, 0, 1, &err);
    assert(err == 4);
    assert(sub4 == NULL);
    free(s4->data);
    free(s4);

    // [5] start_arg out of range
    unstdstring *s5 = unstdstring_new("abc", 0, &err);
    assert(s5 && err == 1);
    unstdstring *sub5 = unstdstring_substr_copy(s5, 5, 1, &err);
    assert(err == 5);
    assert(sub5 == NULL);
    free(s5->data);
    free(s5);

    // [6] calloc failure simulated
    // Must simulate it manually if allocator override is available; otherwise, skip

    // [7] unstdstring_new failure simulated
    // Same applies: simulate failure by replacing unstdstring_new temporarily

    _notify("[+]", "`unstdstring_substr_copy()` passed");
}

void test_unstdstring(void) {
    //! [Constructors]
    test_unstdstring_new();

    //! [Setters]
    test_unstdstring_set();

    //! [Destructors]
    test_unstdstring_free();
    test_unstdstring_reset();
    test_unstdstring_clear();

    //! [comparison]
    test_unstdstring_equal();
    test_unstdstring_equal_overload_0();
    test_unstdstring_equal_ignorecase();
    test_unstdstring_equal_ignorecase_overload_0();

    //! [startswith]
    test_unstdstring_startswith_char();
    test_unstdstring_startswith_str();
    test_unstdstring_startswith_char_ignorecase();

    //! [endswith]
    test_unstdstring_endswith_char();
    test_unstdstring_endswith_str();
    test_unstdstring_endswith_char_ignorecase();

    //! [tolower]
    test_unstdstring_tolower_str();
    test_unstdstring_tolower_str_copy();

    //! [toupper]
    test_unstdstring_toupper_str();
    test_unstdstring_toupper_str_copy();

    //! [is]
    //! [isascii]
    test_unstdstring_isasciicontrolchar();
    test_unstdstring_isasciiprintablechar();
    test_unstdstring_isasciiextendedchar();
    test_unstdstring_isasciivisiblechar();
    test_unstdstring_isasciichar();

    test_unstdstring_isalphabeticchar();
    test_unstdstring_isalphanumericchar();
    test_unstdstring_isdigitchar();
    test_unstdstring_ishexchar();
    test_unstdstring_iswhitespace();

    //! [push]
    //! [char]
    test_unstdstring_pushback_char();

    //! [str]
    test_unstdstring_pushback_str();

    //! [pop]
    //! [char]
    test_unstdstring_popback_char();

    //! [substr]
    test_unstdstring_substr_copy();


    _notify("[+]", "`unstdstring` passed");
}
