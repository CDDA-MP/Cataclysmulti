#include "protocol.h"
#include <unordered_map>
#include <json11.hpp>

#include "network.h"
#include "input.h"
#include "output.h"
#include "color.h"
std::unordered_map<std::string, Network::Callback> Callbacks;

namespace Network
{
void call_Callback(const json11::Json& json)
{
    CHECKMEMBER(json,"cmd");
    Network::Callback t = Callbacks[json["cmd"].string_value()];
    if(t) {
        t(json);
    }
}

static void printVersion(const json11::Json& json)
{
    puts(json["version"].string_value().c_str());
    Network::send(json);
}

static void msgbox(const json11::Json& json)
{
    UI::Msgbox(json["str"].string_value());
}

static void colormap(const json11::Json& json)
{
    CHECKMEMBER(json,"map");
    if (!json["map"].is_object()) return;

    std::map<std::string,json11::Json> map = json["map"].object_items();

    for (auto &i:map) {
        json11::Json::array array;
        if(i.second.is_array() && (array = i.second.array_items()).size() == 3 && array[0].is_number() && array[1].is_number() && array[2].is_number()) {
            addColor(i.first,(short)array[0].number_value(),(short)array[1].number_value(),(chtype)array[2].number_value());
        }
    }
}

#define RegCb(n,f) Callbacks[n] = f
void init_Callbacks()
{
    RegCb("printVersion",printVersion);
    RegCb("msgbox",msgbox);
    RegCb("colormap",colormap);

}
}
