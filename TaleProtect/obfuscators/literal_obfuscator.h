#ifndef TALESTARS_V63_322_LITERAL_OBFUSCATOR_H
#define TALESTARS_V63_322_LITERAL_OBFUSCATOR_H

#pragma once

#include <stdint.h>
#include "global_common/common.h"

/* ----------------------------------------------------------------------
   Config (change per-build with a small generator if you want)
   ----------------------------------------------------------------------
   A must be odd so modular inverse exists. We precompute A_INV such that:
       (A * A_INV) % 2^32 == 1
*/
#define OBF_A        2654435761u        /* example: 0x9E3779B1 (Knuth constant) */
#define OBF_B        0xA5A5A5A5u
#define OBF_C        0x12345678u
#define OBF_A_INV    244002641u         /* modular inverse of OBF_A mod 2^32 */


/* encode at compile-time (expands to integer literal) */
#define OBF_INT32(x) ( (u32t)(((u32t)(x) * (u32t)OBF_A) ^ (u32t)OBF_B) + (u32t)OBF_C )

/* runtime deobfuscator (prevents constant-folding using inline asm as barrier) */
FORCE_INLINE s32t _literal_obfuscator_obf_deobf_runtime_uint32(u32t enc) {
    /* make value opaque to optimizer: the asm volatile with "+r"(v) is
       a read/write operand clobber acting as an optimization barrier. */
    u32t v = enc;
    __asm__ volatile("" : "+r"(v) : /* no inputs */ : /* no clobbers */);
    /* inverse transform: plain = (((v - C) ^ B) * A_INV) mod 2^32 */
    u32t t = (u32t) (v - (u32t) OBF_C);
    t = (t ^ (u32t) OBF_B);
    t = (u32t) ((u64t) t * (u64t) OBF_A_INV); /* multiply in 64-bit to be safe */
    return (s32t) t;
}

#define DEOBF_INT32(enc_literal_or_expr) (_literal_obfuscator_obf_deobf_runtime_uint32((u32t)(enc_literal_or_expr)))

/* Optional helper: decode and assign to variable of desired type */
#define DEOBF_INT32_TO(var, enc_literal_or_expr) \
    do { (var) = __obf_deobf_runtime_uint32((u32t)(enc_literal_or_expr)); } while (0)


#endif //TALESTARS_V63_322_LITERAL_OBFUSCATOR_H
