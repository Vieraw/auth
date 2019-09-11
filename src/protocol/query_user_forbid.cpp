

#include "../auth_client.h"
#include "query_user_forbid.h"

namespace protocol {
    factory_register<query_user_forbid> query_user_forbid::_add(509);

    void role_forbid::read(util::bytestream &data) {
        data >> type;
        data >> time;
        data >> createtime;
        data >> reason;
    }

    void role_forbid::write(util::bytestream &data) {
        data << type;
        data << time;
        data << createtime;

        reason = convert_encoding(reason, "UTF-16LE");
        data << reason;
    }

    void query_user_forbid::read(util::bytestream &data) {}

    void query_user_forbid::write() {
        util::bytestream body;
        body << userid;
        body << list_type;
        body << util::WriteCUInt(forbids.size());
        for (auto &it: forbids) {
            it.write(body);
        }

        getClient()->write(509, body);
    }
}