#include "main.h"

#include <uv.h>

#include "network.h"
#include "version.h"

#include "game.h"

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
    Game::gameOver();

    return 0;
}