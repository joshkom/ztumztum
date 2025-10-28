#include <iostream>
#include <fstream>
#include <cstring>
#include "talecrypt/talecrypt.hpp"
#include "keys.hpp"

//outdated
//#pragma pack(push, 1)
//struct __attribute__((__packed__)) EncryptedUpdateInfo {
////    uint8_t md5sum[16];                   // raw MD5 bytes
//    uint8_t apksigv2_sha256_hash[32];               // SHA-256 hash
//    uint64_t apksigv2_offset;              // APK Sig v2 offset
//    uint8_t major;
//    uint8_t minor;
//    uint8_t patch;
//    uint8_t bsversion;
////    uint8_t aes_key[32];                   // raw AES-256 key
////    uint8_t iv[16];                        // raw AES IV
////    uint8_t libtale_protect_sha256_hash[32];
////    uint8_t download_url[96];              // null-terminated UTF-8
////    char download_url[96];              // null-terminated UTF-8
//};

typedef struct __attribute__((__packed__)) hashset_s {
    unsigned char sha256[32];
    unsigned char blake3[32];
    unsigned char md5[16];
} hashset_t;

typedef struct __attribute__((__packed__)) lib_hashsets_s {
    hashset_t PT_LOADS[2];
    size_t end_of_exec;
} elf_hashsets_t;

typedef struct integrity_native_lib_dir_entry_s {
    char name[128];
    hashset_t hash;
} integrity_native_lib_dir_entry_t;

typedef struct __attribute__((__packed__)) NotAnElf_s {
    elf_hashsets_t taleprotect;
    struct {
        struct {
            elf_hashsets_t gadget;
            hashset_t config;
            hashset_t script;
        } tale;
        struct {
            elf_hashsets_t gadget;
            hashset_t config;
            hashset_t script;
        } stars;
    } gadgets;

    integrity_native_lib_dir_entry_t integrity_native_lib_dir_entries[32];
} NotAnElf_t;

typedef struct __attribute__((__packed__)) nicetits_s {
    struct {
        hashset_t hashset; /* SHA-256 hash specifically */
        uint64_t offset;
    } apksigv2;
} nicetits_t;

struct int_dir_entry {
    const char *name;
    const char *hash; /* blake3 specifically, hence the size */
} static entries[] = {
        {"libtale.not_an_elf.so",           "dabb8bba69a4d72bc268acaf57443a324fad7a60b56971288531bb628682b270"}, /* chicken/egg problem right here bro; gotta store this separately, directly within libtale.protect.so within `TaleProtectContext.integrity_monitor.file_integrity.entry_not_an_elf.hash.blake3`, or just make a separate section for it */
        {"libapp.so",                       "73979873fd7f8b331732d4814e7fe8ff9ca0f9f4cbc1561fd4348717ab49587b"},
        {"libtale.config.so",               "777b88d3affda0c071fce02f221eae6b8ef74264303938ebd9419a80783a2bbb"},
        {"libc++_shared.so",                "c63b2f567ca3bb2c8af4d6601691ddfdb8a62791e84865dfd39495b5ea042a74"},
        {"libtale.protect.so",              "eaa2177a73c4f8b85b82be7c4853f68c2295598886c5c0aede5fb0c0270a4efe"},
        {"libscid_sdk.so",                  "2b70cf74edf4bdaa1afa2296d75481b34895b860976bf37f1d384f9adec34f2f"},
        {"libroyale.config.so",              "3169e21a4d0e5021b143dfafe4d281805736a17c311c6157f94319741f0cd32c"},
        {"libg.so",                         "c39ee5e7094f1bdb3589f9bfa7edd04e6d37e04b5e3bb7108eeff4614419aa40"},
        {"libflutter.so",                   "561a5c0ce5fb768082e8fa20ef0893251107f32366b46e77f102ae70a6c77945"},
        {"libroyale.script.so",              "cf8d59ea8ddb124b28c9c83c931903f1d16d8ae643144fad96ca5a8660bc3590"},
        {"libroyale.so",                     "6b88b934940e3699180eb852b3efcd850844afb69dd7e30c97515e69d72d0016"},
        {"libb.so",                         "ef86c10dec1d23a8485ef10c904ebac7fffaad2dea7ee2c395cd095aefe56831"},
        {"libimage_processing_util_jni.so", "46f47d8f2427ffb3f1da4ad25aae63d4a6c5196e0ec665be84efc9298aa3ab36"},
        {"libsentry-android.so",            "362596a40aaebeacd378ab6854ece0dd27b2469248ad1e6407fa23016013acda"},
        {"libsentry.so",                    "b1ee428133b4e57a4f6c9f5c175076716146f192307d21fa3e57d69790007eac"},
        {"libfmod.so",                      "0a51240ede5f8eb68005b8ef1de453b7309ce3b245630c56aedaedf1e6d9668e"},
        {"libfmodstudio.so",                "b8ee1a28c0330d9f28899f8b88a9e7e0f8bb565b3debe4f3cb03b79af49be409"},
        {"libtale.so",                      "93b48b658a39abd7cc2763c1138918ad01f2e97eec4b62750a3d69fe43f2ae78"},
        {"libtoolChecker.so",               "762b230087bb8a9f61ddf6a6902d8c1700fca6557f2298ecf7dbe053ce090e3a"},
        {"libtale.script.so",               "99c8f5ef245302e02f6794a10b9d792ce881d4bc733d93eb766844e1742f25f9"}
};


