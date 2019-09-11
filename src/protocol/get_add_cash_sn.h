

#ifndef AUTHD_DEV_GET_ADD_CASH_SN_H
#define AUTHD_DEV_GET_ADD_CASH_SN_H


#include "base.h"

namespace protocol {
    class get_add_cash_sn : public base {
        static factory_register <get_add_cash_sn> _add;

    public:
        explicit get_add_cash_sn(const std::shared_ptr<auth_client> &client) : base(client) {}

        void read(util::bytestream &data) override;

        void write() override;

        BigEndian<uint32_t> retcode = -1;
        BigEndian<uint32_t> userid = -1;
        BigEndian<uint32_t> zoneid = -1;
        BigEndian<uint32_t> sn = 0;

    };
}

#endif //AUTHD_DEV_GET_ADD_CASH_SN_H
