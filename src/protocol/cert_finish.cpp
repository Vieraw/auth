

#include "cert_finish.h"
#include "../auth_client.h"

namespace protocol {
    factory_register<cert_finish> cert_finish::_add{548};

    void cert_finish::read(util::bytestream &data) {}

    void cert_finish::write() {
        util::bytestream body;
        body << retcode;
        body << reserved1;
        getClient()->write(548, body);
    }

}