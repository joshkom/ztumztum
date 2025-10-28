#include "../../_test_common.h"

#include "../../../src/unstdqueue.h"

//! [init]
void test_unstdqueue_init(void) {
    unstdqueue_t *queue = NULL;
    u8t error_code;

    // [Succeeds]
    queue = unstdqueue_init(10, 100, &error_code);
    assert(queue != NULL);
    assert(error_code == 1);
    unstdqueue_free(queue);

    queue = unstdqueue_init(0, -1, &error_code);
    assert(queue != NULL);
    assert(error_code == 1);
    unstdqueue_free(queue);

    // unmatched allocated_len and pre-allocation
    queue = unstdqueue_init(100, 10, &error_code);
    assert(queue != NULL);
    assert(error_code == 1);
    assert(queue->max_capacity == 10);
    assert(queue->current_size == 0);
    unstdqueue_free(queue);


    // Memory allocation failure (simulated)
    // Assuming pre-allocation size is very large
//    assert(unstdqueue_init(-1, -1, &error_code) == NULL); fork-bombed my own system, yet memset continued allocating without any failures..
//    assert(error_code == 2);
//    unstdqueue_free(queue);

    _notify("[+]", "`unstdqueue_init()` passed");
}

//! [enqueue]
void test_unstdqueue_enqueue(void) {
    unstdqueue_t *queue = NULL;
    u8t error_code;

    // [Succeeds]
    queue = unstdqueue_init(1, 7, &error_code);
    assert(queue != NULL);

    assert(unstdqueue_enqueue(queue, (void *) (uintptr_t) 0) == 1);
    assert(unstdqueue_enqueue(queue, (void *) (uintptr_t) 1) == 1);
    assert(unstdqueue_size(queue, NULL) == 2);
    assert(!unstdqueue_isfull(queue));

    // [Fails]
    assert(unstdqueue_enqueue(NULL, (void *) 1) == 2);
    assert(unstdqueue_enqueue(queue, NULL) == 1);

    // Enqueue when full
    assert(unstdqueue_enqueue(queue, (void *) 2) == 1);
    assert(unstdqueue_enqueue(queue, (void *) 3) == 1);
    assert(unstdqueue_enqueue(queue, (void *) 4) == 1);
    assert(unstdqueue_enqueue(queue, (void *) 5) == 1);
    assert(unstdqueue_enqueue(queue, (void *) 6) == 5);

    unstdqueue_free(queue);

    // [Succeeds]
    queue = unstdqueue_init(5, 10, &error_code);
    assert(queue != NULL);

    assert(unstdqueue_enqueue(queue, (void *) (uintptr_t) 0) == 1);
    assert(unstdqueue_enqueue(queue, (void *) (uintptr_t) 1) == 1);
    assert(unstdqueue_size(queue, NULL) == 2);
    assert(!unstdqueue_isfull(queue));

    // [Fails]
    assert(unstdqueue_enqueue(NULL, (void *) 1) == 2);
    assert(unstdqueue_enqueue(queue, NULL) == 1);

    // Enqueue when full
    unstdqueue_enqueue(queue, (void *) 2);
    unstdqueue_enqueue(queue, (void *) 3);
    unstdqueue_enqueue(queue, (void *) 4);
    unstdqueue_enqueue(queue, (void *) 5);
    assert(unstdqueue_enqueue(queue, (void *) 6) == 1);

    unstdqueue_free(queue);
    _notify("[+]", "`unstdqueue_enqueue()` passed");
}

