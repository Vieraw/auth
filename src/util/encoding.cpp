

#include <iconv.h>
#include <vector>
#include <array>
#include <stdexcept>

#include "encoding.h"

std::string hex_to_bytes(const std::string &hex) {
    std::string bytes;

    for (std::string::size_type i = 0; i < hex.length(); i += 2) {
        bytes += static_cast<char>(std::strtol(hex.substr(i, 2).c_str(), nullptr, 16));
    }

    return bytes;
}

std::string convert_encoding(const std::string &input, const std::string &out_encode, const std::string &in_encode) {

    iconv_t convert = ::iconv_open(out_encode.c_str(), in_encode.c_str());
    if (convert == reinterpret_cast<iconv_t>(-1)) {
        if (errno == EINVAL) {
            throw std::runtime_error("Iconv error: not supported from " + in_encode + " to " + out_encode);
        }
        throw std::runtime_error("Iconv error: unknown error");
    }

    std::vector<char> in_buf(input.begin(), input.end());
    std::array<char, 1024> buf{};
    std::string dst;

    char *src_ptr = &in_buf[0];
    size_t src_size = input.size();

    while (0 < src_size) {
        char *dst_ptr = &buf[0];
        size_t dst_size = buf.size();
        size_t res = ::iconv(convert, &src_ptr, &src_size, &dst_ptr, &dst_size);
        if (res == static_cast<size_t>(-1)) {
            switch (errno) {
                case E2BIG:
                    break;
                case EILSEQ:
                case EINVAL:
                    throw std::runtime_error("Iconv error: invalid multibyte chars");
                default:
                    throw std::runtime_error("Iconv error: unknown error");
            }
        }
        dst.append(&buf[0], buf.size() - dst_size);
    }

    return dst;
}