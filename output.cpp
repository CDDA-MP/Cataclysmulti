#include "output.h"

#include <string>
#include <ncurses.h>

#include "input.h"
namespace UI {
void draw_border(WINDOW *w, nc_color FG)
{
    wattron(w, FG);
    wborder(w, LINE_XOXO, LINE_XOXO, LINE_OXOX, LINE_OXOX,
            LINE_OXXO, LINE_OOXX, LINE_XXOO, LINE_XOOX );
    wattroff(w, FG);
}

class MsgboxInputHandler:public Input::InputHandler {
public:
    MsgboxInputHandler(std::string& _str) {
        str = _str;
    }

    void Init() {
        unsigned long winwidth = str.size()+2;
        unsigned long winheight = 3;//Only 1 line string supported now.
        w = newwin(winheight,winwidth,(getmaxy(stdscr)-winheight)/2,(getmaxx(stdscr)-winwidth)/2);
        init_pair(0,COLOR_BLACK,COLOR_WHITE);
        UI::draw_border(w,0);

        mvwprintw(w,1,1,str.c_str());
        wrefresh(w);
    }

    bool HandleInput(int key) {
        if(key == '\n') {
            werase(w);
            wrefresh(w);
            delwin(w);
            return true;
        }
        return false;
    }
private:
    WINDOW *w;
    std::string str;
};

void Msgbox(std::string str)
{

    Input::queue.push_back(new MsgboxInputHandler(str));
}
}