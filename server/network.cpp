#include "network.h"

#include <json11.hpp>
#include <uv.h>
#include <vector>
#include <unordered_map>
#include <sstream>

#include "logger.h"
#include "player.h"
#include "protocol.h"

std::unordered_map <std::string, Player *> players;

namespace Network {
    uv_tcp_t socket;

    static void alloc_cb(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
        buf->base = new char[suggested_size];
        buf->len = suggested_size;
    }

    static void read_cb(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf) {
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
            std::string addrstr = Network::getAddress((uv_tcp_t*)stream);
            Player *player = players[addrstr];

            for (auto & s : json) {
                Network::Protocol::call_Callback(s,player);
            }
        }
    }

    static void write_cb(uv_write_s* wr, int) {
        delete wr;
    }

    static void sendRaw(const uv_tcp_t* dest, const char* data, unsigned long len) {
        uv_write_t *wr = new uv_write_t;
        uv_buf_t buf = uv_buf_init((char *) data, (unsigned int) len+1);
        buf.base[len] = ',';
        uv_write(wr, (uv_stream_t*)dest, &buf, 1, write_cb);
    }

    void send(const json11::Json& json,const Player& player) {
        Network::send(json.dump(),player);
    }

    void send(const std::string& str,const Player& player) {
        Network::sendRaw(player.handle,str.c_str(), str.size());
    }

    static void on_new_connection(uv_stream_t *server, int status) {
        if (status == -1) {
            // error!
            return;
        }

        uv_tcp_t *client = new uv_tcp_t;
        uv_tcp_init(uv_default_loop(), client);

        if (uv_accept(server, (uv_stream_t*) client) == 0) {
            uv_read_start((uv_stream_t*) client, alloc_cb, read_cb);

            std::string address = Network::getAddress(client);
            cout << "Connection [" << address << "] connected to the server." << endl;
            players[address] = new Player(client);
        } else {
            uv_close((uv_handle_t*) client, NULL);
        }
    }

    bool bind(const char *addr, int port) {
        uv_tcp_init(uv_default_loop(), &socket);

        sockaddr dest;
        uv_ip4_addr(addr, port, (sockaddr_in*) &dest);

        uv_tcp_bind(&socket, &dest, 0);
        return uv_listen((uv_stream_t*) &socket, 128, on_new_connection) != -1;
    }

    int getSockAddr(const uv_tcp_t *handle, sockaddr_in *_sockaddr) {
        int namelen = sizeof(sockaddr_in);
        return uv_tcp_getpeername(handle, (sockaddr*) _sockaddr, &namelen);
    }

    std::string getAddress(const uv_tcp_t *handle) {
        sockaddr_in addr;
        Network::getSockAddr(handle, &addr);
        std::stringstream ss;
        ss << inet_ntoa(addr.sin_addr) << ':' << (int)addr.sin_port;
        std::string str;
        ss >> str;
        return str;
    }

}