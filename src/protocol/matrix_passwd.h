//
// Created by php on 14-Aug-19.
//

#ifndef AUTHD_DEV_MATRIX_PASSWD_H
#define AUTHD_DEV_MATRIX_PASSWD_H

#include <regex>

#include "base.h"
#include "../util/encoding.h"

namespace protocol {
    class matrix_passwd : public base {
        static factory_register <matrix_passwd> _add;

    public:
        explicit matrix_passwd(const std::shared_ptr<auth_client> &client) : base(client) {}

        void read(util::bytestream &data) override;

        void write() override;

        int check_ip(const BigEndian<uint32_t> &_userid, const LittleEndian<uint32_t> &ip);

        //arg
        util::octet account{};
        util::octet challenge{};
        net::ip loginip;

        //res
        BigEndian<uint32_t> retcode = 2;
        BigEndian<uint32_t> userid = 0;
        BigEndian<uint32_t> algorithm = 0;
        util::octet response{};
        util::octet matrix{};
    };
}


#endif //AUTHD_DEV_MATRIX_PASSWD_H
