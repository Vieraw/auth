

#include "../auth_client.h"
#include "user_login.h"
#include "kickout_user.h"


namespace protocol {
    static factory_register<user_login> _add{15};

    void user_login::read(util::bytestream &data) {
        data >> session;
        data >> userid;
        data >> localsid;
        data >> blkickuser;
        data >> freecreatime;
        data >> loginip;
        data >> account;
        data >> iseckey;
        data >> oseckey;

        BigEndian<uint32_t> zoneid = getClient()->getZoneId();
        BigEndian<uint32_t> zonelocalsid = localsid;
        BigEndian<uint32_t> overwrite = blkickuser;

        registry::getDatabase().record_online(userid, getClient()->getAid(), zoneid, zonelocalsid, overwrite);
        creatime = registry::getDatabase().user_creatime(userid);

        if (getClient()->getZoneId() != zoneid || zonelocalsid != localsid) {
            retcode = 10;
        }

        if (retcode == 0 && blkickuser == 1) {
            protocol::kickout_user protocol{getClient()};
            protocol.userid = userid;
            protocol.localsid = localsid;
            protocol.write();
        }

        write();
    }

    void user_login::write() {
        setSession();
        util::bytestream body;

        std::vector<uint8_t> privileges = registry::getDatabase().user_privileges(userid, getClient()->getZoneId());
        blIsGM = privileges.empty() ? 0 : 1;

        body << session;
        body << retcode;
        body << remain_playtime;
        body << func;
        body << funcparm;
        body << blIsGM;
        body << free_time_left;
        body << free_time_end;
        body << creatime;
        body << adduppoint;
        body << soldpoint;

        getClient()->write(15, body);

        if (retcode == 0) {
            std::cout << "UserLogin account: " << account
                      << " blIsGM: " << (blIsGM ? "true" : "false")
                      << " loginip: " << loginip
                      << " localsid: " << localsid
                      << " blkickuser: " << (blkickuser ? "true" : "false")
                      << " freecreatime: " << freecreatime << std::endl;
        }
    }
}
