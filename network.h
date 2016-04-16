#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <uv.h>
#include <rapidjson/document.h>
namespace Network
{
extern bool connected;
void connect(const char* ip, int port);
void send(const rapidjson::Document& dom);
void sendRaw(const char* data,unsigned int len);
void disconnect();
}

#endif
