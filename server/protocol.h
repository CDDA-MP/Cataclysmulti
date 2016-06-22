#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

#include <json11.hpp>

#include "player.h"

namespace Network {
    namespace Protocol {
        typedef void (*Callback)(const json11::Json &json,Player &player);
        inline void call_Callback(const json11::Json &json,Player &player);
        void call_Callback(const json11::Json &json, Player *player);
        namespace Out {
            void sendmeta(const Player&player);
            typedef std::map<std::string,std::array<int,3>> colormap_t;
            void colormap(const Player &player,const colormap_t &cmap);
            void msgbox(const Player &player,const std::string &str);
        }
    }
}

#endif