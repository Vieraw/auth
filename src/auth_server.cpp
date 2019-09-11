

#include <sstream>
#include <thread>

#include "auth_server.h"

#include "registry.h"
#include "util/ini.h"
#include "security/encryptor.h"
#include "security/rsa.h"

void auth_server::run(const std::string &config) {
    try {
        registry::setConfig(util::ini<char>(config));

        if (registry::getConfig()["GAuthServer"]["address"].empty()) {
            throw server_error("Server error: GAuthServer.address is empty.");
        }

        if (registry::getConfig()["GAuthServer"]["port"].empty()) {
            throw server_error("Server error: GAuthServer.port is empty.");
        }

        if (!registry::getConfig()["GAuthServer"]["cert"].empty()) {
            registry::setCert(registry::getConfig()["GAuthServer"]["cert"]);
        }

        if (!registry::getConfig()["GAuthServer"]["key"].empty()) {
            registry::setRSA(security::rsa(registry::getConfig()["GAuthServer"]["key"]));
        }

        if (registry::getConfig()["Database"]["host"].empty() || registry::getConfig()["Database"]["name"].empty() ||
            registry::getConfig()["Database"]["db"].empty()) {
            throw server_error("Server error: database not configure.");
        }

        try {
            if (std::stoll(registry::getConfig()["Gold"]["interval"]) < 0) {
                registry::getConfig()["Gold"]["interval"] = "30";
            }
        } catch (std::exception &exception) {
            registry::getConfig()["Gold"]["interval"] = "30";
        }

        database db;
        db.connect(registry::getConfig()["Database"]["host"], registry::getConfig()["Database"]["name"],
                   registry::getConfig()["Database"]["password"], registry::getConfig()["Database"]["db"]);

        registry::setDatabase(std::move(db));

        bind(registry::getConfig()["GAuthServer"]["address"], registry::getConfig()["GAuthServer"]["port"]);

        while (is_open()) {
            std::thread(&auth_server::worker, accept()).detach();
            std::cout << "Accept" << std::endl;
        }
    }
    catch (std::exception &exception) {
        close();
        std::cerr << exception.what() << std::endl;
    }
}

void auth_server::worker(std::shared_ptr<auth_client> &&client) {
    auth_client::run(client);
}

void auth_server::bind(const std::string &address, const std::string &port) {
    _socket.bind(address, port);
}

std::shared_ptr<auth_client> auth_server::accept() {
    return std::make_shared<auth_client>(auth_client(_socket.accept()));
}

bool auth_server::is_open() {
    return _socket.is_open();
}

void auth_server::close() {
    _socket.close();
}