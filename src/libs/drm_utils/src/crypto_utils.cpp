//
//  crypto_utils.cpp
//  vividas_drm
//
//  Created by Alejandro Trejo on 3/22/19.
//

#include <openssl/evp.h>
#include <drm_utils.h>

using EVP_MD_CTX_free_ptr = std::unique_ptr<EVP_MD_CTX, decltype(&::EVP_MD_CTX_free)>;
using EVP_CIPHER_CTX_free_ptr = std::unique_ptr<EVP_CIPHER_CTX, decltype(&::EVP_CIPHER_CTX_free)>;
using byte = viv::drm_utils::byte;

std::array<byte, viv::drm_utils::sha256_hash_size> viv::drm_utils::sha256(const char* data, const int length) {
    byte hashed_data[viv::drm_utils::sha256_hash_size];
    unsigned int hashed_data_length = viv::drm_utils::sha256_hash_size;
    std::array<byte, viv::drm_utils::sha256_hash_size> hashed_data_to_return{};
    unsigned char result_code = 0;
    unsigned char total_asserts = 5;

    if (length <= 0) return hashed_data_to_return;

    EVP_MD_CTX_free_ptr digest_ctx(EVP_MD_CTX_new(), ::EVP_MD_CTX_free);
    result_code += digest_ctx.get() != nullptr;
    result_code += EVP_DigestInit_ex(digest_ctx.get(), EVP_sha256(), NULL);
    result_code += EVP_DigestUpdate(digest_ctx.get(), data, length);
    result_code += EVP_DigestFinal_ex(digest_ctx.get(), hashed_data, &hashed_data_length);
    result_code += hashed_data_length == viv::drm_utils::sha256_hash_size;

    if (result_code == total_asserts) std::memcpy(&hashed_data_to_return, &hashed_data, hashed_data_length);

    return hashed_data_to_return;
}

std::array<byte, viv::drm_utils::sha256_hash_size> viv::drm_utils::sha256(std::string& data) {
    return sha256(data.c_str(), data.length());
}

viv::drm_utils::aes_ccm_parameters::aes_ccm_parameters(const std::vector<byte>& nonce, const int tag_length) : nonce(nonce), tag_length(tag_length) {};

viv::drm_utils::aes_ccm_parameters::aes_ccm_parameters(const std::string& nonce_string, int tag_length) : nonce(std::vector<byte>(nonce_string.begin(), nonce_string.end())), tag_length(tag_length) {}

viv::drm_utils::aes_ccm_parameters::aes_ccm_parameters(const char* nonce_array, const int nonce_length, const int tag_length) : nonce(std::vector<byte>(nonce_array, nonce_array + nonce_length)), tag_length(tag_length) {}

std::vector<byte> viv::drm_utils::aes_ccm_decrypt(const std::vector<byte>& encrypted_data, const std::array<byte, aes_key_size>& key, const aes_ccm_parameters& parameters, const std::vector<byte>& associated_data) {
    EVP_CIPHER_CTX_free_ptr ctx(EVP_CIPHER_CTX_new(), ::EVP_CIPHER_CTX_free);
    int nonce_size = parameters.nonce.size();
    int encrypted_data_size = encrypted_data.size() - parameters.tag_length;
    int resulting_length;
    unsigned char result_code = 0;
    unsigned char total_asserts = 8;
    byte* tag = (byte *) encrypted_data.data() + encrypted_data_size;
    byte plain_text[encrypted_data_size];

    result_code += ctx.get() != nullptr;
    result_code += EVP_DecryptInit_ex(ctx.get(), EVP_aes_256_ccm(), NULL, NULL, NULL);
    result_code += EVP_CIPHER_CTX_ctrl(ctx.get(), EVP_CTRL_CCM_SET_IVLEN, nonce_size, NULL);
    result_code += EVP_CIPHER_CTX_ctrl(ctx.get(), EVP_CTRL_CCM_SET_TAG, parameters.tag_length, tag);
    result_code += EVP_DecryptInit_ex(ctx.get(), NULL, NULL, key.data(), parameters.nonce.data());
    result_code += EVP_DecryptUpdate(ctx.get(), NULL, &resulting_length, NULL, encrypted_data_size);
    result_code += EVP_DecryptUpdate(ctx.get(), NULL, &resulting_length, associated_data.data(), associated_data.size());
    result_code += EVP_DecryptUpdate(ctx.get(), plain_text, &resulting_length, encrypted_data.data(), encrypted_data_size);

    if (result_code != total_asserts) return std::vector<byte>(0, 0);
    return std::vector<byte>(plain_text, plain_text + resulting_length);
}

