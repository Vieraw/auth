

#ifndef AUTHD_DEV_CERT_ANSWER_H
#define AUTHD_DEV_CERT_ANSWER_H

#include "base.h"

namespace protocol {
    class cert_answer : public base {
        static factory_register<cert_answer> _add;

    public:
        explicit cert_answer(const std::shared_ptr<auth_client> &client) : base(client) {}

        void read(util::bytestream &data) override;

        void write() override;

        BigEndian<uint32_t> reserved1 = 0;
    };
}


#endif //AUTHD_DEV_CERT_ANSWER_H
