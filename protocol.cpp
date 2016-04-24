#include "protocol.h"
#include <unordered_map>
#include <json11.hpp>

#include "network.h"
#include "input.h"
#include "output.h"
using Network::Callback;
std::unordered_map<std::string, Callback> Callbacks;

namespace Network
{
void call_Callback(const json11::Json& json)
{
    CHECKMEMBER(json,"cmd");
    Callback t = Callbacks[json["cmd"].string_value()];
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
void init_Callbacks()
{
    Callbacks["printVersion"] = printVersion;
    Callbacks["msgbox"] = msgbox;
}
}
