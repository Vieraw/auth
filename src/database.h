

#ifndef AUTHD_DEV_DATABASE_H
#define AUTHD_DEV_DATABASE_H

#include <ctime>
#include <string>
#include <mariadb++/connection.hpp>
#include "util/littlebigendian.h"

class database_error : public std::exception {
    std::string error;
public:

    explicit database_error(std::string &&s) : std::exception(), error(s) {}

    ~database_error() override = default;

    const char *what() const noexcept override { return error.c_str(); }
};

struct forbid {
    BigEndian<uint32_t> userid;
    BigEndian<uint32_t> type;
    BigEndian<int32_t> ctime;
    BigEndian<int32_t> forbid_time;
    std::string reason;
};

struct ip_limit {
    std::string enable = "f";
    std::string lockstatus = "f";
    std::vector<std::map<std::string, LittleEndian<uint32_t>>> ips;
};

class database {
    mariadb::connection_ref _con;
public:
    void connect(const std::string &host, const std::string &name, const std::string &password, const std::string &db);

    void clear_online(const BigEndian<uint32_t> &zoneid, const BigEndian<uint32_t> &aid);


    void record_online(const BigEndian<uint32_t> &uid, const BigEndian<uint32_t> &aid, BigEndian<uint32_t> &zoneid,
                       BigEndian<uint32_t> &zonelocalid, BigEndian<uint32_t> &overwrite);

    void record_offline(const BigEndian<uint32_t> &uid, const BigEndian<uint32_t> &aid, BigEndian<uint32_t> &zoneid,
                        BigEndian<uint32_t> &zonelocalid, BigEndian<uint32_t> &overwrite);

    BigEndian<uint32_t> user_creatime(const BigEndian<uint32_t> &uid);

    void user_password(const std::string &login, BigEndian<uint32_t> &uid, std::string &password);

    std::vector<uint8_t> user_privileges(const BigEndian<uint32_t> &userid, const BigEndian<uint32_t> &zoneid);

    std::map<std::string, BigEndian<uint32_t>> use_cash_now(const BigEndian<uint32_t> &zoneid);

    std::map<std::string, BigEndian<uint32_t>>
    use_cash_now(const BigEndian<uint32_t> &userid, const BigEndian<uint32_t> &zoneid);

    std::vector<forbid> user_forbid(const BigEndian<uint32_t> &userid);

    ip_limit check_ip_limit(const BigEndian<uint32_t> &userid);
};


#endif //AUTHD_DEV_DATABASE_H
