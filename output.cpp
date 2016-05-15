#include "output.h"

#include <string>
#include <sstream>
#include <ncurses.h>
#include <algorithm>
#include <stdarg.h>

#include "input.h"
#include "catacharset.h"
#include "color.h"

std::vector<size_t> get_tag_positions(const std::string &s)
{
    std::vector<size_t> ret;
    size_t pos = s.find("<color_", 0, 7);
    while (pos != std::string::npos) {
        ret.push_back(pos);
        pos = s.find("<color_", pos + 1, 7);
    }
    pos = s.find("</color>", 0, 8);
    while (pos != std::string::npos) {
        ret.push_back(pos);
        pos = s.find("</color>", pos + 1, 8);
    }
    std::sort(ret.begin(), ret.end());
    return ret;
}

std::string word_rewrap (const std::string &in, int width)
{
    std::ostringstream o;

    // find non-printing tags
    std::vector<size_t> tag_positions = get_tag_positions(in);

    int lastwb  = 0; //last word break
    int lastout = 0;
    const char *instr = in.c_str();
    bool skipping_tag = false;

    for (int j = 0, x = 0; j < (int)in.size(); ) {
        const char *ins = instr + j;
        int len = ANY_LENGTH;
        uint32_t uc = UTF8_getch(&ins, &len);

        if (uc == '<') { // maybe skip non-printing tag
            std::vector<size_t>::iterator it;
            for (it = tag_positions.begin(); it != tag_positions.end(); ++it) {
                if ((int)*it == j) {
                    skipping_tag = true;
                    break;
                }
            }
        }

        j += ANY_LENGTH - len;

        if (skipping_tag) {
            if (uc == '>') {
                skipping_tag = false;
            }
            continue;
        }

        x += mk_wcwidth(uc);

        if (x > width) {
            if (lastwb == lastout) {
                lastwb = j;
            }
            for(int k = lastout; k < lastwb; k++) {
                o << in[k];
            }
            o << '\n';
            x = 0;
            lastout = j = lastwb;
        }

        if (uc == ' ' || uc >= 0x2E80) {
            lastwb = j;
        }
    }
    for (int k = lastout; k < (int)in.size(); k++) {
        o << in[k];
    }

    return o.str();
}

void mvprintz(int y, int x, nc_color FG, const char *mes, ...)
{
    va_list ap;
    va_start(ap, mes);
    const std::string text = vstring_format(mes, ap);
    va_end(ap);
    attron(FG);
    mvprintw(y, x, "%s", text.c_str());
    attroff(FG);
}

void mvwprintz(WINDOW *w, int y, int x, nc_color FG, const char *mes, ...)
{
    va_list ap;
    va_start(ap, mes);
    const std::string text = vstring_format(mes, ap);
    va_end(ap);
    wattron(w, FG);
    mvwprintw(w, y, x, "%s", text.c_str());
    wattroff(w, FG);
}

void printz(nc_color FG, const char *mes, ...)
{
    va_list ap;
    va_start(ap, mes);
    const std::string text = vstring_format(mes, ap);
    va_end(ap);
    attron(FG);
    printw("%s", text.c_str());
    attroff(FG);
}

void wprintz(WINDOW *w, nc_color FG, const char *mes, ...)
{
    va_list ap;
    va_start(ap, mes);
    const std::string text = vstring_format(mes, ap);
    va_end(ap);
    wattron(w, FG);
    wprintw(w, "%s", text.c_str());
    wattroff(w, FG);
}

std::string vstring_format(char const *format, va_list args)
{
    errno = 0; // Clear errno before trying
    std::vector<char> buffer(1024, '\0');

#if (defined __CYGWIN__)
    std::string rewritten_format = rewrite_vsnprintf(format);
    format = rewritten_format.c_str();
#endif

    for (;;) {
        size_t const buffer_size = buffer.size();

        va_list args_copy;
        va_copy(args_copy, args);
        int const result = vsnprintf(&buffer[0], buffer_size, format, args_copy);
        va_end(args_copy);

        // No error, and the buffer is big enough; we're done.
        if (result >= 0 && static_cast<size_t>(result) < buffer_size) {
            break;
        }

        // Standards conformant versions return -1 on error only.
        // Some non-standard versions return -1 to indicate a bigger buffer is needed.
        // Some of the latter set errno to ERANGE at the same time.
        if (result < 0 && errno && errno != ERANGE) {
            return std::string("Bad format string for printf.");
        }

        // Looks like we need to grow... bigger, definitely bigger.
        buffer.resize(buffer_size * 2);
    }

    return std::string(&buffer[0]);
}

