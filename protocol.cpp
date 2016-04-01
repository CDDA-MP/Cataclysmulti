#include "protocol.h"
#include <unordered_map>
#include <string>
using namespace Network;
std::unordered_map<std::string, Interface> Interfaces;
namespace Network
{
void call_interface(char* cmd, std::vector<char*> argv)
{
    Interface t = Interfaces[cmd];
    if(t)
        t(argv);
}

static void printVersion(std::vector<char*> argv)
{
    puts(argv[0]);
}

void init_interfaces()
{
    Interfaces["TEST"] = printVersion;
}
}