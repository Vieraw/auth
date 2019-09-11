

#include "../auth_client.h"
#include "game_2_au.h"

namespace protocol {
    static factory_register<game_2_au> _add{8039};

    void game_2_au::read(util::bytestream &data) {
        data >> userid;
        data >> qtype;
        data >> info;
        data >> reserved;

        std::cout << "Game2AU userid: " << userid
                  << " qtype: " << qtype
                  << " info: " << info
                  << " reserved: " << reserved << std::endl;
    }

    void game_2_au::write() {}
}