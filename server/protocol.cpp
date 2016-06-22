#include "protocol.h"

#include <json11.hpp>
#include <unordered_map>
#include <string>

#include "player.h"
#include "network.h"
#include "logger.h"
#include "version.h"

namespace Network {
    namespace Protocol {
        static void metadata(const json11::Json &json, Player &player) {
            if (!(json["client"].is_string() || json["strver"].is_string() || json["num"].is_number())) {return ;}

            player.setInfo(json["client"].string_value(),json["ver"].string_value());
            Network::Protocol::Out::sendmeta(player);
        }
        static void info(const json11::Json &json, Player &player) {
            if (!json["userid"].is_string()) {return ;}

            //TODO:Check userid.
            player.setUserID(json["userid"].string_value());

            player.joinGame();
        }

        //Callback list
        std::unordered_map<std::string, Network::Protocol::Callback> Callbacks {
            {"metadata", metadata },
            {"info",info},
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
            void sendmeta(const Player&player) {
                Network::send(json11::Json::object {
                    {"cmd", "metadata" },
                    {"client", Version::name},
                    {"strver", Version::type + ' ' + Version::version },
                    {"num",Version::number},
                },player);
            }

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
