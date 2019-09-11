

#include "sockets.h"


namespace net {

    void sockets::open(const std::string &name, const std::string &port) {
        int ret;
        struct addrinfo hints{}, *res;

        std::memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;

        if ((ret = getaddrinfo(name.c_str(), port.c_str(), &hints, &res)))
            throw socket_error(std::string("getaddrinfo(): ") + gai_strerror(ret));

        if ((_socket = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0) {
            throw socket_error(std::string("socketfd(): ") + strerror(errno));
        }

        if (connect(_socket, res->ai_addr, res->ai_addrlen) < 0) {
            close();
            throw socket_error(std::string("connect(): ") + strerror(errno));
        }
    }

    void sockets::bind(const std::string &address, const std::string &port) {
        int ret;
        struct addrinfo hints{}, *res;
        std::memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;

        if ((ret = getaddrinfo(address.c_str(), port.c_str(), &hints, &res))) {
            throw socket_error(std::string("getaddrinfo(): ") + gai_strerror(ret));
        }

        if ((_socket = ::socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0) {
            throw socket_error(std::string("socketfd(): ") + std::strerror(errno));
        }

        if (::bind(_socket, res->ai_addr, res->ai_addrlen) < 0) {
            throw socket_error(std::string("bind(): ") + std::strerror(errno));
        }

        if (::listen(_socket, SOMAXCONN) < 0) {
            throw socket_error(std::string("listen(): ") + std::strerror(errno));
        }
    }

    std::string sockets::read(int count) {
        std::unique_ptr<char> buf{new char[count]};
        int result = recv(_socket, buf.get(), count, 0);
        if (result < 0) {
            throw socket_error(std::string("recv(): ") + strerror(errno));
        }
        return std::string(buf.get(), result);
    }

    int sockets::write(util::bytestream &stream) {
        return ::send(_socket, stream.str().c_str(), stream.str().length(), 0);
    }

    sockets sockets::accept() {
        sockaddr_storage addr{};
        socklen_t addr_len = sizeof(addr);

        int ret = ::accept(_socket, reinterpret_cast<struct sockaddr *>(&addr), &addr_len);
        if (ret < 0) {
            throw socket_error(std::string("accept(): ") + strerror(errno));
        }

        return sockets(ret);
    }


    void sockets::close() {
        ::close(_socket);
        _socket = -1;
    }
}