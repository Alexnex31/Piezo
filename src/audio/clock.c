#include <math.h>
#include "audio/clock.h"

#define SNAP_THRESHOLD  0.100   /* beyond this: snap back in one go */
#define CORRECTION      0.05    /* share of the error corrected each frame */
#define MAX_EXTRAPOLATE 0.100   /* audio stopped moving? don't make time up */

void clock_update(SongClock *c, double raw, Uint64 now_ns)
{
    double dt;
    double since_change;
    double target;
    double err;

    if (!c->started) {
        c->time = raw;
        c->last_raw = raw;
        c->last_change_ns = now_ns;
        c->last_tick_ns = now_ns;
        c->started = true;
        return;
    }
    dt = (now_ns - c->last_tick_ns) / 1e9;
    c->last_tick_ns = now_ns;
    if (raw != c->last_raw) {
        c->last_raw = raw;
        c->last_change_ns = now_ns;
    }
    /* Estimate: last audio step + time elapsed since that step. */
    since_change = (now_ns - c->last_change_ns) / 1e9;
    if (since_change > MAX_EXTRAPOLATE)
        since_change = MAX_EXTRAPOLATE;
    target = c->last_raw + since_change;

    c->time += dt;
    err = target - c->time;
    if (fabs(err) > SNAP_THRESHOLD)
        c->time = target;
    else
        c->time += err * CORRECTION;
}