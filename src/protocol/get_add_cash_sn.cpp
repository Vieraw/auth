

#include "../auth_client.h"
#include "add_cash.h"
#include "get_add_cash_sn.h"

namespace protocol {
    static factory_register<get_add_cash_sn> _add{514};

    void get_add_cash_sn::read(util::bytestream &data) {
        data >> session;
        data >> retcode;
        data >> userid;
        data >> zoneid;
        data >> sn;

        auto gold = registry::getDatabase().use_cash_now(userid, zoneid);

        if (gold["userid"]) {
            add_cash protocol{getClient()};
            protocol.session = session;
            protocol.userid = userid;
            protocol.zoneid = zoneid;
            protocol.cash = gold["cash"];
            protocol.sn = sn;
            protocol.write();
        }
    }

    void get_add_cash_sn::write() {
        util::bytestream body;
        body << session;
        body << userid;
        body << zoneid;

        getClient()->write(514, body);
    }
}