#include <uv.h>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include "network.h"
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
    rapidjson::Document dom;
    if(!dom.Parse(buf->base).HasParseError()) {
        Network::call_interface(dom);
    }
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
void send(const rapidjson::Document& dom)// Send json to server
{
    rapidjson::StringBuffer sb;
    rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
    dom.Accept(writer);
    Network::sendRaw(sb.GetString(),(unsigned int)sb.GetSize());
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
