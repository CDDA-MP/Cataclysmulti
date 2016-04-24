#include "main.h"

#include <uv.h>
#include <json11.hpp>
#include <ncurses.h>

#include "network.h"
#include "input.h"
#include "output.h"
#include "version.h"

#include "game.h"

bool IsGameOver = false;
using namespace json11;
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

class SubInputHandler:public Input::InputHandler {
public:
    bool HandleInput(int key)
    {
        printw("Main Key Pressed[once]:%i",key);
        refresh();
        return true;
    }
};

void gameInit() // Call when connected.
{
    Input::init();
    Input::queue.push_back(new SubInputHandler);
    //send client metadata.
    Network::send(Json::object {
        {"cmd","metadata"},
        {"client",VERSION_NAME},
        {"ver",VERSION_PREFIX " " VERSION_VERSION},
        {"name",Game::name},
    });
}

void gameOver() // Call when disconnected
{
    IsGameOver = true;
    Network::disconnect();
    Input::end();
    exit(0);
}