#include "input.h"

#include <pthread.h>
#include <ncurses.h>
#include <assert.h>
#include <locale.h>
#include <vector>

#include "main.h"
#include "game.h"
#include "color.h"


namespace Input
{
InputHandler::InputHandler() {}
InputHandler::~InputHandler() {}
void InputHandler::Init() {}
bool InputHandler::HandleInput(int key) {
    return true;
}
static pthread_t InputThread;
std::vector<InputHandler*> queue;
static void* InputLoop(void* arg)
{
    while(!Game::IsGameOver) {
        int key = getch();
        if(key != -1) {
            if(queue.empty()) {
                Game::HandleInput(key);
            } else {
                if(queue[0]->HandleInput(key)) {
                    delete queue[0];
                    queue.erase(queue.begin());
                }
            }
        } else {
            Game::gameOver();
        }
        if(!queue.empty() && !queue[0]->Inited)
        {
            queue[0]->Init();
            queue[0]->Inited = true;
        }
    }
    return NULL;
}

void init()
{
    setlocale(LC_ALL,"");
    initscr();
    init_colors();
    noecho();
    nodelay(stdscr, false); // make stdin block.
    curs_set(0);
    int r = pthread_create(&InputThread, NULL,InputLoop,NULL);
    assert(!r);
}

void end()
{
    endwin();
}

void queue_pushback(InputHandler* handler)
{
    queue.push_back(handler);
    if (queue.size() == 1) {
        handler->Init();
        handler->Inited = true;
    }
}
}