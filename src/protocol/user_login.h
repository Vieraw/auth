

#ifndef AUTH_DEV_USER_LOGIN_H
#define AUTH_DEV_USER_LOGIN_H

#include "base.h"

namespace protocol {
    class user_login : public base {
        static factory_register <user_login> _add;

    public:
        explicit user_login(const std::shared_ptr<auth_client> &client) : base(client) {}

        void read(util::bytestream &data) override;

        void write() override;

        //arg
        BigEndian<uint32_t> userid = -1;
        BigEndian<uint32_t> localsid = 0;
        uint8_t blkickuser = 0;
        BigEndian<uint32_t> freecreatime = 0;
        net::ip loginip{};
        util::octet account{};
        util::octet iseckey{};
        util::octet oseckey{};

        //res

        uint8_t retcode = 0;
        BigEndian<uint32_t> remain_playtime = 0;
        BigEndian<uint32_t> func = 0;
        BigEndian<uint32_t> funcparm = 0;
        uint8_t blIsGM = 0;
        BigEndian<uint32_t> free_time_left = 0;
        BigEndian<uint32_t> free_time_end = 0;
        BigEndian<uint32_t> creatime = 0;
        BigEndian<uint32_t> adduppoint = 0;
        BigEndian<uint32_t> soldpoint = 0;
    };
}

#endif //AUTH_DEV_USER_LOGIN_H
