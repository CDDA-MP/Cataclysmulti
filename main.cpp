#include <uv.h>
#include <stdio.h>
#include <stdlib.h>
#include "network.h"
#include "input.h"
#include "version.h"
#include "main.h"
bool IsGameOver;
void gameLoop(const char* address, int port);

int main(int argc, char* argv[])
{
    if(argc < 3) {
        puts(VERSION_NAME " " VERSION_PREFIX " " VERSION_VERSION "\n"
                          "Usage: cmulti <address> <port>\n");
        return 1;
    }
    IsGameOver = false;
    gameLoop(argv[1], atoi(argv[2]));
    gameOver();

    return 0;
}

void gameLoop(const char* address, int port)
{
    uv_loop_t* loop = uv_default_loop();

    Network::connect(loop, address, port);
    Input::init(loop);

    uv_run(loop, UV_RUN_DEFAULT);
}

void gameOver()
{
    IsGameOver = true;
    Network::disconnect();
    Input::end();
    exit(0);
}