std::vector<byte> viv::drm_utils::aes_ccm_decrypt(const std::string& encrypted_data, const std::array<byte, aes_key_size>& key, const aes_ccm_parameters& parameters, const std::vector<byte>& associated_data) {
    std::vector<byte> encrypted_data_vector(encrypted_data.begin(), encrypted_data.end());
    return aes_ccm_decrypt(encrypted_data_vector, key, parameters, associated_data);
}

std::vector<byte> viv::drm_utils::aes_ccm_decrypt(const char* encrypted_data, const int encrypted_data_length, const std::array<byte, aes_key_size>& key, const aes_ccm_parameters& parameters, const std::vector<byte>& associated_data) {
    std::vector<byte> encrypted_data_vector(encrypted_data, encrypted_data + encrypted_data_length);
    return aes_ccm_decrypt(encrypted_data_vector, key, parameters, associated_data);
}

template <typename T>
std::array<byte, sizeof(T)> viv::drm_utils::get_bytes(const T value) {
    const unsigned char size = sizeof value;
    std::array<byte, size> value_bytes;
    
    for (int i = 0; i < size; i +=1)
    {
        value_bytes[size - i - 1] = *(((byte*) &value) + i);
    }
    
    return value_bytes;
}

viv::drm_utils::aes_cbc_parameters::aes_cbc_parameters(const std::array<byte, aes_block_size>& iv_salt, const std::array<byte, aes_key_size>& session_key, const uint64_t request_time) {
    std::array<byte, bytes_in_uint64_t> request_time_bytes = get_bytes(request_time);
    std::array<byte, cbc_to_hash_length> to_hash;

    std::memcpy(&to_hash, &iv_salt, aes_block_size);
    std::memcpy(to_hash.data() + aes_block_size, &request_time_bytes, bytes_in_uint64_t);

    std::array<byte, sha256_hash_size>  hash = sha256((const char*) to_hash.data(), cbc_to_hash_length);
    std::memcpy(&this->iv, &hash, aes_block_size);

    long time_in_seconds = request_time / 1000;
    unsigned char odd = (unsigned char) (time_in_seconds & 1);

    std::memcpy(&this->key, &session_key, aes_key_size);
    for (int i = 0; i < aes_block_size; i += 1)
    {
        this->key[(2 * i) + odd] ^= hash[i + aes_block_size];
    }
}

viv::drm_utils::aes_decryption_parameters::aes_decryption_parameters(const std::array<byte, aes_block_size>& iv_salt, const std::array<byte, aes_key_size>& session_key, const uint64_t request_time, const uint64_t block_index) {
    std::array<byte, ccm_to_hash_length> to_hash;
    std::array<byte, bytes_in_uint64_t> request_time_bytes = get_bytes(request_time);
    std::array<byte, bytes_in_uint64_t> block_index_bytes = get_bytes(block_index);

    std::memcpy(&to_hash, &iv_salt, aes_block_size);
    std::memcpy(to_hash.data() + aes_block_size, &request_time_bytes, bytes_in_uint64_t);
    std::memcpy(to_hash.data() + ccm_to_hash_length - bytes_in_uint64_t, &block_index_bytes, bytes_in_uint64_t);

    std::array<byte, sha256_hash_size> hash = sha256((const char*) to_hash.data(), to_hash.size());
    
    std::vector<byte> nonce(decryption_nonce_size);
    std::memcpy(nonce.data(), &hash, decryption_nonce_size);
    this->ccm_parameters = aes_ccm_parameters(nonce, decryption_tag_size);

    std::memcpy(this->associated_data.data(), hash.data() + decryption_nonce_size, decryption_aad_size);
    std::memcpy(&this->key, &session_key, aes_key_size);
    long time_in_seconds = request_time / 1000;
    unsigned char odd = (unsigned char) (time_in_seconds & 1);

    for (int i = 0; i < aes_block_size; i += 1)
    {
        this->key[(2 * i) + odd] ^= hash[i + decryption_nonce_size + decryption_aad_size];
        
    }
}
