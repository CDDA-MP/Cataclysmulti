#include "protocol.h"

#include <json11.hpp>
#include <unordered_map>
#include <string>

#include "player.h"
#include "network.h"
#include "logger.h"

namespace Network {
    namespace Protocol {
        static void metadata(const json11::Json &json, Player &player) {
            if (!(json["client"].is_string() || json["ver"].is_string() || json["userid"].is_string())) {return ;}

            player.setInfo(json["userid"].string_value(),json["ver"].string_value(),json["client"].string_value());
            cout << Network::getAddress(player.handle) << " login with userid [" << player.userid << "]" << endl;
        }

        std::unordered_map<std::string, Network::Protocol::Callback> Callbacks {
            {"metadata", metadata },
        };

        inline void call_Callback(const json11::Json &json, Player &player) {
            call_Callback(json,&player);
        }
        void call_Callback(const json11::Json &json, Player *player) {
            if (!json["cmd"].is_string()) {
                return;
            }

            Network::Protocol::Callback t = Protocol::Callbacks[json["cmd"].string_value()];

            if (t) { t(json, *player); }
        }

        namespace Out {
            void colormap(const Player &player,const colormap_t &cmap) {
                json11::Json json = json11::Json::object {
                    {"cmd","colormap"},
                    {"map",cmap},
                };
                Network::send(json,player);
            }
            void msgbox(const Player &player,const std::string &str) {
                json11::Json json = json11::Json::object {
                    {"cmd","msgbox"},
                    {"str",str},
                };
                Network::send(json,player);
            }
        }
    }
}
