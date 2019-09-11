

#ifndef AUTHD_DEV_BASE_H
#define AUTHD_DEV_BASE_H


#include <memory>
#include <unordered_map>
#include <functional>

#include "../registry.h"
#include "../util/octet.h"
#include "../util/littlebigendian.h"

class auth_client;

namespace protocol {

    class protocol_error : public std::exception {
        std::string error;
    public:

        explicit protocol_error(const std::string &&s) : std::exception(), error(s) {}

        ~protocol_error() override = default;

        const char *what() const noexcept override { return error.c_str(); }
    };

    class base {
        std::shared_ptr<auth_client> _client = nullptr;
    public:
        explicit base(std::shared_ptr<auth_client> client) : _client(std::move(client)) {}

        void setClient(std::shared_ptr<auth_client> &client);

        void setSession();

        std::shared_ptr<auth_client> &getClient();

        virtual void read(util::bytestream &data) = 0;

        virtual void write() = 0;

        BigEndian<uint32_t> session;
    };

    class factory {
    public:
        typedef std::unordered_map<uint32_t, std::function<std::unique_ptr<base>(
                const std::shared_ptr<auth_client> &)>> registry_map;

        static std::unique_ptr<base> create(const uint32_t &name, const std::shared_ptr<auth_client> &client) {
            auto it = factory::registry().find(name);
            return it == factory::registry().end() ? nullptr : (it->second)(client);
        }

        static registry_map &registry() {
            static registry_map impl;
            return impl;
        }

    };

    template<typename T>
    struct factory_register {
        explicit factory_register(const uint32_t &name) {
            factory::registry()[name] = [](const std::shared_ptr<auth_client> &client) {
                return std::unique_ptr<T>(new T(client));
            };
            std::cout << "Registering protocol: " << name << std::endl;
        }
    };
}


#endif //AUTHD_DEV_BASE_H
