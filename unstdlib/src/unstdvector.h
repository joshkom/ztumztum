/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Evan Teran
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#ifndef UNSTDLIB_UNSTDVECTOR_H
#define UNSTDLIB_UNSTDVECTOR_H

#include <stdlib.h> /* for realloc */
#include <assert.h> /* for assert */
#include <string.h> /* for memmove */

/* NOTE: Similar to C's qsort and bsearch, you will receive a T*
 * for a vector of Ts. This means that you cannot use `free` directly
 * as a destructor. Instead, if you have for example a unstdvector_vector_typeM(int *)
 * you will need to supply a function which casts `elem_ptr` to an `int**`
 * and then does a free on what that pointer points to:
 *
 * ex:
 *
 * void free_int(void *p) { free(*(int **)p); }
 */
typedef void (*unstdvector_elem_destructor_t)(void *elem_ptr);

typedef struct unstdvector_metadata_t {
    size_t size;
    size_t capacity;
    unstdvector_elem_destructor_t elem_destructor;
} unstdvector_metadata_t;

/**
 * @brief unstdvector_vector_typeM - The vector type used in this library
 */
#define unstdvector_vector_typeM(type) type *

/**
 * @brief unstdvector - Syntactic sugar to retrieve a vector type
 *
 * @param type The type of vector to act on.
 */
#define unstdvectorM(type) unstdvector_vector_typeM(type)

/*
 * @brief unstdvector_iteratorM - The iterator type used for unstdvector
 */
#define unstdvector_iteratorM(type) unstdvector_vector_typeM(type)

/**
 * @brief unstdvector_vec_to_baseM - For internal use, converts a vector pointer to a metadata pointer
 * @param vec - the vector
 * @return the metadata pointer of the vector
 * @internal
 */
#define unstdvector_vec_to_baseM(vec) \
    (&((unstdvector_metadata_t *)(vec))[-1])

/**
 * @brief unstdvector_base_to_vecM - For internal use, converts a metadata pointer to a vector pointer
 * @param ptr - pointer to the metadata
 * @return the vector
 * @internal
 */
#define unstdvector_base_to_vecM(ptr) \
    ((void *)&((unstdvector_metadata_t *)(ptr))[1])

/**
 * @brief unstdvector_capacityM - gets the current allocated_len of the vector
 * @param vec - the vector
 * @return the allocated_len as a size_t
 */
#define unstdvector_capacityM(vec) \
    ((vec) ? unstdvector_vec_to_baseM(vec)->capacity : (size_t)0)

/**
 * @brief unstdvector_sizeM - gets the current size of the vector
 * @param vec - the vector
 * @return the size as a size_t
 */
#define unstdvector_sizeM(vec) \
    ((vec) ? unstdvector_vec_to_baseM(vec)->size : (size_t)0)

/**
 * @brief unstdvector_elem_destructorM - get the element destructor function used
 * to clean up elements
 * @param vec - the vector
 * @return the function pointer as unstdvector_elem_destructor_t
 */
#define unstdvector_elem_destructorM(vec) \
    ((vec) ? unstdvector_vec_to_baseM(vec)->elem_destructor : NULL)

/**
 * @brief unstdvector_emptyM - returns non-zero if the vector is empty
 * @param vec - the vector
 * @return non-zero if empty, zero if non-empty
 */
#define unstdvector_emptyM(vec) \
    (unstdvector_sizeM(vec) == 0)

/**
 * @brief unstdvector_reserveM - Requests that the vector allocated_len be at least enough
 * to contain n elements. If n is greater than the current vector allocated_len, the
 * function causes the container to reallocate its storage increasing its
 * allocated_len to n (or greater).
 * @param vec - the vector
 * @param n - Minimum allocated_len for the vector.
 * @return void
 */
#define unstdvector_reserveM(vec, n)                  \
    do {                                         \
        size_t cv_cap__ = unstdvector_capacityM(vec); \
        if (cv_cap__ < (n)) {                    \
            unstdvector_growM((vec), (n));            \
        }                                        \
    } while (0)

