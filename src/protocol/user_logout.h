

#ifndef AUTHD_DEV_USER_LOGOUT_H
#define AUTHD_DEV_USER_LOGOUT_H


#include "base.h"

namespace protocol {
    class user_logout : public base {
        static factory_register <user_logout> _add;

    public:
        void read(util::bytestream &data) override;

        explicit user_logout(const std::shared_ptr<auth_client> &client) : base(client) {}

        void write() override;

        BigEndian<uint32_t> userid = -1;
        BigEndian<uint32_t> localsid = 0;
        uint8_t retcode = 0;
    };
}

#endif //AUTHD_DEV_USER_LOGOUT_H
