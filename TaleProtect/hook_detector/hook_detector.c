/* chatgpt was here and decided to cook */

#include "hook_detector.h"
//#include "TaleProtect/func_resolver/func_resolver_3000.h"

#include <global_common/common.h>
#include <unstdmemory_compat.h>

/* Masks/values */
static const u32t AARCH64_BL_MASK = 0xFC000000u;
static const u32t AARCH64_BL_VALUE = 0x94000000u;

static const u32t AARCH64_BRBLR_MASK = 0xFFFFFC1Fu;
static const u32t AARCH64_BLR_VALUE = 0xD63F0000u;
static const u32t AARCH64_BR_VALUE = 0xD61F0000u;

/* ADRP detection (conservative): value 0x90000000 with mask 0x9F000000 */
static const u32t AARCH64_ADRP_MASK = 0x9F000000u;
static const u32t AARCH64_ADRP_VALUE = 0x90000000u;

/* ADD (immediate) detection for ADD (shifted immediate) 64-bit:
 * Use mask 0xFFC00000 and value 0x91000000 to match ADD (immediate).
 * This is a common pattern for ADD (imm) used after ADRP.
 */
static const u32t AARCH64_ADD_IMM_MASK = 0xFFC00000u;
static const u32t AARCH64_ADD_IMM_VAL = 0x91000000u;

/* safe 4-byte load */
static inline u32t load_u32_le(const void *p) {
    u32t w;
    unstdmemory_compat_memcpy(&w, p, sizeof(w));
    return w;
}

//#include <sys/mman.h>
//#include <unistd.h>
//#include <errno.h>
//#include <stdint.h>
//#include <string.h>

//static inline u32t load_u32_le(const void *p) {
//    u32t w = 0;
//    if (!p) {
//        return 0;
//    }
//
//    const sizet n = sizeof(w);
//
//    sizet page_size = (sizet) sysconf(_SC_PAGESIZE);
//    if (page_size == (sizet) -1) page_size = 4096;
//
//    uptrt src_start = (uptrt) p;
//    uptrt src_end = src_start + (uptrt) n - 1;
//    void *mstart = (void *) (src_start & ~(page_size - 1));
//    sizet mlen = (sizet) ((src_end - (uptrt) mstart) + 1);
//
//    /* round up to page boundary */
//    if (mlen % page_size) {
//        mlen = ((mlen / page_size) + 1) * page_size;
//    }
//
////    LOGI("load_u32_le: mprotect %p..%p (len=%zu) -> PROT_READ|PROT_EXEC",
////         mstart, (void *) ((uptrt) mstart + mlen), mlen);
//
//    if (mprotectM(mstart, mlen, PROT_READ | PROT_EXEC) != 0) {
////        int err = errno;
////        LOGE("load_u32_le: mprotect(PROT_READ|PROT_EXEC) failed for %p (len=%zu): %s (%d). "
////             "Skipping read to avoid SIGSEGV.", mstart, mlen, strerror(err), err);
//        return 0;
//    }
//
//    unstdmemory_compat_memcpy(&w, p, n);
//
//    if (mprotectM(mstart, mlen, PROT_EXEC) != 0) {
////        int err2 = errno;
////        LOGW("load_u32_le: mprotect(PROT_EXEC) restore failed for %p (len=%zu): %s (%d). "
////             "Pages may remain READ|EXEC.", mstart, mlen, strerror(err2), err2);
//    } else {
////        LOGI("load_u32_le: restored pages to PROT_EXEC for %p..%p",
////             mstart, (void *) ((uptrt) mstart + mlen));
//    }
//
//    return w;
//}


/* sign-extend 'val' which is 'bits' wide into signed 64-bit */
static inline int64_t sign_extend_bits(u64t val, unsigned bits) {
    u64t m = 1ull << (bits - 1);
    val = val & ((1ull << bits) - 1);
    return (val ^ m) - m; /* arithmetic trick */
}

/* compute BL target: imm26 << 2, sign-extended from 26 bits, added to instr address */
static inline uptrt resolve_bl_target(u32t instr, uptrt pc) {
    u32t imm26 = instr & 0x03FFFFFFu;
    int64_t simm26 = sign_extend_bits(imm26, 26);
    int64_t offset = simm26 << 2; /* imm26 is in words of 4 bytes */
    return (uptrt) ((int64_t) pc + offset);
}

