#include "main.h"

#include <stdlib.h>
#include <uv.h>
#include <iostream>

#include "network.h"
#include "version.h"

#include "game.h"

int main(int argc, char* argv[])
{
    if (argc < 4)
    {
        std::cout << Version::name << " " << Version::type << " " << Version::version << std::endl
                  << "Usage: cmulti <address> <port> <userid>" << std::endl;
        return 1;
    }

    Game::userid = argv[3];
    Network::connect(argv[1], atoi(argv[2]));
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
    Game::gameOver();

    return 0;
}