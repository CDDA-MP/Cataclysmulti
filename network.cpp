#include <uv.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "network.h"
#include "game.h"
#include "protocol.h"
#include "main.h"

namespace Network
{
bool connected;
static uv_connect_t* req;
uv_tcp_t* socket;
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
        Network::disconnect();
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
        gameInit();
    } else {
        printf("Unable to create connection: Status %i\n", status);
    }
}

static void close_cb(uv_handle_s*)
{
    Network::connected = false;
}

static void write_cb(uv_write_s*, int)
{
}

void sendRaw(char* data, int len) // Send raw data to server
{
    if(Network::connected) {
        uv_write_t* wr = new uv_write_t;
        uv_buf_t buf = uv_buf_init(data, len);
        uv_write(wr, (uv_stream_t*)Network::socket, &buf, 1, write_cb);
    } else {
        gameOver();
    }
}

void connect(const char* ip, int port)
{
    Network::connected = false;
    socket = new uv_tcp_t;
    uv_tcp_init(uv_default_loop(), socket);

    Network::req = new uv_connect_t;

    sockaddr_in* dest = new sockaddr_in;
    uv_ip4_addr(ip, port, dest);

    uv_tcp_connect(req, socket, (const sockaddr*)dest, connect_cb);
    // TODO:Connect timeout
}
void disconnect()
{
    if(Network::connected) {
        uv_close((uv_handle_s*)Network::req, close_cb);
        uv_stop(uv_default_loop());
    }
}
}
