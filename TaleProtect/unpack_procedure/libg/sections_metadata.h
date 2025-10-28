#ifndef TALESTARS_V63_322_SECTIONS_METADATA_H
#define TALESTARS_V63_322_SECTIONS_METADATA_H

#include <unstdinttypes.h>

typedef struct {
    const char* name;
    uint64_t offset;
    uint64_t size;
    uint8_t nonce[16];
} section_metadata_t;

extern section_metadata_t sections[];
extern sizet sections_count;

#endif /* TALESTARS_V63_322_SECTIONS_METADATA_H */
