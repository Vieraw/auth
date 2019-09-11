

#include <string>

#ifndef AUTHD_DEV_ENCODING_H
#define AUTHD_DEV_ENCODING_H

std::string hex_to_bytes(const std::string &hex);

std::string
convert_encoding(const std::string &input, const std::string &out_encode, const std::string &in_encode = "UTF-8");

#endif //AUTHD_DEV_ENCODING_H
