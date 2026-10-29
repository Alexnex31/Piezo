#ifndef OPTIONS_H
#define OPTIONS_H

#include <SDL3/SDL.h>

/* Command line: piezo [song.ogg] [bpm] [offset_ms] */
typedef struct {
    const char *song_path;
    double bpm;
    double offset;      /* seconds */
} Options;

bool options_parse(Options *opt, int argc, char *argv[]);

#endif
