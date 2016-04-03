#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <uv.h>
namespace Network
{
extern bool connected;
void connect(const char* ip, int port);
void sendRaw(char* data, int len);
void disconnect();
}

#endif
