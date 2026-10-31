#ifndef APP_H
#define APP_H

#include <SDL3/SDL.h>
#include "audio/music.h"
#include "audio/clock.h"
#include "game/calibration.h"
#include "game/chart.h"
#include "game/game.h"
#include "game/metronome.h"

typedef enum {
    MODE_PLAY,          /* a chart */
    MODE_CALIBRATE      /* tapping on clicks to find the global offset */
} AppMode;

/* Everything the game owns, shared by the SDL callbacks in main.c. */
typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    AppMode mode;
    Music music;            /* the song, or the clicks when calibrating */
    SongClock clock;
    Chart chart;            /* MODE_PLAY only */
    Game game;              /* MODE_PLAY only */
    Calibration calibration;    /* MODE_CALIBRATE only */
    Metronome metronome;
    double global_offset;   /* seconds, +/- keys: shifts drawing and judging */
    double saved_offset;    /* what the config file holds */
    double scroll_speed;    /* pixels per second */
    double raw_time;        /* last raw audio time, refreshed by app_update */
    bool finished;          /* song fully played */
} App;

bool app_init(App *app, int argc, char *argv[]);
void app_update(App *app);
void app_quit(App *app);
/* From the results / calibration end screen: the same again, from the top. */
void app_restart(App *app);

/* Song time the game runs on: the smoothed clock minus the global offset. */
double app_song_time(const App *app);
/* Same, at an exact timestamp (ns), e.g. a key press. */
double app_song_time_at(const App *app, Uint64 ns);

#endif
