#ifndef UNSTDLIB_TEST_UNSTDLIB_H
#define UNSTDLIB_TEST_UNSTDLIB_H

#include "_test_common.h"
#include "test_headers/unstdinttypes/test_unstdinttypes.h"
#include "test_headers/_unstdcommon/test__unstdcommon.h"
#include "test_headers/unstdinternal/test_unstdinternal.h"
#include "test_headers/unstditertools/test_unstditertools.h"
#include "test_headers/unstdmemory/test_unstdmemory.h"
#include "test_headers/unstdstring/test_unstdstring.h"
#include "test_headers/unstdpair/test_unstdpair.h"
#include "test_headers/unstdio/test_unstdio.h"
#include "test_headers/unstdvector/test_unstdvector.h"
#include "test_headers/unstdsocket/test_unstdsocket.h"
#include "test_headers/unstdubytes/test_unstdubytes.h"
#include "test_headers/unstdbytebuffer/test_unstdbytebuffer.h"


void test_unstdlib_all(void) {
    test_unstdinttypes();
    test__unstdcommon();
    test_unstdinternal();
    test_unstditertools();
    test_unstdmemory();
    test_unstdstring();
    test_unstdpair();
    test_unstdio();
    test_unstdvector();
    test_unstdsocket();
    test_unstdubytes();
    test_unstdbytebuffer();
}

#endif /* UNSTDLIB_TEST_UNSTDLIB_H */
