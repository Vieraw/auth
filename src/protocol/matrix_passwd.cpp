

#include "../auth_client.h"
#include "query_user_forbid.h"
#include "matrix_passwd.h"

#include <iostream>

namespace protocol {

    factory_register<matrix_passwd> matrix_passwd::_add{550};

    void matrix_passwd::read(util::bytestream &data) {
        data >> session;
        data >> account;
        data >> challenge;
        data >> loginip;

        account = convert_encoding(account, "UTF-8", "ISO-8859-1");

        std::cout << "MatrixPasswd: account: " << account
                  << " loginip: " << loginip << std::endl;

        if (account.length() > 64) {
            std::cerr << "Hacking attempt from ip: " << loginip << std::endl;
        } else if (!std::regex_match(account, std::regex("[a-z0-9_-]+"))) {
            std::cerr << "Bad symbol detected, login failed from ip:" << loginip << std::endl;
        } else {
            registry::getDatabase().user_password(account, userid, response);
        }

        if (userid > 0) {
            response = hex_to_bytes(response.substr(2));
            retcode = 0;

            std::vector<forbid> forbids = registry::getDatabase().user_forbid(userid);
            query_user_forbid protocol{getClient()};
            protocol.userid = userid;
            protocol.list_type = 0;

            for (auto &it: forbids) {
                protocol.forbids.emplace_back(static_cast<uint8_t>(it.type), it.forbid_time, it.ctime, it.reason);
            }

            int lock_status = check_ip(userid, static_cast<uint32_t>(loginip));
            if (lock_status) {
                protocol.forbids.emplace_back(100, 31536000, std::time(nullptr),
                                              (lock_status == 2 ? registry::getConfig()["IPLimitMessage"]["ip"]
                                                                : registry::getConfig()["IPLimitMessage"]["lock"]));

                std::cout << "Access to account with id: " << userid << " from ip: " << loginip << " is denied"
                          << std::endl;
            }
            protocol.write();
        }

        write();
    }

    void matrix_passwd::write() {
        setSession();

        util::bytestream body;

        body << session;
        body << retcode;
        body << userid;
        body << algorithm;
        body << response;
        body << matrix;

        getClient()->write(550, body);
    }

    int matrix_passwd::check_ip(const BigEndian<uint32_t> &_userid, const LittleEndian<uint32_t> &ip) {
        ip_limit limit = registry::getDatabase().check_ip_limit(_userid);

        if (limit.lockstatus == "t") {
            return 1;
        }

        if (limit.enable == "f") {
            return 0;
        }

        bool flag = false;

        for (auto &it: limit.ips) {
            if (it["ip"] != 0 && (it["mask"] > 0 && it["mask"] <= 32)) {
                uint32_t offset = 32 - it["mask"];
                flag = true;

                if ((ip & -1u << offset) == (it["ip"] & -1u << offset)) {
                    return 0;
                }
            }
        }
        return flag ? 2 : 0;
    }
}