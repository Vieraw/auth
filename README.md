# Auth daemon (Beta)
**Auth daemon** - open source authorization service for the MMORPG game Forsaken World (Dark Age), written in C ++

---

#### Warning - the application is under development, bugs are possible.

---

## Dependencies & Requirements
- C++17 compiler
- CMake 3.1+
- OpenSSL, pthread, mariadbclient or mysqlclient libraries

## Build & Run
- Clone repository: git clone https://github.com/Vieraw/auth.git
- Initialize recursive Git submodules: git submodule update --init --recursive
- Install mariadbclient or mysqlclient libraries.
- Build the application using cmake
```bash
cd /folder/with/your/projects/auth
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```
- Stop game server
- Add run command to file start.sh
- Start game server


## Implemented Protocols
    0xA     -   KickoutUser
    0xF     -   UserLogin
    0x21    -   UserLogout
    0x1FD   -   QueryUserForbid
    0x202   -   GetAddCashSN
    0x203   -   AddCashArg
    0x204   -   AddCashRes
    0x20C   -   QueryUserPrivilege
    0x20F   -   AnnounceZoneid
    0x221   -   CertRequest
    0x222   -   CertAnswer
    0x223   -   CertKey
    0x224   -   CertFinish
    0x226   -   MatrixPasswd
    0x1F67  -   Game2AU


## Configuration
```ini
[GAuthServer]
# The port application is listening
port = 62900 
# The address application accepts connections from
address = 0.0.0.0
# The key to decode packets from gdeliveryd. Must match oseckey in gdeliveryd config.
iseckey = 123
# The key is to encode response packets. Must match iseckey in gdeliveryd config.
oseckey = 123
# Path to certificate file
cert = ./auth.cert
# Path to key file
key = ./auth.key

[Database]
# Mysql Address
host = localhost
#Mysql user name
name = root
#Mysql user password
password =
#Mysql database name
db = fw

[IPLimitMessage]
# Unauthorized IP login message
ip = Blocked IP
# A message when accessing an account is denied
lock = Login denied

[Gold]
# Gold Checkout Interval
interval = 30

#Not implemented at the moment
[BruteForce]
enabled = true
interval = 10
count = 5


```

## TODO
- [ ] Add BruteForce protection
- [ ] Add other protocols


## License
See accompanying [LICENSE](LICENSE) file.
