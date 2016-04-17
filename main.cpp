#include "main.h"

#include <uv.h>
#include <rapidjson/document.h>

#include "network.h"
#include "input.h"
#include "output.h"
#include "version.h"

#include "game.h"

bool IsGameOver = false;

int main(int argc, char* argv[])
{
    if(argc < 4) {
        puts(VERSION_NAME " " VERSION_PREFIX " " VERSION_VERSION "\n"
             "Usage: cmulti <address> <port> <name>\n");
        return 1;
    }
    Game::name = argv[3];
    Network::connect(argv[1], atoi(argv[2]));
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
    gameOver();

    return 0;
}

void gameInit() // Call when connected.
{
    Input::init();

    //send client metadata.
    rapidjson::Document dom;
    dom.SetObject();
    dom.AddMember("cmd","metadata",dom.GetAllocator());
    dom.AddMember("client",VERSION_NAME,dom.GetAllocator());
    dom.AddMember("ver",VERSION_PREFIX " " VERSION_VERSION,dom.GetAllocator());
    Network::send(dom);
}

void gameOver() // Call when disconnected
{
    IsGameOver = true;
    Network::disconnect();
    Input::end();
    exit(0);
}