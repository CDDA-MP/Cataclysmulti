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
        bool queue_empty = queue.empty();
        if(!queue_empty && !queue[0]->Inited)
        {
            queue[0]->Init();
            queue[0]->Inited = true;
        }
        int key = getch();
        if(key != -1) {
            if(queue_empty) {
                Game::HandleInput(key);
            } else {
                if(queue[0]->HandleInput(key)) {
                    delete queue[0];
                    queue.erase(queue.begin());
                }
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