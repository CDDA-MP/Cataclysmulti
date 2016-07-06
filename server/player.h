#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <uv.h>
#include <string>
class Player
{
private:
    //Player Infomations
    std::string m_userid;
    std::string m_version;
    std::string m_client;
    uv_tcp_t *m_handle;
public:
    Player(uv_tcp_t *addr);
    void setInfo(const std::string &client,const std::string &ver);
    void setUserID(const std::string &userid);

    inline uv_tcp_t *getHandle() const {return m_handle;}
    inline const std::string& getUserID() const {return m_userid;}
    inline const std::string& getVersion() const {return m_version;}
    inline const std::string& getClient() const {return m_client;}

    void joinGame();
};

#endif