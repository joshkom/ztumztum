#ifndef UNSTDLIB_TEST_COMMON_H
#define UNSTDLIB_TEST_COMMON_H

#include <string.h>

#define ______unstd_strcmp(a, b) !strcmp(a, b)
#define _notify(notifySignArg, msgArg)                                      \
signed short int colorCode = 0;                                             \
if (______unstd_strcmp(notifySignArg, "[!]")) {                             \
    colorCode = 160; /* Red */                                              \
} else if (______unstd_strcmp(notifySignArg, "[?]")) {                      \
    colorCode = 123; /* Cyan */                                             \
} else if (______unstd_strcmp(notifySignArg, "[*]")) {                      \
    colorCode = 165; /* Magenta */                                          \
} else if (______unstd_strcmp(notifySignArg, "[+]")) {                      \
    colorCode = 84; /* Green */                                             \
} else if (______unstd_strcmp(notifySignArg, "[-]")) {                      \
    colorCode = 246; /* Dark Gray */                                        \
} else if (______unstd_strcmp(notifySignArg, "[ERROR]")) {                  \
    colorCode = 124; /* Dark Red */                                         \
} else if (______unstd_strcmp(notifySignArg, "[i]")) {                      \
    colorCode = 27;                                                         \
}                                                                           \
printf("\033[38;5;%d;1m%s\033[m %s\n", colorCode, notifySignArg, msgArg);   \
fflush(stdout)

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#endif /* UNSTDLIB_TEST_COMMON_H */
