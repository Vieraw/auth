

#include "../auth_client.h"
#include "user_logout.h"

namespace protocol {
    static factory_register<user_logout> _add{33};

    void user_logout::read(util::bytestream &data) {
        data >> session;
        data >> userid;
        data >> localsid;

        BigEndian<uint32_t> zoneid = getClient()->getZoneId();
        BigEndian<uint32_t> zonelocalsid = localsid;
        BigEndian<uint32_t> overwrite = 1;

        registry::getDatabase().record_offline(userid, getClient()->getAid(), zoneid, zonelocalsid, overwrite);

        if (overwrite != 1) {
            retcode = 12;
        }
        write();
    }

    void user_logout::write() {
        setSession();
        util::bytestream body;
        body << session;
        body << retcode;

        getClient()->write(33, body);

        std::cout << "UserLogout retcode: " << static_cast<uint16_t>(retcode)
                  << " userid: " << userid << std::endl;
    }
}