#ifndef ARC4_H
#define ARC4_H

#include <string>
#include <array>

namespace security {
    class arc4 {

    protected:
        std::array<uint8_t, 256> block{};
        int indexA = 0, indexB = 0;

    public:
        arc4() = default;

        explicit arc4(const std::string &key);

        void setKey(const std::string &key);

        std::string encrypt(const std::string &in);
    };
}
#endif