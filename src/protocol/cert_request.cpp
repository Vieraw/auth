

#include "cert_request.h"
#include "cert_answer.h"
#include "../auth_client.h"

namespace protocol {

    factory_register<cert_request> cert_request::_add{545};

    void cert_request::read(util::bytestream &data) {
        data >> reserved1;

        cert_answer protocol{getClient()};
        protocol.write();
    }

    void cert_request::write() {}

}