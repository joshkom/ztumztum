#include <fstream>
#include "talecrypt.hpp"


const std::vector<uint8_t> &talecrypt::AES256Encryptor::get_key() const { return key_; }

bool talecrypt::AES256Encryptor::encrypt_file(const std::string &input_path,
                                              const std::string &output_path,
                                              const std::vector<uint8_t> &key,
                                              const std::vector<uint8_t> &iv) {
    std::ifstream in(input_path, std::ios::binary | std::ios::ate);
    if (!in.is_open()) {
        ERR("Failed to open file for decryption");
    }

    std::streamsize size = in.tellg();
    in.seekg(0, std::ios::beg);
    std::vector<uint8_t> buffer(size);
    if (!in.read(reinterpret_cast<char *>(buffer.data()), size)) {
        ERR("Failed to read input file");
    }

    AES256Encryptor encryptor(key);
    std::vector<uint8_t> encrypted = encryptor.encrypt(buffer,
                                                       const_cast<std::vector<uint8_t> &>(iv));

    std::ofstream out(output_path, std::ios::binary);
    if (!out.write(reinterpret_cast<const char *>(encrypted.data()), encrypted.size())) {
        ERR("Invalid or empty encrypted file");
    }

    return true;
}

bool talecrypt::AES256Encryptor::decrypt_file(const std::string &input_path,
                                              const std::string &output_path,
                                              const std::vector<uint8_t> &key,
                                              const std::vector<uint8_t> &iv) {
    std::ifstream in(input_path, std::ios::binary | std::ios::ate);
    if (!in.is_open()) {
        ERR("Failed to open encrypted file");

    }

    std::streamsize size = in.tellg();
    in.seekg(0, std::ios::beg);
    std::vector<uint8_t> encrypted(size);
    if (!in.read(reinterpret_cast<char *>(encrypted.data()), size)) {
        ERR("Failed to read encrypted file");
    }

    AES256Encryptor decryptor(key);
    std::vector<uint8_t> decrypted;
    decrypted = decryptor.decrypt(encrypted, iv);

    std::ofstream out(output_path, std::ios::binary);
    if (!out.write(reinterpret_cast<const char *>(decrypted.data()), decrypted.size())) {
        ERR("Failed to write decrypted data");
    }

    return true;
}



//std::vector<uint8_t> talecrypt::AES256Encryptor::hex_to_bytes(const char *hex) {
//    size_t len = std::strlen(hex);
//    if (len % 2 != 0) {
//        ERR("Invalid hex length");
//        return {};
//    }
//    std::vector<uint8_t> bytes(len / 2);
//    auto hex_char_to_nibble = [](char c) -> uint8_t {
//        if ('0' <= c && c <= '9') return c - '0';
//        if ('a' <= c && c <= 'f') return c - 'a' + 10;
//        if ('A' <= c && c <= 'F') return c - 'A' + 10;
//        ERR("Invalid hex character");
//        return 0;
//    };
//    for (size_t i = 0; i < bytes.size(); ++i) {
//        bytes[i] = (hex_char_to_nibble(hex[2 * i]) << 4) | hex_char_to_nibble(hex[2 * i + 1]);
//    }
//    return bytes;
//}


std::vector<uint8_t> talecrypt::AES256Encryptor::hex_to_bytes(const std::string &hex) {
    std::vector<uint8_t> bytes;
    bytes.reserve(hex.length() / 2);
    for (size_t i = 0; i < hex.length(); i += 2) {
        uint8_t byte = static_cast<uint8_t>(std::stoi(hex.substr(i, 2), nullptr, 16));
        bytes.push_back(byte);
    }
    return bytes;
}


std::string talecrypt::AES256Encryptor::to_hex_string(const std::vector<uint8_t> &data) {
    std::ostringstream oss;
    for (uint8_t byte: data) {
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
    }
    return oss.str();
}

uint8_t talecrypt::AES256Encryptor::hex_char_to_nibble(char c) {
    if ('0' <= c && c <= '9') return c - '0';
    if ('a' <= c && c <= 'f') return c - 'a' + 10;
    if ('A' <= c && c <= 'F') return c - 'A' + 10;
    ERR("Invalid hex character");
    return 0; // unreachable, but silences compiler warnings
}

std::vector<uint8_t> talecrypt::AES256Encryptor::generate_key() {
    std::vector<uint8_t> key(32);
    if (!RAND_bytes(key.data(), (int) key.size())) {
        ERR("Failed to generate key");
    }
    return key;
}

talecrypt::AES256Encryptor::AES256Encryptor(const std::vector<uint8_t> &key) {
    if (key.size() != 32) ERR("Key must be 32 bytes");
    key_ = key;
}

