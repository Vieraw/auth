

#include "announce_zoneid3.h"
#include "../auth_client.h"

namespace protocol {
    factory_register<announce_zoneid3> announce_zoneid3::_add{527};

    void announce_zoneid3::read(util::bytestream &data) {
        data >> zoneid;
        data >> aid;
        data >> blreset;
        data >> ip1;
        data >> ip2;
        data >> ip3;
        data >> version;
        data >> reserved;

        getClient()->setZoneId(zoneid);
        getClient()->setAid(aid);
        getClient()->setVersion(version);

        registry::getDatabase().clear_online(zoneid, aid);

        std::cout << "AnnounceZoneid aid: " << getClient()->getAid()
                  << " zoneid: " << getClient()->getZoneId()
                  << " version: " << getClient()->getVersion() << std::endl;
    }

    void announce_zoneid3::write() {}
}