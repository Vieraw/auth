

#include "encryptor.h"

namespace security {

    void encryptor::setEncrypt(arc4 *encrypt) {
        _encrypt = std::unique_ptr<arc4>(encrypt);
    }

    void encryptor::setDecrypt(arc4 *decrypt) {
        _decrypt = std::unique_ptr<arc4>(decrypt);
    }

    std::string encryptor::encrypt(const std::string &data) {
        if (_encrypt == nullptr) {
            return data;
        }
        return _encrypt->encrypt(data);
    }

    std::string encryptor::decrypt(const std::string &data) {
        if (_decrypt == nullptr) {
            return data;
        }
        return _decrypt->encrypt(data);
    }

}