/*
 * @brief unstdvector_initM - Initialize a vector.  The vector must be NULL for this to do anything.
 * @param vec - the vector
 * @param allocated_len - vector allocated_len to reserve
 * @param elem_destructor_fn - element destructor function
 * @return void
 */
#define unstdvector_initM(vec, capacity, elem_destructor_fn)               \
    do {                                                              \
        if (!(vec)) {                                                 \
            unstdvector_reserveM((vec), capacity);                         \
            unstdvector_set_elem_destructorM((vec), (elem_destructor_fn)); \
        }                                                             \
    } while (0)

/**
 * @brief unstdvector_eraseM - removes the element at index i from the vector
 * @param vec - the vector
 * @param i - index of element to remove
 * @return void
 */
#define unstdvector_eraseM(vec, i)                                                               \
    do {                                                                                    \
        if (vec) {                                                                          \
            const size_t cv_sz__ = unstdvector_sizeM(vec);                                       \
            if ((i) < cv_sz__) {                                                            \
                unstdvector_elem_destructor_t elem_destructor__ = unstdvector_elem_destructorM(vec); \
                if (elem_destructor__) {                                                    \
                    elem_destructor__(&(vec)[i]);                                           \
                }                                                                           \
                unstdvector_set_sizeM((vec), cv_sz__ - 1);                                       \
                memmove(                                                       \
                    (vec) + (i),                                                            \
                    (vec) + (i) + 1,                                                        \
                    sizeof(*(vec)) * (cv_sz__ - 1 - (i)));                                  \
            }                                                                               \
        }                                                                                   \
    } while (0)

/**
 * @brief unstdvector_clearM - erase all of the elements in the vector
 * @param vec - the vector
 * @return void
 */
#define unstdvector_clearM(vec)                                                              \
    do {                                                                                \
        if (vec) {                                                                      \
            unstdvector_elem_destructor_t elem_destructor__ = unstdvector_elem_destructorM(vec); \
            if (elem_destructor__) {                                                    \
                size_t i__;                                                             \
                for (i__ = 0; i__ < unstdvector_sizeM(vec); ++i__) {                         \
                    elem_destructor__(&(vec)[i__]);                                     \
                }                                                                       \
            }                                                                           \
            unstdvector_set_sizeM(vec, 0);                                                   \
        }                                                                               \
    } while (0)

/**
 * @brief unstdvector_freeM - frees all memory associated with the vector
 * @param vec - the vector
 * @return void
 */
#define unstdvector_freeM(vec)                                                               \
    do {                                                                                \
        if (vec) {                                                                      \
            void *p1__                                  = unstdvector_vec_to_baseM(vec);     \
            unstdvector_elem_destructor_t elem_destructor__ = unstdvector_elem_destructorM(vec); \
            if (elem_destructor__) {                                                    \
                size_t i__;                                                             \
                for (i__ = 0; i__ < unstdvector_sizeM(vec); ++i__) {                         \
                    elem_destructor__(&(vec)[i__]);                                     \
                }                                                                       \
            }                                                                           \
            free(p1__);                                                    \
        }                                                                               \
    } while (0)

/**
 * @brief unstdvector_beginM - returns an iterator to first element of the vector
 * @param vec - the vector
 * @return a pointer to the first element (or NULL)
 */
#define unstdvector_beginM(vec) \
    (vec)

/**
 * @brief unstdvector_endM - returns an iterator to one past the last element of the vector
 * @param vec - the vector
 * @return a pointer to one past the last element (or NULL)
 */
#define unstdvector_endM(vec) \
    ((vec) ? &((vec)[unstdvector_sizeM(vec)]) : NULL)

/**
 * @brief unstdvector_compute_next_grow_logarithmicM - returns an the computed size in next vector grow
 * size is increased by multiplication of 2
 * @param size - current size
 * @return size after next vector grow
 */
#define unstdvector_compute_next_grow_logarithmicM(size) \
    ((size) ? ((size) << 1) : 1)


/**
 * @brief unstdvector_compute_next_growM - returns an the computed size in next vector grow
 * size is increased by 1
 * @param size - current size
 * @return size after next vector grow
 */
