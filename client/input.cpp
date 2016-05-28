#include "input.h"

#include <pthread.h>
#include <ncurses.h>
#include <assert.h>
#include <locale.h>
#include <queue>

#include "main.h"
#include "game.h"
#include "color.h"


namespace Input {
    InputHandler::InputHandler() {}
    InputHandler::~InputHandler() {}
    void InputHandler::Init() {}
    bool InputHandler::HandleInput(int key) {
        return true;
    }
    static pthread_t InputThread;
    std::queue<InputHandler*> iqueue;
    static void* InputLoop(void* arg) {
        while (!Game::IsGameOver) {
            int key = getch();

            if (key != -1) {
                if (iqueue.empty()) {
                    Game::HandleInput(key);
                } else if (iqueue.front()->HandleInput(key)) {
                    delete iqueue.front();
                    iqueue.pop();
                }
            } else {
                Game::gameOver();
            }

            if (!iqueue.empty() && !iqueue.front()->Inited) {
                iqueue.front()->Init();
                iqueue.front()->Inited = true;
            }
        }

        return NULL;
    }

    void init() {
        setlocale(LC_ALL, "");
        initscr();
        init_colors();
        noecho();
        nodelay(stdscr, false);    // make stdin block.
        curs_set(0);
        int r = pthread_create(&InputThread, NULL, InputLoop, NULL);
        assert(!r);
    }

    void end() {
        endwin();
    }

    void queue_pushback(InputHandler* handler) {
        iqueue.push(handler);

        if (iqueue.size() == 1) {
            handler->Init();
            handler->Inited = true;
        }
    }
}