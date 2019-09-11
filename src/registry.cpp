

#include "registry.h"

security::rsa registry::_rsa;

security::rsa &registry::getRSA() {
    return _rsa;
}

void registry::setRSA(security::rsa &&rsa) {
    _rsa = rsa;
}

std::string registry::_cert;

std::string &registry::getCert() {
    return _cert;
}

void registry::setCert(const std::string &filename) {
    std::ifstream cert_file{filename, std::ios_base::in | std::ios_base::binary};
    _cert = std::string((std::istreambuf_iterator<char>(cert_file)), std::istreambuf_iterator<char>());
}

util::ini<char> registry::_config;

util::ini<char> &registry::getConfig() {
    return _config;
}

void registry::setConfig(util::ini<char> &&config) {
    _config = std::move(config);
}

database registry::_database;

database &registry::getDatabase() {
    return _database;
}

void registry::setDatabase(database &&db) {
    _database = std::move(db);
}