#define unstdvector_compute_next_growM(size) \
    ((size) + 1)

/**
 * @brief unstdvector_push_backM - adds an element to the end of the vector
 * @param vec - the vector
 * @param value - the value to add
 * @return void
 */
#define unstdvector_push_backM(vec, value)                                 \
    do {                                                              \
        size_t cv_cap__ = unstdvector_capacityM(vec);                      \
        if (cv_cap__ <= unstdvector_sizeM(vec)) {                          \
            unstdvector_growM((vec), unstdvector_compute_next_growM(cv_cap__)); \
        }                                                             \
        (vec)[unstdvector_sizeM(vec)] = (value);                           \
        unstdvector_set_sizeM((vec), unstdvector_sizeM(vec) + 1);               \
    } while (0)

/**
 * @brief unstdvector_insertM - insert element at position pos to the vector
 * @param vec - the vector
 * @param pos - position in the vector where the new elements are inserted.
 * @param val - value to be copied (or moved) to the inserted elements.
 * @return void
 */
#define unstdvector_insertM(vec, pos, val)                                 \
    do {                                                              \
        size_t cv_cap__ = unstdvector_capacityM(vec);                      \
        if (cv_cap__ <= unstdvector_sizeM(vec)) {                          \
            unstdvector_growM((vec), unstdvector_compute_next_growM(cv_cap__)); \
        }                                                             \
        if ((pos) < unstdvector_sizeM(vec)) {                              \
            memmove(                                     \
                (vec) + (pos) + 1,                                    \
                (vec) + (pos),                                        \
                sizeof(*(vec)) * ((unstdvector_sizeM(vec)) - (pos)));      \
        }                                                             \
        (vec)[(pos)] = (val);                                         \
        unstdvector_set_sizeM((vec), unstdvector_sizeM(vec) + 1);               \
    } while (0)

/**
 * @brief unstdvector_pushback_logarithmicM - adds an element to the end of the vector
 * @param vec - the vector
 * @param value - the value to add
 * @return void
 */
#define unstdvector_pushback_logarithmicM(vec, value)                                 \
    do {                                                              \
        size_t cv_cap__ = unstdvector_capacityM(vec);                      \
        if (cv_cap__ <= unstdvector_sizeM(vec)) {                          \
            unstdvector_growM((vec), unstdvector_compute_next_grow_logarithmicM(cv_cap__)); \
        }                                                             \
        (vec)[unstdvector_sizeM(vec)] = (value);                           \
        unstdvector_set_sizeM((vec), unstdvector_sizeM(vec) + 1);               \
    } while (0)

/**
 * @brief unstdvector_insert_logarithmicM - insert element at position pos to the vector
 * @param vec - the vector
 * @param pos - position in the vector where the new elements are inserted.
 * @param val - value to be copied (or moved) to the inserted elements.
 * @return void
 */
#define unstdvector_insert_logarithmicM(vec, pos, val)                                 \
    do {                                                              \
        size_t cv_cap__ = unstdvector_capacityM(vec);                      \
        if (cv_cap__ <= unstdvector_sizeM(vec)) {                          \
            unstdvector_growM((vec), unstdvector_compute_next_grow_logarithmicM(cv_cap__)); \
        }                                                             \
        if ((pos) < unstdvector_sizeM(vec)) {                              \
            memmove(                                     \
                (vec) + (pos) + 1,                                    \
                (vec) + (pos),                                        \
                sizeof(*(vec)) * ((unstdvector_sizeM(vec)) - (pos)));      \
        }                                                             \
        (vec)[(pos)] = (val);                                         \
        unstdvector_set_sizeM((vec), unstdvector_sizeM(vec) + 1);               \
    } while (0)

/**
 * @brief unstdvector_popbackM - removes the last element from the vector
 * @param vec - the vector
 * @return void
 */
#define unstdvector_popbackM(vec)                                                       \
    do {                                                                            \
        unstdvector_elem_destructor_t elem_destructor__ = unstdvector_elem_destructorM(vec); \
        if (elem_destructor__) {                                                    \
            elem_destructor__(&(vec)[unstdvector_sizeM(vec) - 1]);                       \
        }                                                                           \
        unstdvector_set_sizeM((vec), unstdvector_sizeM(vec) - 1);                             \
    } while (0)

