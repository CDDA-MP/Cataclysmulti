#include <uv.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include "network.h"
#include "game.h"
#include "protocol.h"

static void alloc_cb(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
{
    buf->base = (char*)malloc(suggested_size);
    buf->len = suggested_size;
}

#define STRING_END '\x00'
#define PACK_START '\xFF'
#define ARG_START '\xFE'
static void read_cb(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf)
{
    // Check connection
    if(!nread) {
        puts("disconnected!");
        Network::connected = false;
        return;
    }

    // decode the data
    char* p = buf->base;
    ssize_t length = nread;
    bool skip_till_strend = false;
    char* cmd = nullptr;
    std::vector<char*> argv;
    // Example: "\xFFset\x00\xFEfoo\x00\FEbar\x00" means CMD:set ARG:{foo,bar}
    while(length) {
        if(!skip_till_strend) {
            if(*p == PACK_START) {
                if(cmd == nullptr) {
                    cmd = p + 1;
                    skip_till_strend = true;
                } else {
                    Network::call_interface(cmd, argv);
                    cmd = nullptr;
                    argv.clear();
                }
            } else if(*p == ARG_START) {
                argv.push_back(p + 1);
                skip_till_strend = true;
            }
        } else if(*p == STRING_END) {
            skip_till_strend = false;
        }
        p++;
        length--;
    }
    if(cmd != nullptr) {
        Network::call_interface(cmd, argv);
    }
    free(buf->base);
}

static void connect_cb(uv_connect_t* req, int status)
{
    if(status == 0) {
        uv_read_start(req->handle, alloc_cb, read_cb);
        Network::connected = true;
        Network::init_interfaces();
    } else {
        printf("Unable to create connection: Status %i\n", status);
    }
}

namespace Network
{
bool connected;
void connect(uv_loop_t* loop, const char* ip, int port)
{
    uv_tcp_t* socket = new uv_tcp_t;
    uv_tcp_init(loop, socket);

    uv_connect_t* req = new uv_connect_t;

    sockaddr_in* dest = new sockaddr_in;
    uv_ip4_addr(ip, port, dest);

    uv_tcp_connect(req, socket, (const sockaddr*)dest, connect_cb);
    // TODO:Connect timeout
}
}
