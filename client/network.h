#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <uv.h>
#include <json11.hpp>
#include <string.h>
namespace Network {
    extern bool connected;
    extern bool handshaked;
    void connect(const char* ip, int port);
    void disconnect();

    void send(const json11::Json& json);
    void send(const std::string& str);
}

#endif
