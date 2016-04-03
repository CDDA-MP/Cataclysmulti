#include <uv.h>
#include <ncurses.h>
#include <stdlib.h>
#include <assert.h>
#include "input.h"
#include "main.h"
namespace Input
{
static uv_thread_t input_thread;

static void input_loop(void* arg)
{
    while(!IsGameOver) {
        int ch = getch();
        if(ch != -1) {
            // TODO:Input Handle
        } else {
            gameOver();
        }
    }
}
void init(uv_loop_t* loop)
{
    initscr();
    noecho();
    nodelay(stdscr, false); // Let stdin block.
    int r = uv_thread_create(&input_thread, input_loop, NULL);
    assert(!r);
    uv_thread_join(&input_thread);
}
void end()
{
    endwin();
}
}