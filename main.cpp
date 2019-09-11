#include "src/auth_server.h"

int main(int argc, char *argv[]) {
    auth_server server;
    std::string config = "./auth.ini";

    if (argc > 1) {
        config = argv[1];
    }

    server.run(config);

    return 0;
}
