

#ifndef AUTHD_DEV_KICKOUT_USER_H
#define AUTHD_DEV_KICKOUT_USER_H

#include "base.h"

namespace protocol {
    class kickout_user : public base {
        static factory_register <kickout_user> _add;

    public:
        explicit kickout_user(const std::shared_ptr<auth_client> &client) : base(client) {}

        void read(util::bytestream &data) override;

        void write() override;

        BigEndian<uint32_t> userid = -1;
        BigEndian<uint32_t> localsid = 0;
        BigEndian<uint32_t> cause = 32;

    };
}


#endif //AUTHD_DEV_KICKOUT_USER_H
