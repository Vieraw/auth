

#include "octet.h"

namespace util {


    BigEndian<uint32_t> ReadCUInt(std::istream &is) {
        char buffer[4];
        is.read(buffer, 1);

        switch (buffer[0] & 0xE0u) {
            case 224:
                is.read(buffer, sizeof(buffer));
                return *reinterpret_cast<BigEndian<uint32_t> *>(&buffer);
            case 192:
                is.read(&buffer[1], 3);
                return *reinterpret_cast<BigEndian<uint32_t> *>(&buffer) & 0x3FFFFFFFu;
            case 128:
            case 160:
                is.read(&buffer[1], 1);
                return *reinterpret_cast<BigEndian<uint16_t> *>(&buffer) & 0x7FFFu;
            default:
                return buffer[0];
        }
    }

    std::string WriteCUInt(BigEndian<uint32_t> value) {
        bytestream ss;
        if (value < 128) {
            ss << static_cast<char>(value);
        } else if (value < 16384) {
            auto ushort = static_cast<BigEndian<uint16_t>>(value | 0x8000u);
            ss.write(reinterpret_cast<char *>(&ushort), 2);
        } else if (value < 536870912) {
            value = value | 0xC0000000u;
            ss.write(reinterpret_cast<char *>(&value), 4);
        } else {
            ss << static_cast<char>(224);
            ss.write(reinterpret_cast<char *>(&value), 4);
        }

        return ss.str();
    }

    bytestream &operator<<(bytestream &os, octet &value) {
        os << WriteCUInt(value.length()) << value;
        return os;
    }

    bytestream &operator>>(bytestream &is, net::ip &value) {
        LittleEndian<uint32_t> temp;
        is >> temp;
        value = net::ip(temp);
        return is;
    }

    bytestream &operator<<(bytestream &os, net::ip &value) {
        os.write(reinterpret_cast<char *>(static_cast<uint32_t>(value)), 4);
        return os;
    }

    octet::octet(const std::string &str) {
        this->assign(str);
    }

    bytestream &operator>>(bytestream &is, octet &value) {
        BigEndian<uint32_t> size = ReadCUInt(is);
        std::shared_ptr<char> buffer{new char[size]};
        is.read(buffer.get(), size);
        value.assign(buffer.get(), size);
        return is;
    }

}