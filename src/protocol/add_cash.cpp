

#include "../auth_client.h"
#include "add_cash.h"

namespace protocol {
    static factory_register<add_cash> _arg{515};
    static factory_register<add_cash> _res{516};

    void add_cash::read(util::bytestream &data) {
        data >> retcode;
        data >> userid;
        data >> zoneid;
        data >> sn;
    }

    void add_cash::write() {
        util::bytestream body;
        body << userid;
        body << zoneid;
        body << sn;
        body << cash;

        getClient()->write(515, body);

        std::cout << "AddCash userid: " << userid
                  << " zoneid: " << zoneid
                  << " sn: " << sn
                  << " cash: " << cash << std::endl;
    }
}