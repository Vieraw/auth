

#include <memory>
#include <thread>
#include <mutex>
#include <chrono>

#include "auth_client.h"

#include "registry.h"
#include "util/ini.h"
#include "protocol/base.h"
#include "protocol/get_add_cash_sn.h"


void auth_client::run(std::shared_ptr<auth_client> &client) {
    client->setEncryptor(registry::getConfig()["GAuthServer"]["oseckey"],
                         registry::getConfig()["GAuthServer"]["iseckey"]);

    std::mutex mtx;

    std::thread([client, &mtx]() {
        std::this_thread::sleep_for(std::chrono::seconds(30));
        while (client && client->is_open()) {
            if (client->getZoneId()) {

                auto gold = registry::getDatabase().use_cash_now(client->getZoneId());

                if (gold["userid"]) {
                    std::lock_guard lock{mtx};
                    protocol::get_add_cash_sn protocol{client};
                    protocol.session = -1;
                    protocol.userid = gold["userid"];
                    protocol.zoneid = gold["zoneid"];
                    protocol.write();

                    std::cout << "GetAddCashSn: userid" << gold["userid"]
                              << " zoneid: " << gold["zoneid"] << std::endl;
                }
            }

            std::this_thread::sleep_for(std::chrono::seconds(std::stoll(registry::getConfig()["Gold"]["interval"])));
        }
    }).detach();

    while (client->is_open()) {
        try {
            util::bytestream stream{client->read()};
            std::lock_guard lock{mtx};

            if (stream.str().empty()) {
                client->close();
                throw client_error("Client: connection close.");
            }

            BigEndian<uint32_t> opcode = util::ReadCUInt(stream);
            BigEndian<uint32_t> size = util::ReadCUInt(stream);

            std::cout << "Opcode: " << opcode << " Size: " << size << std::endl;

            auto protocol = protocol::factory::create(opcode, client);
            if (protocol == nullptr) {
                throw client_error("Client error: protocol not found.");
            }
            protocol->read(stream);
        } catch (std::exception &exception) {
            std::cerr << exception.what() << std::endl;
        }
    }
}

void auth_client::setZoneId(const BigEndian<uint32_t> &zoneid) {
    _zoneid = zoneid;
}

BigEndian<uint32_t> auth_client::getZoneId() {
    return _zoneid;
}

void auth_client::setAid(const BigEndian<uint32_t> &aid) {
    _aid = aid;
}

BigEndian<uint32_t> auth_client::getAid() {
    return _aid;
}

void auth_client::setVersion(const BigEndian<uint32_t> &version) {
    _version = version;
}

BigEndian<uint32_t> auth_client::getVersion() {
    return _version;
}

void auth_client::setEncryptor(const std::string &encrypt_key, const std::string &decrypt_key) {
    if (!encrypt_key.empty()) {
        _encryptor.setEncrypt(new security::arc4(encrypt_key));
    }

    if (!decrypt_key.empty()) {
        _encryptor.setDecrypt(new security::arc4(decrypt_key));
    }
}

void auth_client::open(std::string const &name, std::string const &port) {
    _socket.open(name, port);
}

std::string auth_client::read() {
    if (_buffer.empty()) {
        _buffer = _encryptor.decrypt(_socket.read(65536));
    }

    if (_buffer.empty()) {
        return "";
    }

    util::bytestream stream(_buffer);
    util::ReadCUInt(stream);
    BigEndian<uint32_t> size = util::ReadCUInt(stream);
    uint32_t end = size + stream.tellg();

    if (end > _buffer.length()) {
        _buffer.clear();
        throw client_error("Client error: protocol size police error.");
    }

    std::string result = _buffer.substr(0, end);
    _buffer = _buffer.substr(end);
    return result;
}

int auth_client::write(const BigEndian<uint32_t> &opcode, util::bytestream &stream) {
    util::bytestream package;
    package << util::WriteCUInt(opcode) << util::WriteCUInt(stream.str().length()) << stream.str();
    package.str(_encryptor.encrypt(package.str()));
    return _socket.write(package);
}

bool auth_client::is_open() {
    return _socket.is_open();
}

void auth_client::close() {
    _socket.close();
}