//#pragma pack(pop)

//9fdc1ac5d01fa4776aaf865c3f591ab235005b8fad170b962a8df0e6f146978c

// update meta keys
//static uint8_t STATIC_KEY[32] = {
//        0x3a, 0x7a, 0x01, 0xf2, 0x84, 0x19, 0x55, 0xd1,
//        0x66, 0x9b, 0x7b, 0x0f, 0x72, 0x39, 0xde, 0xdc,
//        0x48, 0x2c, 0x50, 0xa3, 0xfe, 0x67, 0x11, 0xd4,
//        0x8c, 0x2d, 0xff, 0xc7, 0x91, 0x60, 0xbc, 0x3f
//};
//static uint8_t STATIC_IV[16] = {
//        0x7f, 0x42, 0x09, 0x0b, 0xa3, 0x1a, 0x16, 0xe4,
//        0x21, 0xcd, 0x04, 0x2a, 0x5e, 0xff, 0x6b, 0xc7
//};

#include <iomanip>  // std::hex, std::setw, etc.
#include <sstream>
#include <filesystem>

std::string to_hex_string(const std::vector<uint8_t> &data) {
    std::ostringstream oss;
    for (uint8_t b: data)
        oss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(b);
    return oss.str();
}

bool rename_file(const std::string &oldName, const std::string &newName) {
    int result = std::rename(oldName.c_str(), newName.c_str());
    if (result != 0) {
        perror("Error renaming file");
        return false;
    }
    return true;
}

bool copy_file(const std::string &sourcePath, const std::string &destPath) {
    std::error_code ec;
    bool result = std::filesystem::copy_file(sourcePath, destPath,
                                             std::filesystem::copy_options::overwrite_existing, ec);

    if (!result || ec) {
        std::cerr << "Error copying file: " << ec.message() << '\n';
        return false;
    }
    return true;
}
//#define _GNU_SOURCE

#include <fcntl.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define BUF_SIZE 4096

struct linux_dirent64 {
    uint64_t d_ino;
    int64_t d_off;
    unsigned short d_reclen;
    unsigned char d_type;
    char d_name[];
};

void hex_to_bytes(const char *hex, uint8_t *out, size_t len) {
    for (size_t i = 0; i < len; i++) {
        sscanf(hex + i * 2, "%2hhx", &out[i]);
    }
}

#include <cstdlib>
#include <cerrno>

bool hex_to_bytes_safe(const char *hex, uint8_t *out, size_t len) {
    char byte_str[3] = {0};
    for (size_t i = 0; i < len; i++) {
        byte_str[0] = hex[i * 2];
        byte_str[1] = hex[i * 2 + 1];
        char *endptr;
        errno = 0;
        unsigned long val = strtoul(byte_str, &endptr, 16);
        if (errno != 0 || endptr != byte_str + 2 || val > 0xFF)
            return false;
        out[i] = static_cast<uint8_t>(val);
    }
    return true;
}