/* compute ADRP base: immhi (bits 5..23) and immlo (bits 29..30)
 * imm21 = (immhi << 2) | immlo  (21 bits)
 * page_offset = sign_extend(imm21,21) << 12
 * target = (pc & ~0xFFF) + page_offset
 */
static inline uptrt resolve_adrp_base(u32t instr, uptrt pc) {
    u32t immlo = (instr >> 29) & 0x3u;
    u32t immhi = (instr >> 5) & 0x7FFFFu; /* 19 bits */
    uint64_t imm21 = ((uint64_t) immhi << 2) | (uint64_t) immlo;
    int64_t simm21 = sign_extend_bits(imm21, 21);
    int64_t page_offset = simm21 << 12; /* page offset */
    uptrt page_base = pc & ~((uptrt) 0xFFF);
    return (uptrt) ((int64_t) page_base + page_offset);
}

/* extract rd (bits 4:0) */
static inline unsigned instr_rd(u32t instr) { return instr & 0x1Fu; }

/* extract rn (bits 9:5) */
static inline unsigned instr_rn(u32t instr) { return (instr >> 5) & 0x1Fu; }

/* extract imm12 from ADD (imm) instruction bits [21:10] */
static inline u32t instr_imm12(u32t instr) { return (instr >> 10) & 0xFFFu; }


/* Decode BL imm26 -> compute sign-extended 26-bit immediate * 4 and target address.
 * instr is the 32-bit word (little-endian native).
 * pc is the instruction address (address of the 4-byte word).
 * returns the absolute target address.
 */
static inline uptrt bl_target_from_instr(u32t instr, uptrt pc) {
    /* imm26 is bits[25:0] (but shifted left 2) and sign-extended from 26 bits */
    int32_t imm26 = (int32_t) (instr & 0x03FFFFFFu);
    /* sign-extend 26-bit to 32-bit signed */
    if (imm26 & (1 << 25)) { /* negative */
        imm26 |= ~((1 << 26) - 1);
    }
    /* multiply by 4 (left shift 2) */
    int64_t offset = ((int64_t) imm26) << 2;
    /* PC for AArch64 branch immediates is address of current instruction */
    return (uptrt) ((int64_t) pc + offset);
}


/* scan the first 'scan_len_arg' bytes (rounded down to multiple of 4) for patterns.
 * cap_limit_arg prevents huge scans. scan_len_arg suggested 16 or 32.
 *
 * Returns true if a tramp-like pattern found; fills 'out_arg' when non-NULL.
 */
