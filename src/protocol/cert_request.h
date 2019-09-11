

#ifndef AUTHD_DEV_CERT_REQUEST_H
#define AUTHD_DEV_CERT_REQUEST_H

#include "base.h"

namespace protocol {
    class cert_request : public base {
        static factory_register<cert_request> _add;

    public:
        explicit cert_request(const std::shared_ptr<auth_client> &client) : base(client) {}

        void read(util::bytestream &data) override;

        void write() override;

        BigEndian<uint32_t> reserved1;
    };
}

#endif //AUTHD_DEV_CERT_REQUEST_H