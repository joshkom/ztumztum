#ifndef TALESTARS_V63_322_TALECRYPT_H
#define TALESTARS_V63_322_TALECRYPT_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <TaleProtect/demolisher/smash.h>
#include <unstdubytes.h>

// AES256Encryptor equivalent in C
typedef struct {
    u8t key[32]; // fixed-size 256-bit key
} AES256Encryptor;

EXTERN_C_BEGIN

// Initialize encryptor from raw key (32 bytes)
bool aes256_init(AES256Encryptor *enc, const u8t *key, sizet key_len);

uint8_t *aes256_decrypt_from_buffer(
        const AES256Encryptor *enc,
        const ubytes encrypted_buf,
        const ubytes iv,
        sizet *out_len
);

bool aes256_decrypt_and_deserialize_from_buffer(
        const AES256Encryptor *enc,
        const ubytes encrypted_buf,
        const ubytes iv,
        void *out_struct,
        sizet struct_size
);

u8t *aes256_decrypt_from_file(
        const AES256Encryptor *enc,
        const i8 *filepath,
        const ubytes iv,
        sizet *out_len
);

// Deserialize struct from decrypted file
// Returns true on success, false on error.
bool aes256_decrypt_and_deserialize_from_file(
        const AES256Encryptor *enc,
        const i8 *filepath,
        const ubytes iv,
        void *out_struct,
        sizet struct_size
);

EXTERN_C_END

#endif //TALESTARS_V63_322_TALECRYPT_H
