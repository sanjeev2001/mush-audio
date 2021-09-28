#include "shellgui.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer_ext.h>
#include <ncurses.h>
#include <pthread.h>
#include <stdio.h>

Mix_Music *music;
WINDOW *nowPlaying;
const char *strings[] = {"eterna.mp3", "promise.mp3"};

void *songView(void *vargp) {
    music = Mix_LoadMUS(strings[1]);
    for (int i = 1; i < 2; i++) {
        Mix_PlayMusic(music, 1);

        int startx, starty, width, height;
        height = 4;
        width = getmaxx(stdscr);
        startx = 0;
        starty = getmaxy(stdscr) - 4;
        nowPlaying = newwin(height, width, starty, startx);
        refresh();
        box(nowPlaying, 0, 0);
        const char *title = Mix_GetMusicTitle(music);
        double length = Mix_GetMusicTotalTime(music);
        int totalMin = (length / 60);
        int totalSecond = (int)length - 60 * totalMin;
        int currentMin = 0;
        int currentSec = 0;
        int c = 0;
        int t = 0;
        char seconds[2];
        mvwprintw(nowPlaying, 2, getmaxx(stdscr) - 13, " Volume: 50%%");
        while (Mix_GetMusicPosition(music) < Mix_GetMusicTotalTime(music)) {
            currentSec = (int)Mix_GetMusicPosition(music) % 60;
            currentMin = (int)Mix_GetMusicPosition(music) / 60;
            if (currentSec < 10) {
                seconds[0] = '0';
                seconds[1] = currentSec + '0';
            } else {
                seconds[0] = currentSec / 10 + '0';
                seconds[1] = currentSec % 10 + '0';
            }
            mvwprintw(nowPlaying, 1, 1, "Playing: %s", title);
            mvwprintw(nowPlaying, 1, width - 14, "[%d:%s / %d:%d]", currentMin,
                      seconds, totalMin, totalSecond);
            wrefresh(nowPlaying);
        }
    }
}

void *input(void *vargp) {
    int ch = getch();
    int volume = MIX_MAX_VOLUME / 2;
    double time;
    int volumePercentage = 50;
    bool paused = FALSE;

    while (ch != 'x') {
        if (ch == 'w') {
            volume = volume + 8;
            if (volume > 128) volume = 128;
            Mix_VolumeMusic(volume);
            volumePercentage = (volume / 128.0) * 100;
            if (volumePercentage == 100) {
                mvwprintw(nowPlaying, 2, getmaxx(stdscr) - 13, "Volume: %d%%",
                          volumePercentage);
            } else if (volumePercentage < 10) {
                mvwprintw(nowPlaying, 2, getmaxx(stdscr) - 13, "  Volume: %d%%",
                          volumePercentage);
            } else {
                mvwprintw(nowPlaying, 2, getmaxx(stdscr) - 13, " Volume: %d%%",
                          volumePercentage);
            }
        } else if (ch == 's') {
            volume = volume - 10;
            if (volume < 0) volume = 0;
            Mix_VolumeMusic(volume);
            volumePercentage = (volume / 128.0) * 100;
            if (volumePercentage == 100) {
                mvwprintw(nowPlaying, 2, getmaxx(stdscr) - 13, "Volume: %d%%",
                          volumePercentage);
            } else if (volumePercentage < 10) {
                mvwprintw(nowPlaying, 2, getmaxx(stdscr) - 13, "  Volume: %d%%",
                          volumePercentage);
            } else {
                mvwprintw(nowPlaying, 2, getmaxx(stdscr) - 13, " Volume: %d%%",
                          volumePercentage);
            }
        } else if (ch == ' ') {
            if (paused == FALSE) {
                paused = TRUE;
                Mix_PauseMusic();
                mvwprintw(nowPlaying, 2, 1, "Paused  ");
            } else {
                paused = FALSE;
                Mix_ResumeMusic();
                mvwprintw(nowPlaying, 2, 1, "        ");
            }
        } else if (ch == 'a') {
            if (Mix_GetMusicPosition(music) > 5.0) {
                Mix_SetMusicPosition(Mix_GetMusicPosition(music) - 5.0);
            } else {
                Mix_SetMusicPosition(0);
            }
        } else if (ch == 'd') {
            if (Mix_GetMusicPosition(music) <
                Mix_GetMusicTotalTime(music) - 5) {
                Mix_SetMusicPosition(Mix_GetMusicPosition(music) + 5.0);
            } else {
                Mix_SetMusicPosition(Mix_GetMusicTotalTime(music));
            }
        }
        ch = getch();
    }
    system("clear");
    exit(0);
}
