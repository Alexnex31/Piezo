#ifndef APP_H
#define APP_H

#include <SDL3/SDL.h>
#include "audio/music.h"
#include "audio/clock.h"
#include "game/metronome.h"

/* Everything the game owns, shared by the SDL callbacks in main.c. */
typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    Music music;
    SongClock clock;
    Metronome metronome;
    double raw_time;    /* last raw audio time, refreshed by app_update */
    bool finished;      /* song fully played */
} App;

bool app_init(App *app, int argc, char *argv[]);
void app_update(App *app);
void app_quit(App *app);

#endif
