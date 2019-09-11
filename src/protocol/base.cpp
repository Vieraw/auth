


#include "base.h"
#include "../auth_client.h"

namespace protocol {

    void base::setSession() {
        session = session & 0x3FFFFFFFu;
    }

    void base::setClient(std::shared_ptr<auth_client> &client) {
        _client = client;
    }

    std::shared_ptr<auth_client> &base::getClient() {
        if (_client == nullptr) {
            throw protocol_error("Protocol error: client is nullptr.");
        }
        return _client;
    }

}