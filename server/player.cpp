#include "player.h"

#include <uv.h>
#include <map>
#include <array>
#include <json11.hpp>

#include "protocol.h"
#include "logger.h"
#include "network.h"

static const Network::Protocol::Out::colormap_t cmap = {
    {"custom",{1,3,0}},
    {"another",{3,2,0}},
};
Player::Player(const uv_tcp_t *client) {
    handle = client;
}
void Player::setInfo(const std::string &_userid,const std::string &_ver,const std::string &_client) {
    userid = _userid;
    version = _ver;
    client = _client;
    Network::Protocol::Out::colormap(*this,cmap);
    Network::Protocol::Out::msgbox(*this,"Hello,<color_red>" + userid + "</color>!");
    Network::Protocol::Out::msgbox(*this,"I am the <color_custom>cataclysmulti</color> server!\nWould you like some <color_another>cakes</color>?");
}