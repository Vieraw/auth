

#include "database.h"

void database::connect(const std::string &host, const std::string &name, const std::string &password,
                       const std::string &db) {

    mariadb::account_ref acc = mariadb::account::create(host, name, password, db);
    _con = mariadb::connection::create(acc);
}

void database::clear_online(const BigEndian<uint32_t> &zoneid, const BigEndian<uint32_t> &aid) {
    std::string query = "CALL clearonlinerecords(?, ?)";
    mariadb::statement_ref stmt = _con->create_statement(query);
    stmt->set_unsigned32(0, zoneid);
    stmt->set_unsigned32(1, aid);
    stmt->execute();
}

void
database::record_online(const BigEndian<uint32_t> &uid, const BigEndian<uint32_t> &aid, BigEndian<uint32_t> &zoneid,
                        BigEndian<uint32_t> &zonelocalid, BigEndian<uint32_t> &overwrite) {
    std::string query = "CALL recordonline(?, ?, @zoneid, @zonelocalid, @overwrite)";
    std::string set = "SET @zoneid = ?, @zonelocalid = ?, @overwrite = ?";
    mariadb::statement_ref stmt = _con->create_statement(set);
    stmt->set_unsigned32(0, zoneid);
    stmt->set_unsigned32(1, zonelocalid);
    stmt->set_unsigned32(2, overwrite);
    stmt->execute();

    stmt = _con->create_statement(query);
    stmt->set_unsigned32(0, uid);
    stmt->set_unsigned32(1, aid);
    stmt->execute();

    mariadb::result_set_ref result = _con->query("SELECT @zoneid, @zonelocalid, @overwrite");

    if (result->next()) {
        zoneid = result->get_unsigned64("@zoneid");
        zonelocalid = result->get_unsigned64("@zonelocalid");
        overwrite = result->get_unsigned64("@overwrite");
    }
}

void
database::record_offline(const BigEndian<uint32_t> &uid, const BigEndian<uint32_t> &aid, BigEndian<uint32_t> &zoneid,
                         BigEndian<uint32_t> &zonelocalid, BigEndian<uint32_t> &overwrite) {
    std::string query = "CALL recordoffline(?, ?, @zoneid, @zonelocalid, @overwrite)";
    std::string set = "SET @zoneid = ?, @zonelocalid = ?, @overwrite = ?";
    mariadb::statement_ref stmt = _con->create_statement(set);
    stmt->set_unsigned32(0, zoneid);
    stmt->set_unsigned32(1, zonelocalid);
    stmt->set_unsigned32(2, overwrite);
    stmt->execute();

    stmt = _con->create_statement(query);
    stmt->set_unsigned32(0, uid);
    stmt->set_unsigned32(1, aid);
    stmt->execute();

    mariadb::result_set_ref result = _con->query("COALESCE(@overwrite, 0) as overwrite");

    if (result->next()) {
        overwrite = result->get_unsigned64("overwrite");
    }
}

BigEndian<uint32_t> database::user_creatime(const BigEndian<uint32_t> &uid) {
    std::string query = "SELECT creatime FROM users WHERE ID=?";
    mariadb::statement_ref stmt = _con->create_statement(query);
    stmt->set_unsigned32(0, uid);
    if (stmt->execute()) {
        mariadb::result_set_ref result = stmt->query();
        if (result->next()) {
            return result->get_date_time("creatime").mktime();
        }
    }
    return 0;
}

void database::user_password(const std::string &login, BigEndian<uint32_t> &uid, std::string &password) {
    std::string query = "CALL acquireuserpasswd(?, @out1, @out2)";
    mariadb::statement_ref stmt = _con->create_statement(query);
    stmt->set_string(0, login);

    if (stmt->execute()) {
        mariadb::result_set_ref result = _con->query("SELECT @out1, @out2");

        if (result->next()) {
            uid = result->get_unsigned64("@out1");
            password = result->get_string("@out2");
        }
    }
}

std::vector<uint8_t> database::user_privileges(const BigEndian<uint32_t> &userid, const BigEndian<uint32_t> &zoneid) {
    std::string query = "SELECT rid FROM auth WHERE userid = ? AND zoneid = ?";
    mariadb::statement_ref stmt = _con->create_statement(query);
    stmt->set_unsigned32(0, userid);
    stmt->set_unsigned32(1, zoneid);

    std::vector<uint8_t> privileges;
    if (stmt->execute()) {
        mariadb::result_set_ref result = stmt->query();
        while (result->next()) {
            int rid = result->get_unsigned32("rid");
            privileges.push_back(rid);
        }
    }
    return privileges;
}

