

#ifndef AUTHD_DEV_AUTH_CLIENT_H
#define AUTHD_DEV_AUTH_CLIENT_H

#include "net/sockets.h"
#include "security/encryptor.h"
#include "util/littlebigendian.h"

class client_error : public std::exception {
    std::string error;
public:

    explicit client_error(std::string &&s) : std::exception(), error(s) {}

    ~client_error() override = default;

    const char *what() const noexcept override { return error.c_str(); }
};

class auth_client {
    net::sockets _socket;
    security::encryptor _encryptor;

    std::string _buffer = "";

    BigEndian<uint32_t> _zoneid = 0;
    BigEndian<uint32_t> _aid = 0;
    BigEndian<uint32_t> _version = 0;

public:
    explicit auth_client() : _socket(-1) {};

    explicit auth_client(const net::sockets &socket) : _socket(socket) {};

    static void run(std::shared_ptr<auth_client> &client);

    void setZoneId(const BigEndian<uint32_t> &zoneid);

    BigEndian<uint32_t> getZoneId();

    void setAid(const BigEndian<uint32_t> &aid);

    BigEndian<uint32_t> getAid();

    void setVersion(const BigEndian<uint32_t> &version);

    BigEndian<uint32_t> getVersion();

    void setEncryptor(const std::string &encrypt_key, const std::string &decrypt_key);

    void open(std::string const &name, std::string const &port);

    std::string read();

    int write(const BigEndian<uint32_t> &opcode, util::bytestream &stream);

    bool is_open();

    void close();

};

#endif //AUTHD_DEV_AUTH_CLIENT_H