//! [dequeue]
void test_unstdqueue_dequeue(void) {
    unstdqueue_t *queue = NULL;
    u8t error_code;

    // [Succeeds]
    queue = unstdqueue_init(1, 10, &error_code);
    assert(queue != NULL);

    unstdqueue_enqueue(queue, (void *) 1);
    unstdqueue_enqueue(queue, (void *) 2);

    void *data = unstdqueue_dequeue(queue, &error_code);
    assert(data == (void *) 1);
    assert(error_code == 1);

    data = unstdqueue_dequeue(queue, &error_code);
    assert(data == (void *) 2);
    assert(error_code == 1);

    // [Fails]
    assert(unstdqueue_dequeue(NULL, &error_code) == NULL);
    assert(error_code == 2);
    assert(unstdqueue_dequeue(queue, &error_code) == NULL);
    assert(error_code == 3);

    // Test with negative integer
    int neg_int = -123;
    assert(unstdqueue_enqueue(queue, (void *) (uintptr_t) neg_int) == 1);
    int dequeued_neg_int = (int) (uintptr_t) unstdqueue_dequeue(queue, NULL);
    assert(dequeued_neg_int == neg_int);

    // Test with positive float
    float pos_float = 3.14f;
    assert(unstdqueue_enqueue(queue, (void *) &pos_float) == 1);
    float *dequeued_pos_float = (float *) unstdqueue_dequeue(queue, NULL);
    assert(*dequeued_pos_float == pos_float);

    // Test with negative float
    float neg_float = -2.71f;
    assert(unstdqueue_enqueue(queue, (void *) &neg_float) == 1);
    float neg_float_2 = -99.91f;
    assert(unstdqueue_enqueue(queue, (void *) &neg_float_2) == 1);

    float *dequeued_neg_float = (float *) unstdqueue_dequeue(queue, NULL);
    assert(*dequeued_neg_float == neg_float);

    float *dequeued_neg_float_2 = (float *) unstdqueue_dequeue(queue, NULL);
    assert(*dequeued_neg_float_2 == neg_float_2);

    unstdqueue_free(queue);
    _notify("[+]", "`unstdqueue_dequeue()` passed");
}

//! [size]
void test_unstdqueue_size(void) {
    unstdqueue_t *queue = NULL;
    u8t error_code;

    // [Succeeds]
    queue = unstdqueue_init(10, 5, &error_code);
    assert(queue != NULL);
    assert(unstdqueue_size(queue, &error_code) == 0);
    assert(error_code == 1);

    unstdqueue_enqueue(queue, (void *) 1);
    assert(unstdqueue_size(queue, &error_code) == 1);
    assert(error_code == 1);

    // [Fails]
    assert(unstdqueue_size(NULL, &error_code) == 0);
    assert(error_code == 2);

    unstdqueue_free(queue);
    _notify("[+]", "`unstdqueue_size()` passed");
}

//! [isempty]
void test_unstdqueue_isempty(void) {
    unstdqueue_t *queue = NULL;

    // [Succeeds]
    queue = unstdqueue_init(10, 5, NULL);
    assert(queue != NULL);
    assert(unstdqueue_isempty(queue) == 1);

    unstdqueue_enqueue(queue, (void *) 1);
    assert(unstdqueue_isempty(queue) == 0);

    // [Fails]
    assert(unstdqueue_isempty(NULL) == 2);

    unstdqueue_free(queue);
    _notify("[+]", "`unstdqueue_isempty()` passed");
}

//! [isfull]
void test_unstdqueue_isfull(void) {
    unstdqueue_t *queue = NULL;

    // [Succeeds]
    queue = unstdqueue_init(10, 3, NULL);
    assert(queue != NULL);

    assert(unstdqueue_isfull(queue) == 0);
    unstdqueue_enqueue(queue, (void *) 1);
    unstdqueue_enqueue(queue, (void *) 2);
    unstdqueue_enqueue(queue, (void *) 3);
    assert(unstdqueue_isfull(queue) == 1);

    // [Fails]
    assert(unstdqueue_isfull(NULL) == 2);

    unstdqueue_free(queue);
    _notify("[+]", "`unstdqueue_isfull()` passed");
}

//! [peek]
void test_unstdqueue_peek(void) {
    unstdqueue_t *queue = NULL;
    u8t error_code;

    // [Succeeds]
    queue = unstdqueue_init(10, 5, &error_code);
    assert(queue != NULL);

    unstdqueue_enqueue(queue, (void *) 1);
    void *data = unstdqueue_peek(queue, &error_code);
    assert(data == (void *) 1);
    assert(error_code == 1);

    // [Fails]
    assert(unstdqueue_peek(NULL, &error_code) == NULL);
    assert(error_code == 2);
    unstdqueue_dequeue(queue, &error_code); // Empty the queue
    assert(unstdqueue_peek(queue, &error_code) == NULL);
    assert(error_code == 3);

    unstdqueue_free(queue);
    _notify("[+]", "`unstdqueue_peek()` passed");
}

