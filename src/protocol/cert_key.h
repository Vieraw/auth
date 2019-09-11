

#ifndef AUTHD_DEV_CERT_KEY_H
#define AUTHD_DEV_CERT_KEY_H

#include "base.h"

namespace protocol {
    class cert_key : public base {
        static factory_register <cert_key> _add;

    public:
        explicit cert_key(const std::shared_ptr<auth_client> &client) : base(client) {}

        void read(util::bytestream &data) override;

        void write() override;

        util::octet d_key1_encrypt = util::octet();
        util::octet d_key2_encrypt = util::octet();
        BigEndian<uint32_t> reserved1 = 0;
    };

}

#endif //AUTHD_DEV_CERT_KEY_H
