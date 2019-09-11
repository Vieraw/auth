

#ifndef AUTHD_DEV_SOCKETS_H
#define AUTHD_DEV_SOCKETS_H

#include <sstream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>

#include "../util/octet.h"

namespace net {

    class socket_error : public std::exception {
        std::string error;
    public:

        explicit socket_error(std::string &&s) : std::exception(), error(s) {}

        ~socket_error() override = default;

        const char *what() const noexcept override { return error.c_str(); }
    };

    class sockets {

    public:
        int _socket = -1;

        sockets() = default;

        explicit sockets(int socket) : _socket(socket) {}

        void open(std::string const &name, std::string const &port);

        void bind(std::string const &address, std::string const &port);

        std::string read(int count);

        int write(util::bytestream &stream);

        bool is_open() { return _socket >= 0; }

        sockets accept();

        void close();
    };
}

#endif //AUTHD_DEV_SOCKETS_H
