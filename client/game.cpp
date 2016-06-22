#include "game.h"

#include <ncurses.h>
#include <json11.hpp>

#include "input.h"
#include "network.h"
#include "version.h"
#include "main.h"
#include "color.h"
#include "protocol.h"

namespace Game {
    bool IsGameOver = false;

// Player Infomation
    char* userid;

    bool HandleInput(int key) {
        printw("Game Key Pressed:%i", key);
        refresh();
        return false;
    }

    void gameInit() { // Call when connected.
        Input::init();
    }

    void gameOver() { // Call when disconnected
        Game::IsGameOver = true;
        Network::disconnect();
        Input::end();
        exit(0);
    }
}