talecrypt::AES256Encryptor::AES256Encryptor(const std::string &hex_key) {
    auto key_bytes = hex_to_bytes(hex_key.c_str());
    if (key_bytes.size() != 32) ERR("Hex key must decode to 32 bytes");
    key_ = std::move(key_bytes);
}

std::vector<uint8_t>
talecrypt::AES256Encryptor::encrypt(const std::vector<uint8_t> &plaintext,
                                    std::vector<uint8_t> &iv) const {
    if (iv.size() != 16) ERR("IV must be exactly 16 bytes");

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) ERR("Failed to create cipher context");

    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key_.data(), iv.data()))
        ERR("EncryptInit failed");

    std::vector<uint8_t> ciphertext(plaintext.size() + 16);
    int len = 0, ciphertext_len = 0;

    if (1 !=
        EVP_EncryptUpdate(ctx, ciphertext.data(), &len, plaintext.data(), (int) plaintext.size()))
        ERR("EncryptUpdate failed");
    ciphertext_len = len;

    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &len))
        ERR("EncryptFinal failed");
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);
    ciphertext.resize(ciphertext_len);

    return ciphertext;
}

std::vector<uint8_t>
talecrypt::AES256Encryptor::decrypt(const std::vector<uint8_t> &ciphertext,
                                    const std::vector<uint8_t> &iv) const {
    if (iv.size() != 16) ERR("IV must be 16 bytes");

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) ERR("Failed to create cipher context");

    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key_.data(), iv.data()))
        ERR("DecryptInit failed");

    std::vector<uint8_t> plaintext(ciphertext.size());
    int len = 0, plaintext_len = 0;

    if (1 !=
        EVP_DecryptUpdate(ctx, plaintext.data(), &len, ciphertext.data(), (int) ciphertext.size()))
        ERR("DecryptUpdate failed");
    plaintext_len = len;

    if (1 != EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len))
        ERR("DecryptFinal failed");
    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);
    plaintext.resize(plaintext_len);

    return plaintext;
}

std::vector<uint8_t>
talecrypt::AES256Encryptor::encrypt_string(const std::string &plaintext,
                                           std::vector<uint8_t> &iv) const {
    return encrypt(std::vector<uint8_t>(plaintext.begin(), plaintext.end()), iv);
}

std::string talecrypt::AES256Encryptor::encrypt_string_to_hex(const std::string &plaintext,
                                                              std::vector<uint8_t> &iv) const {
    std::vector<uint8_t> plain_bytes(plaintext.begin(), plaintext.end());
    std::vector<uint8_t> encrypted_bytes = encrypt(plain_bytes, iv);
    return to_hex_string(encrypted_bytes);
}

std::string
talecrypt::AES256Encryptor::decrypt_string(const std::vector<uint8_t> &ciphertext,
                                           const std::vector<uint8_t> &iv) const {
    auto plaintext_bytes = decrypt(ciphertext, iv);
    return {plaintext_bytes.begin(), plaintext_bytes.end()};
}

//! [filebased]
bool talecrypt::AES256Encryptor::encrypt_to_file(const std::vector<uint8_t> &plaintext,
                                                 const std::string &filepath,
                                                 std::vector<uint8_t> &iv) const {
    std::vector<uint8_t> encrypted = encrypt(plaintext, iv);

    std::ofstream out(filepath, std::ios::binary);
    if (!out.is_open()) return false;

    out.write(reinterpret_cast<const char *>(encrypted.data()), encrypted.size());
    return out.good();
}

std::vector<uint8_t> talecrypt::AES256Encryptor::decrypt_from_file(const std::string &filepath,
                                                                   const std::vector<uint8_t> &iv) const {
    std::ifstream in(filepath, std::ios::binary | std::ios::ate);
    if (!in.is_open()) ERR("Failed to open file for decryption");

    std::streamsize size = in.tellg();
    if (size <= 0) ERR("Invalid or empty encrypted file");

    in.seekg(0, std::ios::beg);
    std::vector<uint8_t> encrypted(size);
    if (!in.read(reinterpret_cast<char *>(encrypted.data()), size))
        ERR("Failed to read encrypted data");

    return decrypt(encrypted, iv);
}

std::string talecrypt::AES256Encryptor::decrypt_string_from_hex(const std::string &hex_ciphertext,
                                                                const std::vector<uint8_t> &iv) const {
    auto encrypted_bytes = hex_to_bytes(hex_ciphertext);
    auto decrypted_bytes = decrypt(encrypted_bytes, iv);
    return {decrypted_bytes.begin(), decrypted_bytes.end()};
}
