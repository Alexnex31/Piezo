#include "game/calibration.h"

void calibration_reset(Calibration *c)
{
    SDL_zerop(c);
    c->metronome.bpm = CALIBRATION_BPM;
    c->last_beat = -1;
}

void calibration_tap(Calibration *c, double t)
{
    double beat = metronome_beat(&c->metronome, t);
    long long nearest = (long long)SDL_round(beat);

    if (c->done || c->count >= CALIBRATION_TAPS)
        return;
    if (nearest < CALIBRATION_COUNT_IN || nearest == c->last_beat)
        return;
    c->deltas[c->count++] = (beat - nearest)
                          * metronome_beat_length(&c->metronome);
    c->last_beat = nearest;
}

void calibration_finish(Calibration *c)
{
    c->done = true;
    c->success = c->count >= CALIBRATION_MIN_TAPS;
    c->offset = c->success ? calibration_median(c) : 0.0;
}

static int compare_doubles(const void *a, const void *b)
{
    double da = *(const double *)a;
    double db = *(const double *)b;

    return (da > db) - (da < db);
}

double calibration_median(const Calibration *c)
{
    double sorted[CALIBRATION_TAPS];
    int mid = c->count / 2;

    if (c->count == 0)
        return 0.0;
    SDL_memcpy(sorted, c->deltas, c->count * sizeof(double));
    SDL_qsort(sorted, c->count, sizeof(double), compare_doubles);
    if (c->count % 2)
        return sorted[mid];
    return (sorted[mid - 1] + sorted[mid]) / 2.0;
}
