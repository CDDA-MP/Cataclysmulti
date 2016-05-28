#ifndef _COLOR_H_
#define _COLOR_H_

#include <ncurses.h>
#include <string>

typedef int nc_color;

void init_colors();
nc_color color_from_string(const std::string &color);
nc_color get_color_from_tag(const std::string &s, const nc_color base_color);

void addColor(const std::string& name, short fcolor, short bcolor, chtype bor);    //Add a new color to color_map.
#endif