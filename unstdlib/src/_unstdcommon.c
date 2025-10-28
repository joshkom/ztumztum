#include "_unstdcommon.h"


extern u8t unstdcommon_endianness(void) {
    u16t i = 0x00ff;
    return ((u8t *) &i)[0] == 0x00;
}
