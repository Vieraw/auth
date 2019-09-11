cmake_minimum_required(VERSION 3.1)

project(auth)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME}
        main.cpp
        src/database.cpp
        src/auth_server.cpp
        src/auth_client.cpp
        src/registry.cpp
        src/util/encoding.cpp
        src/util/octet.cpp
        src/net/sockets.cpp
        src/net/ip.cpp
        src/security/encryptor.cpp
        src/security/arc4.cpp
        src/security/rsa.cpp
        src/protocol/base.cpp
        src/protocol/cert_request.cpp
        src/protocol/cert_answer.cpp
        src/protocol/cert_key.cpp
        src/protocol/cert_finish.cpp
        src/protocol/announce_zoneid3.cpp
        src/protocol/matrix_passwd.cpp
        src/protocol/query_user_forbid.cpp
        src/protocol/user_login.cpp
        src/protocol/query_user_privilege.cpp
        src/protocol/game_2_au.cpp
        src/protocol/kickout_user.cpp
        src/protocol/user_logout.cpp
        src/protocol/get_add_cash_sn.cpp
        src/protocol/add_cash.cpp
        )

add_subdirectory(external/mariadbpp)

target_link_libraries(${PROJECT_NAME} pthread crypto mariadbclientpp)

configure_file(auth.key auth.key COPYONLY)
configure_file(auth.cert auth.cert COPYONLY)
configure_file(auth.ini auth.ini COPYONLY)
