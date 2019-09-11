

#ifndef AUTHD_DEV_CERT_FINISH_H
#define AUTHD_DEV_CERT_FINISH_H

#include "base.h"

namespace protocol {
    class cert_finish : public base {
        static factory_register<cert_finish> _add;

    public:
        explicit cert_finish(const std::shared_ptr<auth_client> &client) : base(client) {}

        void read(util::bytestream &data) override;

        void write() override;

        BigEndian<uint32_t> retcode = 0;
        BigEndian<uint32_t> reserved1 = 0;
    };
}

#endif //AUTHD_DEV_CERT_FINISH_H