/**
 * @brief unstdvector_copyM - copy a vector
 * @param from - the original vector
 * @param to - destination to which the function copy to
 * @return void
 */
#define unstdvector_copyM(from, to)                                                       \
    do {                                                                             \
        if ((from)) {                                                                \
            unstdvector_growM(to, unstdvector_sizeM(from));                                    \
            unstdvector_set_sizeM(to, unstdvector_sizeM(from));                                \
            memcpy((to), (from), unstdvector_sizeM(from) * sizeof(*(from))); \
        }                                                                            \
    } while (0)

/**
 * @brief unstdvector_swapM - exchanges the content of the vector by the content of another vector of the same type
 * @param vec - the original vector
 * @param other - the other vector to swap content with
 * @param type - the type of both vectors
 * @return void
 */
#define unstdvector_swapM(vec, other, type)                       \
    do {                                                     \
        if (vec && other) {                                  \
            unstdvector_vector_typeM(type) cv_swap__ = vec;       \
            vec                                 = other;     \
            other                               = cv_swap__; \
        }                                                    \
    } while (0)

/**
 * @brief unstdvector_set_capacityM - For internal use, sets the allocated_len variable of the vector
 * @param vec - the vector
 * @param size - the new allocated_len to set
 * @return void
 * @internal
 */
#define unstdvector_set_capacityM(vec, size)                  \
    do {                                                 \
        if (vec) {                                       \
            unstdvector_vec_to_baseM(vec)->capacity = (size); \
        }                                                \
    } while (0)

/**
 * @brief unstdvector_set_sizeM - For internal use, sets the size variable of the vector
 * @param vec - the vector
 * @param size - the new allocated_len to set
 * @return void
 * @internal
 */
#define unstdvector_set_sizeM(vec, _size)                  \
    do {                                              \
        if (vec) {                                    \
            unstdvector_vec_to_baseM(vec)->size = (_size); \
        }                                             \
    } while (0)

/**
 * @brief unstdvector_set_elem_destructorM - set the element destructor function
 * used to clean up removed elements. The vector must NOT be NULL for this to do anything.
 * @param vec - the vector
 * @param elem_destructor_fn - function pointer of type unstdvector_elem_destructor_t used to destroy elements
 * @return void
 */
#define unstdvector_set_elem_destructorM(vec, elem_destructor_fn)                  \
    do {                                                                      \
        if (vec) {                                                            \
            unstdvector_vec_to_baseM(vec)->elem_destructor = (elem_destructor_fn); \
        }                                                                     \
    } while (0)

/**
 * @brief unstdvector_growM - For internal use, ensures that the vector is at least <count> elements big
 * @param vec - the vector
 * @param count - the new allocated_len to set
 * @return void
 * @internal
 */
#define unstdvector_growM(vec, count)                                                      \
    do {                                                                              \
        const size_t cv_sz__ = (count) * sizeof(*(vec)) + sizeof(unstdvector_metadata_t); \
        if (vec) {                                                                    \
            void *cv_p1__ = unstdvector_vec_to_baseM(vec);                                 \
            void *cv_p2__ = realloc(cv_p1__, cv_sz__);                   \
            assert(cv_p2__);                                             \
            (vec) = unstdvector_base_to_vecM(cv_p2__);                                     \
        } else {                                                                      \
            void *cv_p__ = malloc(cv_sz__);                              \
            assert(cv_p__);                                              \
            (vec) = unstdvector_base_to_vecM(cv_p__);                                      \
            unstdvector_set_sizeM((vec), 0);                                               \
            unstdvector_set_elem_destructorM((vec), NULL);                                 \
        }                                                                             \
        unstdvector_set_capacityM((vec), (count));                                         \
    } while (0)

/**
 * @brief unstdvector_shrink_to_fitM - requests the container to reduce its allocated_len to fit its size
 * @param vec - the vector
 * @return void
 */
