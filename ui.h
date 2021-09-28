#ifndef UI_H
#define UI_H
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

void init_curses();
void draw_songView();
WINDOW *create_newwin(int height, int width, int starty, int startx);

#ifdef __cplusplus
}
#endif

#endif