#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <json11.hpp>
#include <string>

#include "uv.h"
#include "player.h"

namespace Network {
    bool bind(const char *addr = "0.0.0.0", int port = 8000);

    void send(const json11::Json& json,const Player& player);
    void send(const std::string& str,const Player& player);

    struct Address {
        char *addr;
        in_port_t port;
    };

    struct AddressHasher {
        std::size_t operator()(const Network::Address& k) const;
    };

    int getSockAddr(const uv_tcp_t *handle, sockaddr_in *_sockaddr);
    Address getAddress(const uv_tcp_t *handle);
}

namespace std {
    template<>
    struct hash<Network::Address>;
}
bool operator==(Network::Address a,Network::Address b);

#endif
