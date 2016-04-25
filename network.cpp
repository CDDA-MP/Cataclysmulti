#include "network.h"

#include <uv.h>
#include <json11.hpp>

#include "game.h"
#include "protocol.h"
#include "main.h"


// TODO:Remove libuv dependence.
namespace Network
{
bool connected;
static uv_connect_t* req;
uv_tcp_t* socket;
static void alloc_cb(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
{
    buf->base = new char[suggested_size];
    buf->len = suggested_size;
}

static void read_cb(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf)
{
    // Check connection
    if(!nread) {
        Network::disconnect();
        return;
    }
    std::string err;
    json11::Json json = json11::Json::parse(buf->base,err);
    if(err.length() == 0) {
        Network::call_Callback(json);
    }
}

static void connect_cb(uv_connect_t* req, int status)
{
    if(status == 0) {
        uv_read_start(req->handle, alloc_cb, read_cb);
        Network::connected = true;
        Network::init_Callbacks();
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
void send(const json11::Json& json)
{
    Network::send(json.dump());
}

void send(const std::string& str) {
    Network::sendRaw(str.c_str(),str.length());
}

void sendRaw(const char* data, unsigned int len) // Send raw data to server
{
    if(Network::connected) {
        uv_write_t* wr = new uv_write_t;
        uv_buf_t buf = uv_buf_init((char *)data, len);
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

    sockaddr dest;
    uv_ip4_addr(ip, port, (sockaddr_in*)&dest);

    uv_tcp_connect(req, socket, &dest, connect_cb);
    // TODO:Connect timeout
}

void disconnect()
{
    if(Network::connected) {
        if(uv_is_active((uv_handle_s*)Network::socket) && !uv_is_closing((uv_handle_s*)Network::socket)) {
            uv_close((uv_handle_s*)Network::socket, close_cb);
        }
        uv_stop(uv_default_loop());
    }
}
}
