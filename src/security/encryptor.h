

#ifndef AUTHD_DEV_ENCRYPTOR_H
#define AUTHD_DEV_ENCRYPTOR_H

#include <memory>
#include "arc4.h"

namespace security {
    class encryptor {
        std::unique_ptr<arc4> _encrypt;
        std::unique_ptr<arc4> _decrypt;

    public:
        encryptor() = default;

        encryptor(arc4 *encrypt, arc4 *decrypt) : _encrypt(encrypt), _decrypt(decrypt) {}

        void setEncrypt(arc4 *encrypt);

        void setDecrypt(arc4 *decrypt);

        std::string encrypt(const std::string &data);

        std::string decrypt(const std::string &data);
    };

}
#endif //AUTHD_DEV_ENCRYPTOR_H
