#ifndef CLOCK_H
#define CLOCK_H

#include <SDL3/SDL.h>

/*
 * The "raw" audio time advances in steps (at the rate SDL consumes the
 * stream). So we keep our own clock, advanced every frame with a monotonic
 * timer, and gently pull it back toward the audio.
 */
typedef struct {
    double time;            /* smoothed song time, in seconds */
    double last_raw;
    Uint64 last_change_ns;
    Uint64 last_tick_ns;
    bool started;
} SongClock;

void clock_update(SongClock *c, double raw, Uint64 now_ns);
/*
 * Clock time at any timestamp near the last update, e.g. a key event's:
 * the clock runs at real speed between two updates.
 */
double clock_time_at(const SongClock *c, Uint64 ns);

#endif