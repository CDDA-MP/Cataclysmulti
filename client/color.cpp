#include "color.h"

#include <iostream>
#include <fstream>
#include <unordered_map>

#include "output.h"
#include "debug.h"
#include "input.h"
#include "translation.h"

std::unordered_map<std::string, nc_color> color_map;

void init_colors() {
    start_color();

    addColor("red", COLOR_RED, COLOR_BLACK, A_BOLD);
}

nc_color color_from_string(const std::string &color) {
    const nc_color col = color_map[color];

    if (col > 0) {
        return col;
    }

    return 0;
}

nc_color get_color_from_tag(const std::string &s, const nc_color base_color) {
    if (s.empty() || s[0] != '<' || s.substr(0, 8) == "</color>") {
        return base_color;
    }

    if (s.substr(0, 7) != "<color_") {
        return base_color;
    }

    size_t tag_close = s.find('>');

    if (tag_close == std::string::npos) {
        return base_color;
    }

    std::string color_name = s.substr(7, tag_close - 7);
    return color_from_string(color_name);
}

void addColor(const std::string& name, short fcolor, short bcolor, chtype bor) {
    static short color_pairs_id;
    init_pair(++color_pairs_id, fcolor, bcolor);
    color_map[name] = (int) COLOR_PAIR(color_pairs_id) | (int) bor;
}