std::map<std::string, BigEndian<uint32_t>> database::use_cash_now(const BigEndian<uint32_t> &zoneid) {
    std::map<std::string, BigEndian<uint32_t>> gold;
    std::string query = "SELECT userid, zoneid, creatime FROM usecashnow WHERE status = ? AND zoneid = ? LIMIT 1";
    mariadb::statement_ref stmt = _con->create_statement(query);
    stmt->set_unsigned32(0, 0);
    stmt->set_unsigned32(1, zoneid);

    if (stmt->execute()) {
        mariadb::result_set_ref result = stmt->query();
        if (result->next()) {
            gold.insert(std::pair("userid", result->get_unsigned32("userid")));
            gold.insert(std::pair("zoneid", result->get_unsigned32("zoneid")));

            mariadb::date_time time = result->get_date_time("creatime");

            query = "UPDATE usecashnow SET status = ? WHERE userid = ? AND creatime = ?";
            stmt = _con->create_statement(query);
            stmt->set_unsigned32(0, 1);
            stmt->set_unsigned32(1, gold["userid"]);
            stmt->set_date_time(2, time);
            stmt->execute();
        }
    }
    return gold;
}

std::map<std::string, BigEndian<uint32_t>>
database::use_cash_now(const BigEndian<uint32_t> &userid, const BigEndian<uint32_t> &zoneid) {
    std::map<std::string, BigEndian<uint32_t>> gold;
    std::string query = "SELECT * FROM usecashnow WHERE userid = ? AND zoneid = ?";
    mariadb::statement_ref stmt = _con->create_statement(query);
    stmt->set_unsigned32(0, userid);
    stmt->set_unsigned32(1, zoneid);

    if (stmt->execute()) {
        mariadb::result_set_ref result = stmt->query();
        if (result->next()) {
            gold.insert(std::pair("userid", result->get_unsigned32("userid")));
            gold.insert(std::pair("zoneid", result->get_unsigned32("zoneid")));
            gold.insert(std::pair("cash", result->get_unsigned32("cash")));

            query = "DELETE FROM usecashnow WHERE userid = ? AND zoneid = ? AND status = ?";
            stmt = _con->create_statement(query);
            stmt->set_unsigned32(0, gold["userid"]);
            stmt->set_unsigned32(1, gold["zoneid"]);
            stmt->set_unsigned32(2, 1);
            stmt->execute();
        }
    }
    return gold;
}

std::vector<forbid> database::user_forbid(const BigEndian<uint32_t> &userid) {
    std::vector<forbid> forbids;
    std::string query = "SELECT userid, type, ctime, forbid_time, reason FROM forbid WHERE userid = ? AND DATE_ADD(ctime, INTERVAL forbid_time SECOND) > NOW()";
    mariadb::statement_ref stmt = _con->create_statement(query);
    stmt->set_unsigned32(0, userid);

    if (stmt->execute()) {
        mariadb::result_set_ref result = stmt->query();
        while (result->next()) {
            forbid temp;
            temp.userid = result->get_unsigned32("userid");
            temp.type = result->get_unsigned32("type");
            temp.ctime = result->get_date_time("ctime").mktime();
            temp.forbid_time = temp.ctime + result->get_unsigned32("forbid_time") - std::time(nullptr);
            temp.reason = result->get_string("reason");
            forbids.push_back(temp);
        }
    }
    return forbids;
}

ip_limit database::check_ip_limit(const BigEndian<uint32_t> &userid) {
    std::string query = "SELECT `ipaddr1`, `ipmask1`, `ipaddr2`, `ipmask2`, `ipaddr3`, `ipmask3`, `enable`, `lockstatus` FROM `iplimit` WHERE `uid` = ? LIMIT 1";
    mariadb::statement_ref stmt = _con->create_statement(query);
    stmt->set_unsigned32(0, userid);

    ip_limit limit{};

    if (stmt->execute()) {
        mariadb::result_set_ref result = stmt->query();

        if (result->next()) {
            limit.enable = result->get_string("enable");
            limit.lockstatus = result->get_string("lockstatus");
            for (int i = 0; i < 6; ++i) {
                std::map<std::string, LittleEndian<uint32_t>> temp;
                temp.insert(std::pair("ip", result->get_unsigned32(i)));
                try {
                    temp.insert(std::pair("mask", std::stoll(result->get_string(++i))));
                } catch (std::exception &exception) {
                    temp.insert(std::pair("mask", 32));
                }
                limit.ips.push_back(temp);
            }
        }
    }
    return limit;
}