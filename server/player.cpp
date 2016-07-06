#include "player.h"

#include <uv.h>
#include <map>
#include <array>
#include <json11.hpp>

#include "protocol.h"
#include "logger.h"
#include "network.h"

static const Network::Protocol::Out::colormap_t cmap =
{
    {"custom",{{1,3,0}}},
    {"another",{{3,2,0}}},
};

Player::Player(uv_tcp_t *client)
{
    m_handle = client;
}

void Player::setInfo(const std::string &client,const std::string &ver)
{
    m_version = ver;
    m_client = client;
}

void Player::setUserID(const std::string &userid)
{
    m_userid = userid;
}

void Player::joinGame()
{
    Network::Protocol::Out::colormap(*this,cmap);
    Network::Protocol::Out::msgbox(*this,"Hello,<color_red>" + m_userid + "</color>!");
    Network::Protocol::Out::msgbox(*this,"I am the <color_custom>cataclysmulti</color> server!\nSo many <color_another>colors</color>!");
}