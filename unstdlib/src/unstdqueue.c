#include <stdlib.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdbool.h>
#include "unstdqueue.h"

typedef struct unstdqueue_node {
    void *data;                       // Pointer to the data held by the node
    struct unstdqueue_node *next;     // Pointer to the next node in the queue
} unstdqueue_node;

unstdqueue_t *unstdqueue_init(u64t preallocate_size_arg, const u64t max_capacity_arg, u8t *const out_error_arg) {
    unstdqueue_t *queue = (unstdqueue_t *) malloc(sizeof(unstdqueue_t));
    if (!queue) {
        if (out_error_arg) {
            *out_error_arg = 2;
        }
        return NULL;
    }

    atomic_init(&queue->current_size, 0);
    queue->max_capacity = max_capacity_arg;
    queue->head = NULL;
    queue->tail = NULL;

    if (max_capacity_arg > 0 && preallocate_size_arg > max_capacity_arg) {
        preallocate_size_arg = max_capacity_arg;
    }

    unstdqueue_node *current_node = NULL;

    for (u64t i = 0; i < preallocate_size_arg; ++i) {
        unstdqueue_node *node = (unstdqueue_node *) malloc(sizeof(unstdqueue_node));
        if (!node) {
            if (out_error_arg) {
                *out_error_arg = 2;
            }
            unstdqueue_free(queue);
            return NULL;
        }
        node->data = NULL;
        node->next = NULL;

        if (current_node) {
            current_node->next = node;
        } else {
            queue->head = node;
        }
        current_node = node;
    }

    queue->tail = queue->head;

    if (out_error_arg) {
        *out_error_arg = 1;
    }

    return queue;
}

unstdqueue_node *_unstdqueue_allocate_node(void *const buffer_arg) {
    unstdqueue_node *node = (unstdqueue_node *) malloc(sizeof(unstdqueue_node));
    if (!node) {
        return NULL;
    }

    node->data = (void *) buffer_arg;
    node->next = NULL;

    return node;
}

u8t unstdqueue_enqueue(unstdqueue_t *const queue_arg, void *const buffer_arg) {
    if (!queue_arg) {
        return 2;
    }

    if (queue_arg->max_capacity > 0 && atomic_load(&queue_arg->current_size) >= queue_arg->max_capacity) {
        return 5;
    }

    //! Pre-allocate
    if (queue_arg->tail) {
        if (atomic_load(&queue_arg->current_size) <= 0) {
            queue_arg->tail->data = buffer_arg;
        } else {
            if (queue_arg->tail->next) {
                queue_arg->tail->data = buffer_arg;
            } else {
                unstdqueue_node *node = _unstdqueue_allocate_node(buffer_arg);
                if (!node) {
                    return 4;
                }
                queue_arg->tail->next = node;
            }
            queue_arg->tail = queue_arg->tail->next;
        }
    } else {
        unstdqueue_node *node = _unstdqueue_allocate_node(buffer_arg);
        if (!node) {
            return 4;
        }
        queue_arg->tail = node;
        if (!queue_arg->head) {
            queue_arg->head = node;
        }
    }

    //! Post-allocate
//    if (queue_arg->tail) {
//        queue_arg->tail->data = buffer_arg;
//        if (!queue_arg->tail->next) {
//            unstdqueue_node *node_empty = _unstdqueue_allocate_node(NULL);
//            if (!node_empty) {
//                return 4;
//            }
//            queue_arg->tail->next = node_empty;
//        }
//        queue_arg->tail = queue_arg->tail->next;
//    } else {
//        unstdqueue_node *node = _unstdqueue_allocate_node(buffer_arg);
//        if (!node) {
//            return 4;
//        }
//        queue_arg->tail = node;
//        if (!queue_arg->head) {
//            queue_arg->head = node;
//        }
//    }

    atomic_fetch_add(&queue_arg->current_size, 1);

    return 1;
}


void *unstdqueue_dequeue(unstdqueue_t *queue_arg, u8t *const out_error_arg) {
    if (!queue_arg) {
        if (out_error_arg) *out_error_arg = 2;
        return NULL;
    }

    if (atomic_load(&queue_arg->current_size) == 0) {
        if (out_error_arg) *out_error_arg = 3;
        return NULL;
    }

    unstdqueue_node *node = queue_arg->head;
    if (!node) {
        if (out_error_arg) *out_error_arg = 4; // Error: Queue is empty (should not occur)
        return NULL;
    }

    void *data = node->data;
    queue_arg->head = node->next;
    if (!queue_arg->head) {
        queue_arg->tail = NULL;
    }

    free(node);

    atomic_fetch_sub(&queue_arg->current_size, 1);

    if (out_error_arg) {
        *out_error_arg = 1;
    }

    return data;
}

u64t unstdqueue_size(unstdqueue_t *queue_arg, u8t *const out_error_arg) {
    if (!queue_arg) {
        if (out_error_arg) {
            *out_error_arg = 2;
        }
        return 0;
    }

    if (out_error_arg) {
        *out_error_arg = 1;
    }

    return atomic_load(&queue_arg->current_size);
}


u8t unstdqueue_isempty(unstdqueue_t *queue_arg) {
    if (!queue_arg) {
        return 2;
    }

    return atomic_load(&queue_arg->current_size) == 0;
}


u8t unstdqueue_isfull(const unstdqueue_t *const queue_arg) {
    if (!queue_arg) {
        return 2;
    }

    return queue_arg->max_capacity > 0
           && atomic_load(&queue_arg->current_size) >= queue_arg->max_capacity;
}


void *unstdqueue_peek(unstdqueue_t *queue_arg, u8t *const out_error_arg) {
    if (!queue_arg) {
        if (out_error_arg) {
            *out_error_arg = 2;
        }
        return NULL;
    }

    if (atomic_load(&queue_arg->current_size) == 0) {
        if (out_error_arg) {
            *out_error_arg = 3;
        }
        return NULL;
    }

    if (out_error_arg) *out_error_arg = 1;
    return queue_arg->head->data;
}


u8t unstdqueue_free(unstdqueue_t *const queue_arg) {
    if (!queue_arg) {
        return 2;
    }

    unstdqueue_node *current = queue_arg->head;
    while (current) {
        unstdqueue_node *next = current->next;
        free(current);
        current = next;
    }

    free(queue_arg);
    return 1;
}
