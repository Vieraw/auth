

#include "../auth_client.h"
#include "query_user_privilege.h"

namespace protocol {

    factory_register<query_user_privilege> query_user_privilege::_add{524};

    void query_user_privilege::read(util::bytestream &data) {
        data >> userid;
        data >> zoneid;

        write();
    }

    void query_user_privilege::write() {
        util::bytestream body;
        body << userid;
        std::vector<uint8_t> privileges = registry::getDatabase().user_privileges(userid, zoneid);
        body << util::WriteCUInt(privileges.size());
        for (auto &it: privileges) {
            body << it;
        }

        getClient()->write(507, body);

        std::cout << "QueryUserPrivilege userid: " << userid
                  << " count: " << privileges.size() << std::endl;
    }
}