std::string bytes_to_hex(const uint8_t *bytes, size_t len) {
    char buf[3];
    std::string out;
    out.reserve(len * 2);
    for (size_t i = 0; i < len; i++) {
        snprintf(buf, sizeof(buf), "%02x", bytes[i]);
        out += buf;
    }
    return out;
}


int main() {
    //! [LIBTALEPROTECT]
    {
        NotAnElf_t ImNotAnElf{};

        if (!hex_to_bytes_safe("2f5e6d5bc78de702312824551ea1a3020b40af0393ee904eff2e04514ab80f15",
                               ImNotAnElf.taleprotect.PT_LOADS[0].sha256,
                               32
        )) {
            fprintf(stderr, "Invalid SHA256 hex string\n");
            return 1;
        }
        if (!hex_to_bytes_safe("0000000000000000000000000000000000000000000000000000000000000000", /* blake3 */
                               ImNotAnElf.taleprotect.PT_LOADS[0].blake3,
                               32
        )) {
            fprintf(stderr, "Invalid SHA256 hex string\n");
            return 1;
        }
        if (!hex_to_bytes_safe("293be06fad70dca22d4fcad5f02fb260",
                               ImNotAnElf.taleprotect.PT_LOADS[0].md5,
                               16
        )) {
            fprintf(stderr, "Invalid SHA256 hex string\n");
            return 1;
        }

        // gadgets::tale::gadget
//    ImNotAnElf.gadgets.tale.gadget.end_of_exec = 0x67AE10;
        ImNotAnElf.gadgets.tale.gadget.end_of_exec = 0x0;
        if (!hex_to_bytes_safe("0000000000000000000000000000000000000000000000000000000000000000",
                               ImNotAnElf.gadgets.tale.gadget.PT_LOADS[0].sha256,
                               32
        )) {
            fprintf(stderr, "Invalid SHA256 hex string\n");
            return 1;
        }
        if (!hex_to_bytes_safe("36420617dd1ef18ba5deeba64be60393ae230309eb7e1f9fd5ebac0e420e7707", /* blake3 */
                               ImNotAnElf.gadgets.tale.gadget.PT_LOADS[0].blake3,
                               32
        )) {
            fprintf(stderr, "Invalid SHA256 hex string\n");
            return 1;
        }
        if (!hex_to_bytes_safe("00000000000000000000000000000000",
                               ImNotAnElf.gadgets.tale.gadget.PT_LOADS[0].md5,
                               16
        )) {
            fprintf(stderr, "Invalid SHA256 hex string\n");
            return 1;
        }

        // gadgets::stars::gadget
//    ImNotAnElf.gadgets.stars.gadget.end_of_exec = 0x67AE10;
        ImNotAnElf.gadgets.stars.gadget.end_of_exec = 0x0;
        if (!hex_to_bytes_safe("0000000000000000000000000000000000000000000000000000000000000000",
                               ImNotAnElf.gadgets.stars.gadget.PT_LOADS[0].sha256,
                               32
        )) {
            fprintf(stderr, "Invalid SHA256 hex string\n");
            return 1;
        }
        if (!hex_to_bytes_safe("ce528adb12243aa19ece006ff8295803cfb3f642d35f6483fa21ddde56920ece", /* blake3 */
                               ImNotAnElf.gadgets.stars.gadget.PT_LOADS[0].blake3,
                               32
        )) {
            fprintf(stderr, "Invalid SHA256 hex string\n");
            return 1;
        }
        if (!hex_to_bytes_safe("00000000000000000000000000000000",
                               ImNotAnElf.gadgets.stars.gadget.PT_LOADS[0].md5,
                               16
        )) {
            fprintf(stderr, "Invalid SHA256 hex string\n");
            return 1;
        }

        size_t num_entries = sizeof(entries) / sizeof(entries[0]);

        for (size_t i = 0; i < num_entries; i++) {
            strcpy(ImNotAnElf.integrity_native_lib_dir_entries[i].name, entries[i].name);
            if (!hex_to_bytes_safe(entries[i].hash,
                                   ImNotAnElf.integrity_native_lib_dir_entries[i].hash.blake3,
                                   32)) {
                fprintf(stderr, "Invalid hash for %s\n", entries[i].name);
            }
        }


        talecrypt::AES256Encryptor aes(rasp_repo_keys::NotAnElfKeys::KEY_VEC);
        auto buffer = talecrypt::AES256Encryptor::serialize_struct(ImNotAnElf);
        auto encrypted = aes.encrypt(buffer, rasp_repo_keys::NotAnElfKeys::IV_VEC);
        auto decrypted = aes.decrypt(encrypted, rasp_repo_keys::NotAnElfKeys::IV_VEC);
        NotAnElf_t test_loaded =
                talecrypt::AES256Encryptor::deserialize_struct<NotAnElf_t>(decrypted);

        printf("=== [IN-MEMORY TEST] ===\n");
//    printf("Decrypted MD5: %s\n", bytes_to_hex(test_loaded.apksigv2_sha256_hash, 16).c_str());

        std::ofstream out("libtale.not_an_elf.so", std::ios::binary);
        out.write((const char *) encrypted.data(), (long) encrypted.size());
        out.close();

        std::ofstream meta("libtale.not_an_elf.so.meta", std::ios::binary);
        meta.write((const char *) &ImNotAnElf, sizeof(ImNotAnElf));
        meta.close();

        std::ifstream in("libtale.not_an_elf.so", std::ios::binary | std::ios::ate);
        auto size = in.tellg();
        in.seekg(0, std::ios::beg);
        std::vector<uint8_t> encrypted_read(size);
        in.read((char *) encrypted_read.data(), size);
        in.close();

        auto decrypted2 = aes.decrypt(encrypted_read, rasp_repo_keys::NotAnElfKeys::IV_VEC);
        NotAnElf_t loaded =
                talecrypt::AES256Encryptor::deserialize_struct<NotAnElf_t>(decrypted2);

//    printf("MD5: %s\n", bytes_to_hex(loaded.md5sum, 16).c_str());
        printf("NAE sha256: %s\n", bytes_to_hex(loaded.taleprotect.PT_LOADS[0].sha256, 32).c_str());
        printf("NAE md5: %s\n", bytes_to_hex(loaded.taleprotect.PT_LOADS[0].md5, 16).c_str());
    }


//     ============================= server config

    {
        nicetits_t nicetits;

//        nicetits.apksigv2.offset = 1688141808;
        nicetits.apksigv2.offset = 1090715632;

        if (!hex_to_bytes_safe("eb069a5fd80fb7e6e93300a24c935aa612a9d2d3898761b4f7d614ac6b831b79",
                               nicetits.apksigv2.hashset.sha256,
                               32
        )) {
            fprintf(stderr, "Invalid SHA256 hex string\n");
            return 1;
        }


        talecrypt::AES256Encryptor aes(rasp_repo_keys::NiceTitsKeys::KEY_VEC);
        auto buffer = talecrypt::AES256Encryptor::serialize_struct(nicetits);
        auto encrypted = aes.encrypt(buffer, rasp_repo_keys::NiceTitsKeys::IV_VEC);
        auto decrypted = aes.decrypt(encrypted, rasp_repo_keys::NiceTitsKeys::IV_VEC);
//        NotAnElf_t test_loaded =
//                talecrypt::AES256Encryptor::deserialize_struct<NotAnElf_t>(decrypted);

        printf("=== [IN-MEMORY TEST] ===\n");

        std::ofstream out("nicedeck", std::ios::binary);
        out.write((const char *) encrypted.data(), (long) encrypted.size());
        out.close();

        std::ofstream meta("nicetits.meta", std::ios::binary);
        meta.write((const char *) &nicetits, sizeof(nicetits));
        meta.close();

        std::ifstream in("nicedeck", std::ios::binary | std::ios::ate);
        auto size = in.tellg();
        in.seekg(0, std::ios::beg);
        std::vector<uint8_t> encrypted_read(size);
        in.read((char *) encrypted_read.data(), size);
        in.close();

        printf("apksigv2=>hash=>sha256: %s\n", bytes_to_hex(nicetits.apksigv2.hashset.sha256, 16).c_str());
    }

    return 0;
}
