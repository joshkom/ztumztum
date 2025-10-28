#include "../../_test_common.h"

// Headers
#include "../../../src/unstdinttypes.h"
#include "../../../src/unstdvector.h"


void test_unstdvector(void) {
    unstdvector_vector_typeM(s32t)v = NULL;
    unstdvector_vector_typeM(s32t)a = NULL;
    unstdvector_vector_typeM(s32t)b = NULL;
    unstdvector_vector_typeM(s32t)c = NULL;
    unstdvector_vector_typeM(char *)str_vect = NULL;

    /* add some elements to the back */
    unstdvector_pushback_logarithmicM(v, 10);
    unstdvector_pushback_logarithmicM(v, 20);
    unstdvector_pushback_logarithmicM(v, 30);

    /* and remove one too */
    unstdvector_popbackM(v);

    assert(unstdvector_capacityM(v) == 4);
    assert(unstdvector_sizeM(v) == 2);

    /* iterator over the vector using "iterator" style */
    if (v) {
        unstdvector_iteratorM(s32t)it;
        s32t i = 0;
        for (it = unstdvector_beginM(v); it != unstdvector_endM(v); ++it) {
            switch (i) {
                case 0:
                    assert(*it == 10);
                    break;
                case 1:
                    assert(*it == 20);
                default:
                    break;
            }
            ++i;
        }
    }

    /* iterator over the vector standard indexing too! */
    if (v) {
        sizet i;
        for (i = 0; i < unstdvector_sizeM(v); ++i) {
            switch (i) {
                case 0:
                    assert(v[i] == 10);
                    break;
                case 1:
                    assert(v[i] == 20);
                default:
                    break;
            }
        }
    }

    /* well, we don't have destructors, so let's clean things up */
    unstdvector_freeM(v);

    unstdvector_pushback_logarithmicM(a, 1);
    unstdvector_pushback_logarithmicM(a, 5);
    unstdvector_pushback_logarithmicM(a, 4);
    unstdvector_popbackM(a); // delete 4
    unstdvector_pushback_logarithmicM(a, 5);
    unstdvector_eraseM(a, 1); // delete 5
    unstdvector_eraseM(a, 0);
    unstdvector_insert_logarithmicM(a, 0, 1);

    assert(unstdvector_capacityM(a) == 4);
    assert(unstdvector_sizeM(a) == 2);

    if (a) {
        sizet i;
        unstdvector_copyM(a, b);
        assert(unstdvector_sizeM(a) == unstdvector_sizeM(b));
        for (i = 0; i < unstdvector_sizeM(b); ++i) {
            assert(a[i] == b[i]);
        }
    }

    unstdvector_freeM(a);

    assert(unstdvector_capacityM(b) == 2);
    assert(unstdvector_sizeM(b) == 2);

    if (b) {
        sizet i;
        unstdvector_insert_logarithmicM(b, 0, 0);
        unstdvector_insert_logarithmicM(b, 2, 4);
        unstdvector_insert_logarithmicM(b, 2, 2);
        unstdvector_insert_logarithmicM(b, 3, 3);
        assert(unstdvector_capacityM(b) == 8);
        assert(unstdvector_sizeM(b) == 6);
        // expected vector: [0, 1, 2, 3, 4, 5]
        for (i = 0; i < unstdvector_sizeM(b); ++i) {
            assert(b[i] == (s32t) i);
        }
    }

    unstdvector_freeM(b);

    unstdvector_reserveM(c, 100);
    assert(unstdvector_capacityM(c) == 100);
    assert(unstdvector_sizeM(c) == 0);
    unstdvector_pushback_logarithmicM(c, 10);
    assert(unstdvector_capacityM(c) == 100);
    assert(unstdvector_sizeM(c) == 1);
    unstdvector_reserveM(c, 10);
    assert(unstdvector_capacityM(c) == 100);

    {
        s32t i;
        for (i = 0; i < 100; ++i) {
            unstdvector_pushback_logarithmicM(c, i);
        }
    }
    assert(unstdvector_capacityM(c) == 200);
    unstdvector_freeM(c);

    unstdvector_pushback_logarithmicM(str_vect, strdup("Hello world"));
    unstdvector_pushback_logarithmicM(str_vect, strdup("Good  bye world"));
    unstdvector_pushback_logarithmicM(str_vect, strdup("not today"));

    unstdvector_free_each_and_freeM(str_vect, free);

    _notify("[+]", "`unstdvector` passed");
}
