#ifndef TALESTARS_V63_322_HOOK_DETECTOR_H
#define TALESTARS_V63_322_HOOK_DETECTOR_H

#include <unstdinttypes.h>
#include <unstdbool.h>

/* result struct */
typedef struct {
    bool found;
    enum : u8t {
        TR_NONE = 0, TR_BL, TR_BLR, TR_BR, TR_ADRP_BR
    } type;
    sizet offset;         /* offset inside scanned region */
    u32t word;         /* matched instruction word */
    uptrt resolved_target; /* 0 if not resolved; otherwise absolute address */
    u32t reg;          /* reg used (for ADRP/BR etc) */
} tramp_info_t;

/* Convenience pretty-print for detected info */
void hook_detector_print_tramp_info(
        const void *func_arg,
        const tramp_info_t *ti_arg
);

/* Detect trampoline-like patterns in the first `scan_len_arg` bytes of func_arg (cap to <= cap_limit_arg).
 * - func_arg: pointer to code (function address)
 * - scan_len_arg: how many bytes to examine (suggest 16 or 32)
 * - cap_limit_arg: extra cap to avoid huge scans (if 0 uses 256)
 * - out_arg: pointer to tramp_info_t to receive details
 *
 * returns true if any trampoline-like pattern was detected.
 */
bool hook_detector_detect_trampoline_like(
        const void *func_arg,
        sizet scan_len_arg,
        sizet cap_limit_arg,
        tramp_info_t *out_arg
);

#endif //TALESTARS_V63_322_HOOK_DETECTOR_H
