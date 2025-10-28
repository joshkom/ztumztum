#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
static inline __attribute__((always_inline)) unsigned char* decode_646f626279__5(void){
    size_t __hc_len = 6u;
    unsigned char* buf = (unsigned char*)mallocM(__hc_len);
    if(!buf) return NULL;
    volatile unsigned long long __hc_tmpA,__hc_tmpB;
    /* block 0 */
    __hc_tmpA = 0xB820E8334C6CEA63ULL; __hc_tmpA = (((( (__hc_tmpA * 0x000000009D5E4B49ULL) >> 38 ) | ( (__hc_tmpA * 0x000000009D5E4B49ULL) << 26 )) & 0xFFFFFFFFFFFFFFFFULL) ^ 0x0000000036970A05ULL);
    __hc_tmpB = 0x001E58989BD90000ULL; __hc_tmpB = ((( ((( __hc_tmpB >> 50 ) | ( __hc_tmpB << 14 )) & 0xFFFFFFFFFFFFFFFFULL) << 36 ) | ( ((( __hc_tmpB >> 50 ) | ( __hc_tmpB << 14 )) & 0xFFFFFFFFFFFFFFFFULL) >> 28 )) & 0xFFFFFFFFFFFFFFFFULL);
    buf[0] = (unsigned char)((__hc_tmpB >> 0) & 0xFFULL);
    buf[1] = (unsigned char)((__hc_tmpB >> 8) & 0xFFULL);
    buf[2] = (unsigned char)((__hc_tmpB >> 16) & 0xFFULL);
    buf[3] = (unsigned char)((__hc_tmpB >> 24) & 0xFFULL);
    buf[4] = (unsigned char)((__hc_tmpB >> 32) & 0xFFULL);
    buf[5] = 0x0;
    return buf;
}
static inline __attribute__((always_inline)) static inline size_t decode_646f626279__5_len(void){ return (size_t)5u; }