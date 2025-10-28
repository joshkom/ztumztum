#ifndef UNSTDLIB_UNSTDBOOL_H
#define UNSTDLIB_UNSTDBOOL_H

#if !defined(__cplusplus) && !(defined(__bool_true_false_are_defined) || defined(bool))
typedef unsigned char bool;
#define true (bool) 1
#define false (bool) 0
#else
#include <stdbool.h>
#endif

#endif /* UNSTDLIB_UNSTDBOOL_H */
