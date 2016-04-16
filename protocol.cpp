#include "protocol.h"
#include <unordered_map>
#include <rapidjson/document.h>

#include "network.h"
using namespace Network;
std::unordered_map<std::string, Interface> Interfaces;

namespace Network
{
void call_interface(const rapidjson::Document& dom)
{
    CHECKMEMBER(dom,"cmd")
    Interface t = Interfaces[dom["cmd"].GetString()];
    if(t) {
        t(dom);
    }
}

static void printVersion(const rapidjson::Document& dom)
{
    puts(dom["version"].GetString());
    Network::send(dom);
}

void init_interfaces()
{
    Interfaces["printVersion"] = printVersion;
}
}