#include <pthread.h>
#include <ncurses.h>
#include <stdlib.h>
#include <assert.h>
#include "input.h"
#include "main.h"
namespace Input
{
static pthread_t input_thread;

static void* input_loop(void* arg)
{
    while(!IsGameOver) {
        int ch = getch();
        if(ch != -1) {
            // TODO:Input Handle
            printw("tada:%i\n",ch);
            refresh();
        } else {
            gameOver();
        }
    }
}
void init()
{
    initscr();
    noecho();
    nodelay(stdscr, false); // Let stdin block.
    Input::start();
}
void start()//Start or restart getch loop thread
{
    int r = pthread_create(&input_thread, NULL,input_loop,NULL);
    assert(!r);
}
void pause()//Kill getch loop thread
{
    pthread_cancel(input_thread);
}
void end()
{
    endwin();
}
}