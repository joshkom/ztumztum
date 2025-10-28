#ifndef UNSTDLIB_H
#define UNSTDLIB_H

#ifdef __STDC__
#ifdef __STDC_VERSION__
#if (__STDC_VERSION__ < 201112L)
#error "Compiler's version should be at least C11."
#endif
#endif
#endif

#include "_unstdcommon.h"
#include "unstdbool.h"
#include "unstdinttypes.h"
#include "unstdinternal.h"
#include "unstdio.h"
#include "unstditertools.h"
#include "unstdmemory.h"
#include "unstdpair.h"
#include "unstdstring.h"
#include "unstdvector.h"
#include "unstdsocket.h"

#endif /* UNSTDLIB_H */
