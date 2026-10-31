#ifndef CHART_H
#define CHART_H

#include <SDL3/SDL.h>

#define CHART_COLUMNS 4

/*
 * Chart file, one entry per line, '#' starts a comment:
 *
 *     song   click120.ogg    # relative to the chart's folder
 *     bpm    120
 *     offset 0               # ms, time of the first beat
 *     0    1                 # note: time in ms, column 1 to 4
 *     500  2
 *
 * Note times are absolute song times: the offset only places the beat grid.
 */
typedef struct {
    double time;        /* seconds */
    int column;         /* 0 to CHART_COLUMNS - 1 */
} Note;

typedef struct {
    char *song_path;
    double bpm;
    double offset;      /* seconds */
    Note *notes;        /* sorted by time */
    int count;
    int capacity;
} Chart;

bool chart_load(Chart *c, const char *path);
void chart_free(Chart *c);

#endif
