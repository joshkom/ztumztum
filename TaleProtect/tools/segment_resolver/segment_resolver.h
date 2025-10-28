#ifndef TALESTARS_V63_322_SEGMENT_RESOLVER_H
#define TALESTARS_V63_322_SEGMENT_RESOLVER_H

#include <unstdinttypes.h>
#include <global_common/common.h>

#define MAX_RO_SEGMENTS 8

typedef struct {
    u8t *start;
    sizet size;
} segment_t;

typedef struct {
    u8t *base;
    u8t *end_of_exec;
    segment_t exec;                   /* .text / executable */
    segment_t ro[MAX_RO_SEGMENTS];    /* all read-only segments */
    i32 ro_count;                     /* number of valid ro segments */
} segments_t;

EXTERN_C segments_t get_segments(void *dummy_func_ptr_arg);

typedef enum {
    SEG_SEARCH_MAPS = 0,
    SEG_SEARCH_PHDR = 1,
    SEG_SEARCH_MAPS_DLADDR = 2
//    SEG_SEARCH_MAPS_DLINFO = 3,
} seg_search_mode_t;

EXTERN_C segments_t get_segments_ext(
        const char *lib_name_arg,
        seg_search_mode_t mode_arg,
        u8t pt_load_only_arg /* 0 = include all, 1 = only PT_LOAD */
);

#endif //TALESTARS_V63_322_SEGMENT_RESOLVER_H