#define unstdvector_shrink_to_fitM(vec)                     \
    do {                                               \
        if (vec) {                                     \
            const size_t cv_sz___ = unstdvector_sizeM(vec); \
            unstdvector_growM(vec, cv_sz___);               \
        }                                              \
    } while (0)

/**
 * @brief unstdvector_atM - returns a reference to the element at position n in the vector.
 * @param vec - the vector
 * @param n - position of an element in the vector.
 * @return the element at the specified position in the vector.
 */
#define unstdvector_atM(vec, n) \
    ((vec) ? (((int)(n) < 0 || (size_t)(n) >= unstdvector_sizeM(vec)) ? NULL : &(vec)[n]) : NULL)

/**
 * @brief unstdvector_frontM - returns a reference to the first element in the vector. Unlike member unstdvector_beginM, which returns an iterator to this same element, this function returns a direct reference.
 * @return a reference to the first element in the vector container.
 */
#define unstdvector_frontM(vec) \
    ((vec) ? ((unstdvector_sizeM(vec) > 0) ? unstdvector_atM(vec, 0) : NULL) : NULL)

/**
 * @brief unstdvector_backM - returns a reference to the last element in the vector.Unlike member unstdvector_endM, which returns an iterator just past this element, this function returns a direct reference.
 * @return a reference to the last element in the vector.
 */
#define unstdvector_backM(vec) \
    ((vec) ? ((unstdvector_sizeM(vec) > 0) ? unstdvector_atM(vec, unstdvector_sizeM(vec) - 1) : NULL) : NULL)

/**
 * @brief unstdvector_resizeM - resizes the container to contain count elements.
 * @param vec - the vector
 * @param count - new size of the vector
 * @param value - the value to initialize new elements with
 * @return void
 */
#define unstdvector_resizeM(vec, count, value)                          \
    do {                                                           \
        if (vec) {                                                 \
            size_t cv_sz_count__ = (size_t)(count);                \
            size_t cv_sz__       = unstdvector_vec_to_baseM(vec)->size; \
            if (cv_sz_count__ > cv_sz__) {                         \
                unstdvector_reserveM((vec), cv_sz_count__);             \
                unstdvector_set_sizeM((vec), cv_sz_count__);            \
                do {                                               \
                    (vec)[cv_sz__++] = (value);                    \
                } while (cv_sz__ < cv_sz_count__);                 \
            } else {                                               \
                while (cv_sz_count__ < cv_sz__--) {                \
                    unstdvector_popbackM(vec);                         \
                }                                                  \
            }                                                      \
        }                                                          \
    } while (0)


/**
 * @brief For header to iterate over vector each element's address.
 * @param iterator_arg - iterator of type pointer to vector element.
 * @param vector_arg The vector.
 * @returns void.
 */
#define unstdvector_for_each_inM(iterator_arg, vector_arg) \
    for (iterator_arg = unstdvector_beginM(vector_arg); iterator_arg < unstdvector_endM(vector_arg); iterator_arg++)

/**
 * @brief Calls function resolved_ptr on each element of the vector.
 * @param vector_arg The vector.
 * @param func_arg Function to be called on each element that takes each element as argument.
 * @returns void.
 */
#define unstdvector_for_eachM(vector_arg, func_arg)               \
    do {                                                          \
        if ((vector_arg) && (func_arg) != NULL) {                 \
            sizet i;                                             \
            for (i = 0; i < unstdvector_sizeM(vector_arg); i++) { \
                func_arg((vector_arg)[i]);                        \
            }                                                     \
        }                                                         \
    } while (0)

/**
 * @brief Calls `free_func` on each element.
 * @details Contained in the vector and then destroys the vector itself.
 * @param vector_arg The vector.
 * @param free_func Function used to free each element in the vector with.
 *                  one parameter which is the element to be freed.
 * @returns void.
 */
#define unstdvector_free_each_and_freeM(vector_arg, free_func_arg) \
    do {                                                           \
        unstdvector_for_eachM((vector_arg), (free_func_arg));      \
        unstdvector_freeM(vector_arg);                             \
    } while (0)

#endif /* UNSTDLIB_UNSTDVECTOR_H */
