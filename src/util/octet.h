

#ifndef AUTHD_DEV_OCTETS_H
#define AUTHD_DEV_OCTETS_H

#include <sstream>
#include <memory>
#include "../net/ip.h"
#include "../util/littlebigendian.h"

namespace util {

    BigEndian<uint32_t> ReadCUInt(std::istream &is);

    std::string WriteCUInt(BigEndian<uint32_t> value);

    class bytestream : public std::stringstream {
    public:
        using std::stringstream::basic_stringstream;

    };

    template<typename T>
    bytestream &operator>>(bytestream &is, BigEndian<T> &value) {
        is.read(reinterpret_cast<char *>(&value), sizeof(T));
        return is;
    }

    template<typename T>
    bytestream &operator>>(bytestream &is, LittleEndian<T> &value) {
        is.read(reinterpret_cast<char *>(&value), sizeof(T));
        return is;
    }

    template<typename T>
    bytestream &operator<<(bytestream &os, BigEndian<T> &value) {
        os.write(reinterpret_cast<char *>(&value), sizeof(T));
        return os;
    }

    template<typename T>
    bytestream &operator<<(bytestream &os, LittleEndian<T> &value) {
        os.write(reinterpret_cast<char *>(&value), sizeof(T));
        return os;
    }

    bytestream &operator>>(bytestream &is, net::ip &value);

    bytestream &operator<<(bytestream &os, net::ip &value);


    class octet : public std::string {
    public:
        using std::string::string;

        octet(const std::string &str);

        friend bytestream &operator>>(bytestream &is, octet &value);

        friend bytestream &operator<<(bytestream &os, octet &value);

    };
}

#endif //AUTHD_DEV_OCTETS_H
