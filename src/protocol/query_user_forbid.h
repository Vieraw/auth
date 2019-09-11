

#ifndef AUTH_DEV_QUERY_USER_FORBID_H
#define AUTH_DEV_QUERY_USER_FORBID_H

#include "base.h"
#include "../util/encoding.h"

namespace protocol {

    struct role_forbid {
        uint8_t type = 0;
        BigEndian<int32_t> time = 0;
        BigEndian<int32_t> createtime = 0;
        util::octet reason{};

        role_forbid() = default;

        role_forbid(const uint8_t _type, const BigEndian<int32_t> &_time, const BigEndian<int32_t> &_createtime,
                    util::octet _reason)
                : type(_type), time(_time), createtime(_createtime), reason(std::move(_reason)) {}

        void read(util::bytestream &data);

        void write(util::bytestream &data);
    };

    class query_user_forbid : public base {
        static factory_register <query_user_forbid> _add;

    public:
        explicit query_user_forbid(const std::shared_ptr<auth_client> &client) : base(client) {}

        void read(util::bytestream &data) override;

        void write() override;

        BigEndian<uint32_t> userid = 0;
        BigEndian<uint32_t> list_type = 1;
        std::vector<role_forbid> forbids;
    };
}


#endif //AUTH_DEV_QUERY_USER_FORBID_H
