#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <json11.hpp>
#include <string>

#include "uv.h"
#include "player.h"

namespace Network {
    bool bind(const char *addr = "0.0.0.0", int port = 8000);
    int getSockAddr(const uv_tcp_t *handle, sockaddr_in *_sockaddr);
    std::string getAddress(const uv_tcp_t *handle);
    void send(const json11::Json& json,const Player& player) ;
    void send(const std::string& str,const Player& player) ;
}


#endif