//! [free]
void test_unstdqueue_free(void) {
    unstdqueue_t *queue = NULL;

    // [Succeeds]
    queue = unstdqueue_init(10, 5, NULL);
    assert(queue != NULL);
    assert(unstdqueue_free(queue) == 1);

    // [Fails]
    assert(unstdqueue_free(NULL) == 2);

    _notify("[+]", "`unstdqueue_free()` passed");
}

void test_unstdqueue_with_various_data_types(void) {
    unstdqueue_t *queue = unstdqueue_init(0, 10, NULL);
    assert(queue != NULL);

    // Test with a negative integer
    int neg_int = -456;
    assert(unstdqueue_enqueue(queue, (void *) (uintptr_t) neg_int) == 1);
    int dequeued_neg_int = (int) (uintptr_t) unstdqueue_dequeue(queue, NULL);
    assert(dequeued_neg_int == neg_int);

    // Test with a pointer to an integer
    int *int_ptr = (int *) malloc(sizeof(int));
    *int_ptr = 1234;
    assert(unstdqueue_enqueue(queue, int_ptr) == 1);
    int *dequeued_int_ptr = (int *) unstdqueue_dequeue(queue, NULL);
    assert(*dequeued_int_ptr == 1234);
    free(dequeued_int_ptr);

    // Test with a floating-point number
    double pos_double = 9.81;
    assert(unstdqueue_enqueue(queue, (void *) &pos_double) == 1);
    double *dequeued_pos_double = (double *) unstdqueue_dequeue(queue, NULL);
    assert(*dequeued_pos_double == pos_double);

    // Test with a struct
    // Define a struct for testing
    typedef struct {
        int id;
        char name[50];
    } Person;

    Person p1 = {1, "John Doe"};
    assert(unstdqueue_enqueue(queue, &p1) == 1);
    Person *dequeued_person = (Person *) unstdqueue_dequeue(queue, NULL);
    assert(dequeued_person->id == p1.id);
    assert(strcmp(dequeued_person->name, p1.name) == 0);

    // Test with a pointer to a struct
    Person *p2 = (Person *) malloc(sizeof(Person));
    p2->id = 2;
    strcpy(p2->name, "Jane Doe");
    assert(unstdqueue_enqueue(queue, p2) == 1);
    Person *dequeued_person_ptr = (Person *) unstdqueue_dequeue(queue, NULL);
    assert(dequeued_person_ptr->id == p2->id);
    assert(strcmp(dequeued_person_ptr->name, p2->name) == 0);
    free(dequeued_person_ptr);

    // Test with a dynamically allocated array
    int *dynamic_array = (int *) malloc(3 * sizeof(int));
    dynamic_array[0] = 1;
    dynamic_array[1] = 2;
    dynamic_array[2] = 3;
    assert(unstdqueue_enqueue(queue, dynamic_array) == 1);
    int *dequeued_array = (int *) unstdqueue_dequeue(queue, NULL);
    assert(dequeued_array[0] == 1);
    assert(dequeued_array[1] == 2);
    assert(dequeued_array[2] == 3);
    free(dequeued_array);

    // Test with a string literal
    char *string_literal = "Hello, Queue!";
    assert(unstdqueue_enqueue(queue, string_literal) == 1);
    char *dequeued_string = (char *) unstdqueue_dequeue(queue, NULL);
    assert(strcmp(dequeued_string, string_literal) == 0);

    // Clean up
    unstdqueue_free(queue);

    _notify("[+]", "`unstdqueue_enqueue()` && ``unstdqueue_dequeue()` tests with various data types passed");
}

void test_unstdqueue(void) {
    test_unstdqueue_init();
    test_unstdqueue_enqueue();
    test_unstdqueue_dequeue();
    test_unstdqueue_size();
    test_unstdqueue_isempty();
    test_unstdqueue_isfull();
    test_unstdqueue_peek();
    test_unstdqueue_free();

    test_unstdqueue_with_various_data_types();

    _notify("[+]", "`unstdqueue` passed");
}
