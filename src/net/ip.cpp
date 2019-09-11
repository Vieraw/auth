//
// Created by php on 28-Aug-19.
//

#include <sstream>
#include <vector>
#include "ip.h"

namespace net {

    std::vector<std::string> split(const std::string &data, char separator) {
        std::vector<std::string> temp{};
        std::stringstream ss(data);
        std::string str;

        while (std::getline(ss, str, separator)) {
            temp.push_back(str);
        }
        return temp;
    }


    ip::ip(const std::string &ip) {
        std::vector<std::string> parts = split(ip, '.');
        if (parts.size() != 4) {
            throw ip_error("Invalid format");
        }

        _ip = (std::stoull(parts[0]) << 24u) |
              (std::stoull(parts[1]) << 16u) |
              (std::stoull(parts[2]) << 8u) |
              (std::stoull(parts[3]));
    }

    ip::ip(const uint32_t &ip) {
        _ip = ip;
    }

    ip &ip::operator++() {
        ++_ip;
        return *this;
    }

    ip ip::operator++(int) {
        const ip result(*this);
        ++_ip;
        return result;
    }

    ip &ip::operator--() {
        --_ip;
        return *this;
    }

    ip ip::operator--(int) {
        const ip result(*this);
        --_ip;
        return result;
    }

    ip::operator unsigned int() const {
        return _ip;
    }

    std::string ip::to_string() const {
        std::stringstream ss;
        ss << (_ip >> 24u & 0xFFu) << '.' << (_ip >> 16u & 0xFFu) << '.' << (_ip >> 8u & 0xFFu) << '.' << (_ip & 0xFFu);
        return ss.str();
    }

    bool operator<(const ip &first, const ip &second) {
        return first._ip < second._ip;
    }

    bool operator==(const ip &first, const ip &second) {
        return first._ip == second._ip;
    }

    std::ostream &operator<<(std::ostream &os, const ip &address) {
        os << address.to_string();
        return os;
    }
}