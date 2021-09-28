#include "ui.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer_ext.h>
#include <ncurses.h>
#include <pthread.h>
#include <string.h>

#include "shellgui.h"

/*  Routine to set up ncurses
    Turns off line buffering, removes cursor and echoing
*/
void init_curses() {
    initscr();
    raw();
    noecho();
    curs_set(0);
    draw_songView();
}

void draw_songView() {
    int result = 0;
    int flags = MIX_INIT_MP3;

    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        exit(1);
    }

    if (flags != (result = Mix_Init(flags))) {
        printf("Could not initialize mixer (result: %d).\n", result);
        printf("Mix_Init: %s\n", Mix_GetError());
        exit(1);
    }

    Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640);

    printw("w to increase volume\n");
    printw("s to decrease volume\n");
    printw("Space to pause/play\n");
    printw("a/d Arrow to change the current time stamp\n");
    printw("x to exit\n");

    pthread_t songThread;
    pthread_t inputThread;
    pthread_create(&songThread, NULL, songView, NULL);
    pthread_create(&inputThread, NULL, input, NULL);
    pthread_join(songThread, NULL);
    pthread_join(inputThread, NULL);

    refresh();
    return;
}

int main() {
    init_curses();
    return 0;
}