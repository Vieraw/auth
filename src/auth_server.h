

#ifndef AUTHD_DEV_SERVER_H
#define AUTHD_DEV_SERVER_H


#include "net/sockets.h"
#include "auth_client.h"

class server_error : public std::exception {
    std::string error;
public:

    explicit server_error(std::string &&s) : std::exception(), error(s) {}

    ~server_error() override = default;

    const char *what() const noexcept override { return error.c_str(); }
};


class auth_server {
    net::sockets _socket;

public:
    void run(const std::string &config);

    static void worker(std::shared_ptr<auth_client> &&client);

    void bind(const std::string &address, const std::string &port);

    std::shared_ptr<auth_client> accept();

    bool is_open();

    void close();
};


#endif //AUTHD_DEV_SERVER_H
