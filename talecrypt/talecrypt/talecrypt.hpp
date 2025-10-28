//
// Created by johan on 19.07.25.
//

#ifndef BSWRAPPER_TALECRYPT_HPP
#define BSWRAPPER_TALECRYPT_HPP

#include <openssl/evp.h>
#include <openssl/rand.h>
#include <vector>
#include <string>
#include <cstdio>
#include <cctype>
#include <cstring>

#include <iomanip>
#include <sstream>

#define ERR(msg) do { \
    std::fprintf(stderr, "ERROR: %s\n", msg); \
} while(0)

#include <iomanip>  // std::hex, std::setw, etc.
#include <sstream>
#include <fstream>

class talecrypt {
public:
    class MD5Helper {
    public:
        inline static std::string to_hex(const unsigned char *digest, std::size_t len) {
            std::ostringstream ss;
            for (std::size_t i = 0; i < len; ++i)
                ss << std::hex << std::setw(2) << std::setfill('0') << (int) digest[i];
            return ss.str();
        }

        inline static std::string hash_bytes(const void *data, size_t size) {
            EVP_MD_CTX *ctx = EVP_MD_CTX_new();
            if (!ctx) return {};

            unsigned char digest[EVP_MAX_MD_SIZE];
            unsigned int digest_len = 0;

            if (EVP_DigestInit_ex(ctx, EVP_md5(), nullptr) &&
                EVP_DigestUpdate(ctx, data, size) &&
                EVP_DigestFinal_ex(ctx, digest, &digest_len)) {
                EVP_MD_CTX_free(ctx);
                return to_hex(digest, digest_len);
            }

            EVP_MD_CTX_free(ctx);
            return {};
        }

        inline static std::string from_string(const std::string &input) {
            return hash_bytes(input.data(), input.size());
        }

        inline static std::string from_buffer(const void *data, std::size_t size) {
            return hash_bytes(data, size);
        }

        inline static std::string from_vector(const std::vector<uint8_t> &buffer) {
            return hash_bytes(buffer.data(), buffer.size());
        }

        inline static std::string from_file(FILE *file) {
            if (!file) return {};

            constexpr size_t buf_size = 4096;
            unsigned char buffer[buf_size];
            unsigned char digest[EVP_MAX_MD_SIZE];
            unsigned int digest_len = 0;

            EVP_MD_CTX *ctx = EVP_MD_CTX_new();
            if (!ctx) return {};

            if (!EVP_DigestInit_ex(ctx, EVP_md5(), nullptr)) {
                EVP_MD_CTX_free(ctx);
                return {};
            }

            size_t bytes_read = 0;
            while ((bytes_read = fread(buffer, 1, buf_size, file)) != 0) {
                if (!EVP_DigestUpdate(ctx, buffer, bytes_read)) {
                    EVP_MD_CTX_free(ctx);
                    return {};
                }
            }

            if (!EVP_DigestFinal_ex(ctx, digest, &digest_len)) {
                EVP_MD_CTX_free(ctx);
                return {};
            }

            EVP_MD_CTX_free(ctx);
            return to_hex(digest, digest_len);
        }

        inline static std::string from_file_path(const std::string &filepath) {
            FILE *file = fopen(filepath.c_str(), "rb");
            if (!file) return {};
            std::string result = from_file(file);
            fclose(file);
            return result;
        }
    };

    class AES256Encryptor {
    private:
        std::vector<uint8_t> key_;
    public:
        static bool encrypt_file(const std::string &input_path,
                                 const std::string &output_path,
                                 const std::vector<uint8_t> &key,
                                 const std::vector<uint8_t> &iv);

        static bool decrypt_file(const std::string &input_path,
                                 const std::string &output_path,
                                 const std::vector<uint8_t> &key,
                                 const std::vector<uint8_t> &iv);

//        static std::vector<uint8_t> hex_to_bytes(const char *hex);

        // Helper: convert hex string to byte vector
        static std::vector<uint8_t> hex_to_bytes(const std::string &hex);

        static std::string to_hex_string(const std::vector<uint8_t> &data);

        static uint8_t hex_char_to_nibble(char c);

        template<typename T>
        static T deserialize_struct(const std::vector<uint8_t> &buffer) {
            if (buffer.size() != sizeof(T)) ERR("Invalid buffer size");
            T obj;
            std::memcpy(&obj, buffer.data(), sizeof(T));
            return obj;
        }

        template<typename T>
        static std::vector<uint8_t> serialize_struct(const T &obj) {
            return {
                    reinterpret_cast<const uint8_t *>(&obj),
                    reinterpret_cast<const uint8_t *>(&obj) + sizeof(T)
            };
        }

        static std::vector<uint8_t> generate_key();

        AES256Encryptor() : key_(generate_key()) {}

        explicit AES256Encryptor(const std::vector<uint8_t> &key);

        explicit AES256Encryptor(const std::string &hex_key);

        std::vector<uint8_t> encrypt(const std::vector<uint8_t> &plaintext,
                                     std::vector<uint8_t> &iv) const;

        [[nodiscard]] std::vector<uint8_t> decrypt(const std::vector<uint8_t> &ciphertext,
                                                   const std::vector<uint8_t> &iv) const;

        std::vector<uint8_t> encrypt_string(const std::string &plaintext,
                                            std::vector<uint8_t> &iv) const;

        std::string encrypt_string_to_hex(const std::string &plaintext,
                                          std::vector<uint8_t> &iv) const;


        std::string decrypt_string(const std::vector<uint8_t> &ciphertext,
                                   const std::vector<uint8_t> &iv) const;

        std::string decrypt_string_from_hex(const std::string &hex_ciphertext,
                                            const std::vector<uint8_t> &iv) const;

        bool encrypt_to_file(const std::vector<uint8_t> &plaintext,
                             const std::string &filepath,
                             std::vector<uint8_t> &iv) const;

        template<typename T>
        void serialize_and_encrypt_to_file(const T &data,
                                           const std::string &filepath,
                                           std::vector<uint8_t> &iv) const {
            auto serialized = serialize_struct(data);
            auto encrypted = encrypt(serialized, iv);

            std::ofstream out(filepath, std::ios::binary);
            if (!out.is_open()) {
                ERR("Failed to open file for writing.");
            }
            out.write(reinterpret_cast<const char *>(encrypted.data()), encrypted.size());
        }


        std::vector<uint8_t> decrypt_from_file(const std::string &filepath,
                                               const std::vector<uint8_t> &iv) const;

        template<typename T>
        T decrypt_and_deserialize_from_file(const std::string &filepath,
                                            const std::vector<uint8_t> &iv) const {
            auto decrypted = decrypt_from_file(filepath, iv);
            return deserialize_struct<T>(decrypted);
        }

        [[nodiscard]] const std::vector<uint8_t> &get_key() const;
    };

};

#endif //BSWRAPPER_TALECRYPT_HPP