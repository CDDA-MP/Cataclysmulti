#include "protocol.h"
#include <unordered_map>
#include <string>
typedef void (*Handler)(std::vector<char*>);
std::unordered_map<std::string, Handler> Handlers;

void handle_cmd(char* cmd, std::vector<char*> argv)
{
    Handler hnd = Handlers[cmd];
    if(hnd)
        hnd(argv);
}

static void printVersion(std::vector<char*> argv)
{
    puts(argv[0]);
}

void init_handlers()
{
    Handlers["TEST"] = printVersion;
}