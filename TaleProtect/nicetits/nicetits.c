//
// Created by johan on 12.10.25.
// its 5 in the morning, yes, 5am. im too tired of this schit
#include "nicetits.h"

nicetits_t nicetits = {0};

void nicetits_xor_decrypt_buffer(u8t *buf, sizet size) {
    for (sizet i = 0; i < size; i++) {
        buf[i] ^= NICETITS_CONF_XOR_KEY;
    }
}
