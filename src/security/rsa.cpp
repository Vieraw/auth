

#include "rsa.h"

namespace security {

    rsa::rsa(const std::string &key) {
        setKey(key);
    }

    void rsa::setKey(const std::string &filename) {
        FILE *fp = fopen(filename.c_str(), "r");
        if (fp == nullptr) {
            throw rsa_error("RSA error: key file not found in: " + filename);
        }
        _private_key = PEM_read_RSAPrivateKey(fp, nullptr, nullptr, nullptr);
        _key_size = RSA_size(_private_key);
        fclose(fp);
    }

    std::string rsa::decrypt(const std::string &str) {
        if (_private_key == nullptr) {
            throw rsa_error("RSA error: key is nullptr.");
        }

        std::shared_ptr<uint8_t> rsa_in{new uint8_t[str.length() + 1]};
        std::shared_ptr<uint8_t> rsa_out{new uint8_t[_key_size]};

        std::copy(str.begin(), str.end(), rsa_in.get());

        int len = RSA_private_decrypt(str.length(), rsa_in.get(), rsa_out.get(),
                                      _private_key, RSA_PKCS1_PADDING);

        if (len == -1) {
            throw rsa_error("RSA error: decrypt error.");
        }

        return std::string(reinterpret_cast<char *>(rsa_out.get()), len);
    }
}