std::vector<std::string> split_by_color(const std::string &s)
{
    std::vector<std::string> ret;
    std::vector<size_t> tag_positions = get_tag_positions(s);
    size_t last_pos = 0;
    std::vector<size_t>::iterator it;
    for (it = tag_positions.begin(); it != tag_positions.end(); ++it) {
        ret.push_back(s.substr(last_pos, *it - last_pos));
        last_pos = *it;
    }
    // and the last (or only) one
    ret.push_back(s.substr(last_pos, std::string::npos));
    return ret;
}


//remove prefix of a strng, between c1 and c2, ie, "<prefix>remove it"
std::string rm_prefix(std::string str, char c1, char c2)
{
    if(!str.empty() && str[0] == c1) {
        size_t pos = str.find_first_of(c2);
        if(pos != std::string::npos) {
            str = str.substr(pos + 1);
        }
    }
    return str;
}

std::string rm_prefix(std::string str)
{
    return rm_prefix(str,'<','>');
}
std::string remove_color_tags(const std::string &s)
{
    std::string ret;
    std::vector<size_t> tag_positions = get_tag_positions(s);
    size_t next_pos = 0;

    if ( tag_positions.size() > 1 ) {
        for (size_t i = 0; i < tag_positions.size(); ++i) {
            ret += s.substr(next_pos, tag_positions[i] - next_pos);
            next_pos = s.find(">", tag_positions[i], 1) + 1;
        }

        ret += s.substr(next_pos, std::string::npos);
    } else {
        return s;
    }
    return ret;
}

void print_colored_text( WINDOW *w, int y, int x, nc_color &color, nc_color base_color, const std::string &text )
{
    wmove( w, y, x );
    const auto color_segments = split_by_color( text );
    for( auto seg : color_segments ) {
        if( seg.empty() ) {
            continue;
        }

        if( seg[0] == '<' ) {
            color = get_color_from_tag( seg, base_color );
            seg = rm_prefix( seg );
        }

        wprintz( w, color, "%s", seg.c_str() );
    }
}

std::vector<std::string> foldstring ( std::string str, int width )
{
    std::vector<std::string> lines;
    if ( width < 1 ) {
        lines.push_back( str );
        return lines;
    }
    std::stringstream sstr(str);
    std::string strline;
    while (std::getline(sstr, strline, '\n')) {
        std::string wrapped = word_rewrap(strline, width);
        std::stringstream swrapped(wrapped);
        std::string wline;
        while (std::getline(swrapped, wline, '\n')) {
            lines.push_back(wline);
        }
    }
    return lines;
}

// returns number of printed lines
int fold_and_print(WINDOW *w, int begin_y, int begin_x, int width, nc_color base_color,
                   const std::string &text)
{
    nc_color color = base_color;
    std::vector<std::string> textformatted;
    textformatted = foldstring(text, width);
    for( int line_num = 0; (size_t)line_num < textformatted.size(); line_num++) {
        print_colored_text( w, line_num + begin_y, begin_x, color, base_color, textformatted[line_num] );
    }
    return textformatted.size();
}

void draw_border(WINDOW *w, nc_color FG)
{
    wattron(w, FG);
    wborder(w, LINE_XOXO, LINE_XOXO, LINE_OXOX, LINE_OXOX,
            LINE_OXXO, LINE_OOXX, LINE_XXOO, LINE_XOOX );
    wattroff(w, FG);
}


namespace UI {
class MsgboxInputHandler:public Input::InputHandler {
public:
    MsgboxInputHandler(std::string& _str) {
        str = _str;
    }

    void Init() {
        std::vector<std::string> textformatted = foldstring(str,getmaxx(stdscr)-2);
        int winwidth = 0;
        for(auto &s:textformatted) { //Get the width of the longest line.
            winwidth = std::max(winwidth,utf8_width(remove_color_tags(s)));
        }
        int winheight = (int)textformatted.size();
        w = newwin(winheight+2,winwidth+2,(getmaxy(stdscr)-winheight)/2,(getmaxx(stdscr)-winwidth)/2);
        draw_border(w,0);

        fold_and_print(w,1,1,winwidth,0,str);
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
    Input::queue_pushback(new MsgboxInputHandler(str));
}
}