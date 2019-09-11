

#ifndef AUTHD_DEV_QUERY_USER_PRIVILEGE_H
#define AUTHD_DEV_QUERY_USER_PRIVILEGE_H

#include "base.h"

namespace protocol {
    class query_user_privilege : public base {
        static factory_register <query_user_privilege> _add;

    public:
        explicit query_user_privilege(const std::shared_ptr<auth_client> &client) : base(client) {}

        void read(util::bytestream &data) override;

        void write() override;

        BigEndian<uint32_t> userid;
        BigEndian<uint32_t> zoneid;
    };
}


#endif //AUTHD_DEV_QUERY_USER_PRIVILEGE_H
