#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <uv.h>
namespace Network
{
extern bool connected;
void connect(uv_loop_t* loop, const char* ip, int port);
void disconnect();
}

#endif
