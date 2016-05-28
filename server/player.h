#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <uv.h>
#include <string>
class Player {
    public:
        //Player Infomations
        std::string userid;
        std::string version;
        std::string client;
        const uv_tcp_t *handle;
        Player(const uv_tcp_t *addr);
        void setInfo(const std::string &_userid,const std::string &_ver,const std::string &_client);
    private:

};

#endif