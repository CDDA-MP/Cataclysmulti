#include <uv.h>
#include <stdio.h>
#include <stdlib.h>
#include "network.h"
#include "input.h"
#include "version.h"
#include "main.h"
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
    // TODO:Login
    Input::init();
}

void gameOver() // Call when disconnected
{
    IsGameOver = true;
    Network::disconnect();
    Input::end();
    exit(0);
}