bool hook_detector_detect_trampoline_like(const void *func_arg,
                                          sizet scan_len_arg,
                                          sizet cap_limit_arg,
                                          tramp_info_t *out_arg) {
    if (!func_arg || scan_len_arg < 4) {
        return false;
    }
    if (!out_arg) {
        return false;
    }

    if (cap_limit_arg == 0) {
        cap_limit_arg = 256;
    }
    if (scan_len_arg > cap_limit_arg) {
        scan_len_arg = cap_limit_arg;
    }
    scan_len_arg &= ~((sizet) 3);

    const u8t *base = (const u8t *) func_arg;
    unstdmemory_compat_memset(out_arg, 0, sizeof(*out_arg));

    for (sizet off = 0; off + 4 <= scan_len_arg; off += 4) {
        const void *ptr = base + off;
        uptrt pc = (uptrt) ptr;
        u32t w = load_u32_le(ptr);

        /* BL imm26 */
        if ((w & AARCH64_BL_MASK) == AARCH64_BL_VALUE) {
            out_arg->found = true;
            out_arg->type = TR_BL;
            out_arg->offset = off;
            out_arg->word = w;
            out_arg->resolved_target = resolve_bl_target(w, pc);
            out_arg->reg = 0;
            return true;
        }

        /* BLR / BR - we can detect but only resolve if matched register came from ADRP (+ADD) earlier */
        if ((w & AARCH64_BRBLR_MASK) == AARCH64_BLR_VALUE) {
            out_arg->found = true;
            out_arg->type = TR_BLR;
            out_arg->offset = off;
            out_arg->word = w;
            out_arg->resolved_target = 0;
            out_arg->reg = instr_rn(w);
            /* we keep searching earlier ADRP in preceding instructions? Better to scan forward only:
               but for simplicity, if ADRP appears before and loaded same reg, we will catch it when ADRP encountered below. */
            return true;
        }
        if ((w & AARCH64_BRBLR_MASK) == AARCH64_BR_VALUE) {
            out_arg->found = true;
            out_arg->type = TR_BR;
            out_arg->offset = off;
            out_arg->word = w;
            out_arg->resolved_target = 0;
            out_arg->reg = instr_rn(w);
            return true;
        }

        /* ADRP detected: try to resolve ADRP [+ optional ADD] followed by BR/BLR */
        if ((w & AARCH64_ADRP_MASK) == AARCH64_ADRP_VALUE) {
            unsigned rd = instr_rd(w);
            uptrt adrp_base = resolve_adrp_base(w, pc);
            /* look ahead for up to 2 instructions (ADD imm, BR/BLR) */
            sizet look_max = 2;
            for (sizet k = 1; k <= look_max; ++k) {
                sizet joff = off + 4 * k;
                if (joff + 4 > scan_len_arg) break;
                u32t w2 = load_u32_le(base + joff);

                /* If ADD (imm) and it writes back to same register (rd = rn = rd_adrp) */
                if ((w2 & AARCH64_ADD_IMM_MASK) == AARCH64_ADD_IMM_VAL) {
                    unsigned rd2 = instr_rd(w2);
                    unsigned rn2 = instr_rn(w2);
                    if (rd2 == rd && rn2 == rd) {
                        u32t imm12 = instr_imm12(w2);
                        uptrt final_addr = adrp_base + (uptrt) imm12;
                        /* Now check next instruction for BR/BLR that uses rd */
                        sizet joff2 = joff + 4;
                        if (joff2 + 4 <= scan_len_arg) {
                            u32t w3 = load_u32_le(base + joff2);
                            if ((w3 & AARCH64_BRBLR_MASK) == AARCH64_BR_VALUE ||
                                (w3 & AARCH64_BRBLR_MASK) == AARCH64_BLR_VALUE) {
                                unsigned rn3 = instr_rn(w3);
                                if (rn3 == rd) {
                                    out_arg->found = true;
                                    out_arg->type = TR_ADRP_BR;
                                    out_arg->offset = off;
                                    out_arg->word = w; /* ADRP word */
                                    out_arg->resolved_target = final_addr;
                                    out_arg->reg = rd;
                                    return true;
                                }
                            }
                        }
                        /* even if BR not present immediately, consider ADRP+ADD as resolved value */
                    }
                }

                /* Check direct BR/BLR using same register as ADRP's rd */
                if ((w2 & AARCH64_BRBLR_MASK) == AARCH64_BR_VALUE ||
                    (w2 & AARCH64_BRBLR_MASK) == AARCH64_BLR_VALUE) {
                    unsigned rn2 = instr_rn(w2);
                    if (rn2 == rd) {
                        out_arg->found = true;
                        out_arg->type = TR_ADRP_BR;
                        out_arg->offset = off;
                        out_arg->word = w;
                        out_arg->resolved_target = adrp_base; /* page base; may require ADD to be exact */
                        out_arg->reg = rd;
                        return true;
                    }
                }
            }
            /* didn't resolve with immediate BR in lookahead; continue scanning */
        }
    }

    return false;
}

void hook_detector_print_tramp_info(const void *func_arg, const tramp_info_t *t) {
    if (!t) {
        return;
    }
    if (!t->found) {
        LOGI("No trampoline-like pattern at %p", func_arg);
        return;
    }
    const char *type = "UNK";
    switch (t->type) {
        case TR_BL:
            type = "BL";
            break;
        case TR_BLR:
            type = "BLR";
            break;
        case TR_BR:
            type = "BR";
            break;
        case TR_ADRP_BR:
            type = "ADRP+BR";
            break;
        default:
            break;
    }
    if (t->resolved_target) {
        LOGI("Detected %s at %p+%zu (word=0x%08x) -> target=0x%zx (reg x%u)",
             type, func_arg, t->offset, t->word, (sizet) t->resolved_target, t->reg);
    } else {
        LOGI("Detected %s at %p+%zu (word=0x%08x) -> target unknown (reg x%u)",
             type, func_arg, t->offset, t->word, t->reg);
    }
}
