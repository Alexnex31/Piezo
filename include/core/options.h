#ifndef OPTIONS_H
#define OPTIONS_H

#include <SDL3/SDL.h>

/* Command line: piezo [chart]  or  piezo --calibrate */
typedef struct {
    const char *chart_path;
    bool calibrate;
} Options;

bool options_parse(Options *opt, int argc, char *argv[]);

#endif
