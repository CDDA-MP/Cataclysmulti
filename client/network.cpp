#include "network.h"

#include <uv.h>
#include <json11.hpp>

#include "game.h"
#include "protocol.h"
#include "main.h"

// TODO:Remove libuv dependence.
namespace Network {
    bool connected;
    static uv_connect_t req;
    uv_tcp_t socket;
    static void alloc_cb(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
        buf->base = new char[suggested_size];
        buf->len = suggested_size;
    }

    static void read_cb(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf) {
        // Check connection
        if (!nread) {
            Network::disconnect();
            return;
        }

        std::string err;
        std::string fulljsonstr = buf->base;
        char lastchar = fulljsonstr[fulljsonstr.size() - 1];

        if (lastchar == ',') {
            fulljsonstr.pop_back();
        }

        if (lastchar != ']') {
            fulljsonstr.push_back(']');
        }

        if (fulljsonstr[0] != '[') {
            fulljsonstr.insert(0, "[");
        }

        std::vector<json11::Json> json = json11::Json::parse(fulljsonstr, err).array_items();

        if (err.length() == 0) {
            for (auto & s : json) {
                Network::Protocol::call_Callback(s);
            }
        }
    }

    static void connect_cb(uv_connect_t* req, int status) {
        if (status == 0) {
            uv_read_start(req->handle, alloc_cb, read_cb);
            Network::connected = true;
            Game::gameInit();
        } else {
            printf("Unable to create connection: Status %i\n", status);
        }
    }

    static void close_cb(uv_handle_s*) {
        Network::connected = false;
    }

    static void write_cb(uv_write_s*wr, int) {
        delete wr;
    }

    static void sendRaw(const char* data, unsigned long len) {    // Send raw data to server
        if (Network::connected) {
            uv_write_t *wr = new uv_write_t;
            uv_buf_t buf = uv_buf_init((char *) data, (unsigned int) len+1);
            buf.base[len] = ',';
            uv_write(wr, (uv_stream_t*) &Network::socket, &buf, 1, write_cb);
        } else {
            Game::gameOver();
        }
    }

    void send(const json11::Json& json) {
        Network::send(json.dump());
    }

    void send(const std::string& str) {
        Network::sendRaw(str.c_str(), str.size());
    }

    void connect(const char* addr, int port) {
        Network::connected = false;
        uv_tcp_init(uv_default_loop(), &socket);

        sockaddr dest;
        uv_ip4_addr(addr, port, (sockaddr_in*) &dest);

        uv_tcp_connect(&req, &socket, &dest, connect_cb);
    }

    void disconnect() {
        if (Network::connected) {
            if (uv_is_active((uv_handle_s*) &Network::socket) && !uv_is_closing((uv_handle_s*) &Network::socket)) {
                uv_close((uv_handle_s*) &Network::socket, close_cb);
            }

            uv_stop(uv_default_loop());
        }
    }
}
