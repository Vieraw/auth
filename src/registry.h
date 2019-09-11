


#ifndef AUTHD_DEV_REGISTRY_H
#define AUTHD_DEV_REGISTRY_H

#include "security/rsa.h"
#include "util/ini.h"
#include "database.h"

class registry {
    static security::rsa _rsa;
    static std::string _cert;
    static util::ini<char> _config;
    static database _database;

public:
    static security::rsa &getRSA();

    static void setRSA(security::rsa &&rsa);

    static std::string &getCert();

    static void setCert(const std::string &filename);

    static util::ini<char> &getConfig();

    static void setConfig(util::ini<char> &&config);

    static database &getDatabase();

    static void setDatabase(database &&db);
};


#endif //AUTHD_DEV_REGISTRY_H
