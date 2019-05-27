//
//  drm_utils.h
//  vividas_drm_utils
//
//  Created by Yamil Molinar on 3/22/19.
//

#ifndef VIVIDAS_DRM_UTILS_H
#define VIVIDAS_DRM_UTILS_H

#include <string>
#include <array>
#include <vector>
#include <sstream>

namespace viv {
    namespace drm_utils {

        typedef unsigned char byte;
        static const unsigned int sha256_hash_size = 32;
        static const unsigned int aes_key_size = 32;
        static const unsigned int aes_block_size = 16;
        static const unsigned char bytes_in_uint64_t = 8;
        static const unsigned char bytes_in_uint32_t = 4;
        static const int cbc_to_hash_length = aes_block_size + bytes_in_uint64_t;
        
        static const unsigned int decryption_nonce_size = 8;
        static const unsigned int decryption_aad_size = 8;
        static const unsigned int decryption_tag_size = 8;
        static const unsigned int ccm_to_hash_length = aes_block_size + (bytes_in_uint64_t * 2);

        typedef struct aes_ccm_parameters {
            std::vector<byte> nonce;
            int tag_length;
            aes_ccm_parameters() {};
            aes_ccm_parameters(const std::vector<byte>& nonce, const int tag_length);
            aes_ccm_parameters(const std::string& nonce, const int tag_length);
            aes_ccm_parameters(const char* nonce_array, const int nonce_length, const int tag_length);
        } aes_ccm_parameters;
        
        typedef struct aes_cbc_parameters {
            std::array<byte, aes_key_size> key;
            std::array<byte, aes_block_size> iv;
            aes_cbc_parameters(const std::array<byte, aes_block_size>& iv_salt, const std::array<byte, aes_key_size>& session_key, const uint64_t request_time);
        }aes_cbc_parameters;

        typedef struct aes_decryption_parameters {
            aes_ccm_parameters ccm_parameters;
            std::array<byte, decryption_aad_size> associated_data;
            std::array<byte, aes_key_size> key;
            aes_decryption_parameters(const std::array<byte, aes_block_size>& iv_salt, const std::array<byte, aes_key_size>& session_key, const uint64_t request_time, const uint64_t block_index);
        } aes_decryption_parameters;

        std::array<byte, sha256_hash_size> sha256(const char* data, const int length);
        std::array<byte, sha256_hash_size> sha256(std::string& data);

        std::vector<byte> aes_ccm_decrypt(const std::vector<byte>& encrypted_data, const std::array<byte, aes_key_size>& key, const aes_ccm_parameters& parameters, const std::vector<byte>& associated_data);
        std::vector<byte> aes_ccm_decrypt(const std::string& encrypted_data, const std::array<byte, aes_key_size>& key, const aes_ccm_parameters& parameters, const std::vector<byte>& associated_data);
        std::vector<byte> aes_ccm_decrypt(const char* encrypted_data, const int encrypted_data_length, const std::array<byte, aes_key_size>& key, const aes_ccm_parameters& parameters, const std::vector<byte>& associated_data);

        template <typename T>
        std::array<byte, sizeof(T)> get_bytes(const T value);
    }
}

#endif // VIVIDAS_DRM_UTILS_H
