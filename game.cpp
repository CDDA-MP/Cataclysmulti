#include "game.h"

#include <ncurses.h>
namespace Game
{
char* name;
bool HandleInput(int key)
{
    printw("Game Key Pressed:%i",key);
    refresh();
    return false;
}
}