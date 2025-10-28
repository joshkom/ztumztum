//
// Created by johan on 12.10.25.
//

#ifndef TALESTARS_V63_322_NICETITS_H
#define TALESTARS_V63_322_NICETITS_H

#include "TaleProtect/gcontext.h"

#define NICETITS_CONF_XOR_KEY 0xAA

typedef struct __attribute__((__packed__)) nicetits_s {
    struct {
        hashset_t hashset; /* SHA-256 hash specifically */
        u64t offset;
    } apksigv2;
} nicetits_t;

extern nicetits_t nicetits;

void nicetits_xor_decrypt_buffer(u8t *buf, sizet size);

#endif //TALESTARS_V63_322_NICETITS_H
