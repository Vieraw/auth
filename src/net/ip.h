//
// Created by php on 28-Aug-19.
//

#ifndef AUTH_DEV_IP_H
#define AUTH_DEV_IP_H

#include <string>

namespace net {

    class ip_error : public std::exception {
        std::string error;
    public:

        explicit ip_error(std::string &&s) : std::exception(), error(s) {}

        ~ip_error() override = default;

        const char *what() const noexcept override { return error.c_str(); }
    };

    class ip {
        uint32_t _ip;
    public:
        ip() = default;

        explicit ip(const std::string &ip);

        explicit ip(const uint32_t &ip);

        ip &operator++();

        ip operator++(int);

        ip &operator--();

        ip operator--(int);

        explicit operator unsigned int() const;

        [[nodiscard]] std::string to_string() const;

        friend bool operator<(const ip &first, const ip &second);

        friend bool operator==(const ip &first, const ip &second);
    };

    std::ostream &operator<<(std::ostream &os, const ip &address);
}

#endif //AUTH_DEV_IP_H
