

#ifndef AUTHD_DEV_RSA_H
#define AUTHD_DEV_RSA_H

#include <memory>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <string>

namespace security {

    class rsa_error : public std::exception {
        std::string error;
    public:

        explicit rsa_error(std::string &&s) : std::exception(), error(s) {}

        ~rsa_error() override = default;

        const char *what() const noexcept override { return error.c_str(); }
    };

    class rsa {
        RSA *_private_key = nullptr;
        uint32_t _key_size = 0;

    public:
        rsa() = default;

        explicit rsa(const std::string &key);

        void setKey(const std::string &filename);

        std::string decrypt(const std::string &str);
    };
}

#endif //AUTHD_DEV_RSA_H
