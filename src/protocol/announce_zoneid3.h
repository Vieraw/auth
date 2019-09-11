

#ifndef AUTHD_DEV_ANNOUNCE_ZONEID3_H
#define AUTHD_DEV_ANNOUNCE_ZONEID3_H

#include "base.h"

namespace protocol {
    class announce_zoneid3 : public base {
        static factory_register<announce_zoneid3> _add;

    public:
        explicit announce_zoneid3(const std::shared_ptr<auth_client> &client) : base(client) {}

        void read(util::bytestream &data) override;

        void write() override;


        BigEndian<uint32_t> zoneid = 0;
        BigEndian<uint32_t> aid = 0;
        uint8_t blreset = 0;
        net::ip ip1{};
        net::ip ip2{};
        net::ip ip3{};
        BigEndian<uint32_t> version = 0;
        BigEndian<uint32_t> reserved = 0;
    };
}

#endif //AUTHD_DEV_ANNOUNCE_ZONEID3_H
