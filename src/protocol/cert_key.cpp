

#include "../auth_client.h"
#include "cert_key.h"
#include "cert_finish.h"

namespace protocol {

    factory_register<cert_key> cert_key::_add{547};

    void cert_key::read(util::bytestream &data) {
        data >> d_key1_encrypt;
        data >> d_key2_encrypt;
        data >> reserved1;

        d_key1_encrypt = registry::getRSA().decrypt(d_key1_encrypt);
        d_key2_encrypt = registry::getRSA().decrypt(d_key2_encrypt);

        getClient()->setEncryptor(d_key2_encrypt, d_key1_encrypt);

        cert_finish protocol{getClient()};
        protocol.write();
    }

    void cert_key::write() {}
}
