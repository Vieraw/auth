

#ifndef AUTHD_DEV_GAME_2_AU_H
#define AUTHD_DEV_GAME_2_AU_H

#include "base.h"

namespace protocol {
    class game_2_au : public base {
        static factory_register <game_2_au> _add;

    public:
        explicit game_2_au(const std::shared_ptr<auth_client> &client) : base(client) {}

        void read(util::bytestream &data) override;

        void write() override;

        BigEndian<uint32_t> userid = -1;
        BigEndian<uint32_t> qtype = 0;
        util::octet info{};
        BigEndian<uint32_t> reserved = 0;

    };
}


#endif //AUTHD_DEV_GAME_2_AU_H
