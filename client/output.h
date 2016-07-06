#ifndef _OUTPUT_H_
#define _OUTPUT_H_

#include <string>
#include <ncurses.h>
#include <vector>

#include "color.h"
// *****From Cataclysm-DDA:output.h***** //

//      LINE_NESW  - X for on, O for off
#define LINE_XOXO 4194424 // '|'   Vertical line. ncurses: ACS_VLINE; Unicode: U+2502
#define LINE_OXOX 4194417 // '-'   Horizontal line. ncurses: ACS_HLINE; Unicode: U+2500
#define LINE_XXOO 4194413 // '|_'  Lower left corner. ncurses: ACS_LLCORNER; Unicode: U+2514
#define LINE_OXXO 4194412 // '|^'  Upper left corner. ncurses: ACS_ULCORNER; Unicode: U+250C
#define LINE_OOXX 4194411 // '^|'  Upper right corner. ncurses: ACS_URCORNER; Unicode: U+2510
#define LINE_XOOX 4194410 // '_|'  Lower right corner. ncurses: ACS_LRCORNER; Unicode: U+2518
#define LINE_XXXO 4194420 // '|-'  Tee pointing right. ncurses: ACS_LTEE; Unicode: U+251C
#define LINE_XXOX 4194422 // '_|_' Tee pointing up. ncurses: ACS_BTEE; Unicode: U+2534
#define LINE_XOXX 4194421 // '-|'  Tee pointing left. ncurses: ACS_RTEE; Unicode: U+2524
#define LINE_OXXX 4194423 // '^|^' Tee pointing down. ncurses: ACS_TTEE; Unicode: U+252C
#define LINE_XXXX 4194414 // '-|-' Large Plus or cross over. ncurses: ACS_PLUS; Unicode: U+253C

#define LINE_XOXO_C 0xa0
#define LINE_OXOX_C 0xa1
#define LINE_XXOO_C 0xa2
#define LINE_OXXO_C 0xa3
#define LINE_OOXX_C 0xa4
#define LINE_XOOX_C 0xa5
#define LINE_XXXO_C 0xa6
#define LINE_XXOX_C 0xa7
#define LINE_XOXX_C 0xa8
#define LINE_OXXX_C 0xa9
#define LINE_XXXX_C 0xaa

std::vector<size_t> get_tag_positions(const std::string &s);
std::string word_rewrap(const std::string &in, int width);
void mvprintz(int y, int x, nc_color FG, const char *mes, ...);
void mvwprintz(WINDOW *w, int y, int x, nc_color FG, const char *mes, ...);
void printz(nc_color FG, const char *mes, ...);
void wprintz(WINDOW *w, nc_color FG, const char *mes, ...);
void draw_border(WINDOW *w, nc_color FG);
std::string vstring_format(char const *format, va_list args);
std::vector<std::string> split_by_color(const std::string &s);
std::string rm_prefix(std::string str, char c1, char c2);
std::string rm_prefix(std::string str);
void print_colored_text(WINDOW *w, int y, int x, nc_color &color, nc_color base_color, const std::string &text);
std::vector<std::string> foldstring(std::string str, int width);
int fold_and_print(WINDOW *w, int begin_y, int begin_x, int width, nc_color base_color,
                   const std::string &text);


// *****Cataclysmulti UI***** //

namespace UI
{
    void Msgbox(std::string str);
}
#endif
