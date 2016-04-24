#include "input.h"

#include <pthread.h>
#include <ncurses.h>
#include <assert.h>
#include <vector>

#include "main.h"
#include "game.h"
namespace Input
{
static pthread_t InputThread;
std::vector<InputHandler*> queue;
static void* InputLoop(void* arg)
{
    while(!IsGameOver) {
        int key = getch();
        if(key != -1) {
            if(queue.empty()) {
                Game::HandleInput(key);
            } else if(queue[0]->HandleInput(key)) {
                delete queue[0];
                queue.erase(queue.begin());
            }
        } else {
            gameOver();
        }
    }
}

void init()
{
    initscr();
    noecho();
    nodelay(stdscr, false); // make stdin block.
    int r = pthread_create(&InputThread, NULL,InputLoop,NULL);
    assert(!r);
}
void end()
{
    endwin();
}
}