

#ifndef AUTHD_DEV_ADD_CASH_H
#define AUTHD_DEV_ADD_CASH_H

#include "base.h"

namespace protocol {
    class add_cash : public base {
        static factory_register <add_cash> _arg;
        static factory_register <add_cash> _res;

    public:
        explicit add_cash(const std::shared_ptr<auth_client> &client) : base(client) {}

        void read(util::bytestream &data) override;

        void write() override;

        BigEndian<uint32_t> retcode;
        BigEndian<uint32_t> userid;
        BigEndian<uint32_t> zoneid;
        BigEndian<uint32_t> sn;
        BigEndian<uint32_t> cash;
    };
}


#endif //AUTHD_DEV_ADD_CASH_H
