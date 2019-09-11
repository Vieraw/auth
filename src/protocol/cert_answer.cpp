

#include "cert_answer.h"
#include "../auth_client.h"

namespace protocol {

    factory_register<cert_answer> cert_answer::_add{546};

    void cert_answer::write() {
        if (registry::getCert().empty()) {
            throw protocol_error("Protocol error: cert is empty.");
        }

        util::octet authd_cert = registry::getCert();

        util::bytestream body;
        body << authd_cert;
        body << reserved1;

        getClient()->write(546, body);
    }

    void cert_answer::read(util::bytestream &data) {}
}