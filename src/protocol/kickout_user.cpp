

#include "../auth_client.h"
#include "kickout_user.h"

namespace protocol {
    static factory_register<kickout_user> _add{10};

    void kickout_user::read(util::bytestream &data) {}

    void kickout_user::write() {
        util::bytestream body;
        body << userid;
        body << localsid;
        body << cause;

        getClient()->write(10, body);

        std::cout << "KickoutUser userid: " << userid
                  << " localsid: " << localsid
                  << " cause: " << cause << std::endl;
    }
}