#include "talecrypt.h"
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unstdmemory_compat.h>

//#define ERR_AND_SMSH(msg) LOGE(msg); demolisher_smash()
#define ERR_AND_SMSH(msg) demolisher_smash()

bool aes256_init(AES256Encryptor *enc, const uint8_t *key, size_t key_len) {
    if (!enc || !key || key_len != 32) {
        ERR_AND_SMSH("Key must be 32 bytes");
        return false;
    }
    unstdmemory_compat_memcpy(enc->key, key, 32);
    return true;
}


uint8_t *aes256_decrypt_from_buffer(const AES256Encryptor *enc,
                                    const ubytes encrypted_buf,
                                    const ubytes iv,
                                    sizet *out_len) {
    if (!enc || !encrypted_buf.data || !iv.data ||
        iv.length != 16 || encrypted_buf.length == 0) {
        ERR_AND_SMSH("Invalid params to decrypt_from_buffer");
        return NULL;
    }

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        ERR_AND_SMSH("Failed to create cipher context");
        return NULL;
    }

    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, enc->key, iv.data)) {
        EVP_CIPHER_CTX_free(ctx);
        ERR_AND_SMSH("DecryptInit failed");
        return NULL;
    }

    uint8_t *plaintext = (uint8_t *) malloc(encrypted_buf.length);
    if (!plaintext) {
        EVP_CIPHER_CTX_free(ctx);
        ERR_AND_SMSH("malloc failed");
        return NULL;
    }

    int len = 0, plaintext_len = 0;
    if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, encrypted_buf.data, encrypted_buf.length)) {
        EVP_CIPHER_CTX_free(ctx);
        freeM(plaintext);
        ERR_AND_SMSH("DecryptUpdate failed");
        return NULL;
    }
    plaintext_len = len;

    if (1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) {
        EVP_CIPHER_CTX_free(ctx);
        freeM(plaintext);
        ERR_AND_SMSH("DecryptFinal failed");
        return NULL;
    }
    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);
    *out_len = plaintext_len;
    return plaintext;
}

bool aes256_decrypt_and_deserialize_from_buffer(const AES256Encryptor *enc,
                                                const ubytes encrypted_buf,
                                                const ubytes iv,
                                                void *out_struct,
                                                sizet struct_size) {
    sizet dec_len = 0;
    uint8_t *decrypted = aes256_decrypt_from_buffer(enc, encrypted_buf, iv, &dec_len);
    if (!decrypted) {
        return false;
    }

    if (dec_len != struct_size) {
        freeM(decrypted);
        ERR_AND_SMSH("Invalid buffer size after decryption");
        return false;
    }

    unstdmemory_compat_memcpy(out_struct, decrypted, struct_size);
    freeM(decrypted);
    return true;
}

uint8_t *aes256_decrypt_from_file(const AES256Encryptor *enc,
                                  const char *filepath,
                                  const ubytes iv,
                                  sizet *out_len) {
    if (!enc || !filepath || !iv.data || iv.length != 16) {
        ERR_AND_SMSH("Invalid params to decrypt_from_file");
        return NULL;
    }

    FILE *fp = fopen(filepath, "rb");
    if (!fp) {
        ERR_AND_SMSH("Failed to open file for decryption");
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    long fsize = ftell(fp);
    if (fsize <= 0) {
        fclose(fp);
        ERR_AND_SMSH("Invalid or empty encrypted file");
        return NULL;
    }
    rewind(fp);

    uint8_t *encrypted = (uint8_t *) malloc(fsize);
    if (!encrypted) {
        fclose(fp);
        ERR_AND_SMSH("malloc failed");
        return NULL;
    }
    if (fread(encrypted, 1, fsize, fp) != (size_t) fsize) {
        fclose(fp);
        freeM(encrypted);
        ERR_AND_SMSH("Failed to read encrypted file");
        return NULL;
    }
    fclose(fp);

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        freeM(encrypted);
        ERR_AND_SMSH("Failed to create cipher context");
        return NULL;
    }

    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, enc->key, iv.data)) {
        EVP_CIPHER_CTX_free(ctx);
        freeM(encrypted);
        ERR_AND_SMSH("DecryptInit failed");
        return NULL;
    }

    uint8_t *plaintext = (uint8_t *) malloc(fsize);
    if (!plaintext) {
        EVP_CIPHER_CTX_free(ctx);
        freeM(encrypted);
        ERR_AND_SMSH("malloc failed");
        return NULL;
    }

    int len = 0, plaintext_len = 0;
    if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, encrypted, fsize)) {
        EVP_CIPHER_CTX_free(ctx);
        freeM(encrypted);
        freeM(plaintext);
        ERR_AND_SMSH("DecryptUpdate failed");
        return NULL;
    }
    plaintext_len = len;

    if (1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) {
        EVP_CIPHER_CTX_free(ctx);
        freeM(encrypted);
        freeM(plaintext);
        ERR_AND_SMSH("DecryptFinal failed");
        return NULL;
    }
    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);
    freeM(encrypted);

    *out_len = plaintext_len;
    return plaintext;
}

bool aes256_decrypt_and_deserialize_from_file(const AES256Encryptor *enc,
                                              const char *filepath,
                                              const ubytes iv,
                                              void *out_struct,
                                              sizet struct_size) {
    size_t dec_len = 0;
    uint8_t *decrypted = aes256_decrypt_from_file(enc, filepath, iv, &dec_len);
    if (!decrypted) {
        return false;
    }

    if (dec_len != struct_size) {
        freeM(decrypted);
        ERR_AND_SMSH("Invalid buffer size after decryption");
        return false;
    }

    unstdmemory_compat_memcpy(out_struct, decrypted, struct_size);
    freeM(decrypted);
    return true;
}
