#include "arc4.h"

namespace security {
    arc4::arc4(const std::string &key) {
        setKey(key);
    }

    void arc4::setKey(const std::string &key) {
        indexA = indexB = 0;
        for (int i = 0; i < 256; i++) {
            block[i] = static_cast<unsigned char>(i);
        }

        int j = 0;
        for (int i = 0; i < 256; i++) {
            j = (j + block[i] + static_cast<uint8_t>(key[i % key.length()])) % 256;
            std::swap(block[i], block[j]);
        }
    }

    std::string arc4::encrypt(const std::string &in) {
        std::string result(in.length(), '0');
        for (size_t k = 0; k < in.length(); ++k) {
            indexA = (indexA + 1) % 256;
            indexB = (indexB + block[indexA]) % 256;
            std::swap(block[indexA], block[indexB]);
            result[k] = block[(block[indexA] + block[indexB]) % 256] ^ static_cast<uint8_t>(in[k]);
        }
        return result;
    }
}