#include "unstdchar_compat.h"


i8 unstdchar_compat_tolower8(i8 buffer_arg) {
    if (buffer_arg >= 'A' && buffer_arg <= 'Z') {
        return buffer_arg + 32;
    }
    return buffer_arg;
}
