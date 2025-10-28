#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
static inline __attribute__((always_inline)) unsigned char* decode_636f6d2e7461__14(void){
    size_t __hc_len = 15u;
    unsigned char* buf = (unsigned char*)mallocM(__hc_len);
    if(!buf) return NULL;
    volatile unsigned long long __hc_tmpA,__hc_tmpB;
    /* block 0 */
    __hc_tmpA = 0xB820E8334C6CEA63ULL; __hc_tmpA = (((( (__hc_tmpA * 0x000000009D5E4B49ULL) >> 38 ) | ( (__hc_tmpA * 0x000000009D5E4B49ULL) << 26 )) & 0xFFFFFFFFFFFFFFFFULL) ^ 0x0000000036970A05ULL);
    __hc_tmpB = 0x185D0B9B5BD8D95BULL; __hc_tmpB = ((( ((( __hc_tmpB >> 50 ) | ( __hc_tmpB << 14 )) & 0xFFFFFFFFFFFFFFFFULL) << 36 ) | ( ((( __hc_tmpB >> 50 ) | ( __hc_tmpB << 14 )) & 0xFFFFFFFFFFFFFFFFULL) >> 28 )) & 0xFFFFFFFFFFFFFFFFULL);
    buf[0] = (unsigned char)((__hc_tmpB >> 0) & 0xFFULL);
    buf[1] = (unsigned char)((__hc_tmpB >> 8) & 0xFFULL);
    buf[2] = (unsigned char)((__hc_tmpB >> 16) & 0xFFULL);
    buf[3] = (unsigned char)((__hc_tmpB >> 24) & 0xFFULL);
    buf[4] = (unsigned char)((__hc_tmpB >> 32) & 0xFFULL);
    buf[5] = (unsigned char)((__hc_tmpB >> 40) & 0xFFULL);
    buf[6] = (unsigned char)((__hc_tmpB >> 48) & 0xFFULL);
    buf[7] = (unsigned char)((__hc_tmpB >> 56) & 0xFFULL);
    /* block 1 */
    __hc_tmpA = 0x05BDE0C3780BB3C5ULL; __hc_tmpA = (((( (__hc_tmpA + 0x0000000073A5E55CULL) << 26 ) | ( (__hc_tmpA + 0x0000000073A5E55CULL) >> 38 )) & 0xFFFFFFFFFFFFFFFFULL) * 0x000000008DA8AF8BULL);
    __hc_tmpB = 0x8C3BB737D108DFE0ULL; __hc_tmpB = ((__hc_tmpB - 0x000000006F325DD1ULL) ^ __hc_tmpA);
    buf[8] = (unsigned char)((__hc_tmpB >> 0) & 0xFFULL);
    buf[9] = (unsigned char)((__hc_tmpB >> 8) & 0xFFULL);
    buf[10] = (unsigned char)((__hc_tmpB >> 16) & 0xFFULL);
    buf[11] = (unsigned char)((__hc_tmpB >> 24) & 0xFFULL);
    buf[12] = (unsigned char)((__hc_tmpB >> 32) & 0xFFULL);
    buf[13] = (unsigned char)((__hc_tmpB >> 40) & 0xFFULL);
    buf[14] = 0x0;
    return buf;
}
static inline __attribute__((always_inline)) static inline size_t decode_636f6d2e7461__14_len(void){ return (size_t)14u; }