#ifndef CALIBRATION_H
#define CALIBRATION_H

#include <SDL3/SDL.h>
#include "game/metronome.h"

#define CALIBRATION_BPM      120.0
#define CALIBRATION_COUNT_IN 4      /* clicks to listen to before tapping */
#define CALIBRATION_TAPS     32
#define CALIBRATION_MIN_TAPS 8      /* fewer when the clicks run out: fail */

/*
 * The player taps on each click they hear. How late the taps land, on the
 * clock, is the delay of the whole chain (sound card, speakers, keyboard,
 * reflexes): the global offset that makes "on the sound" a Perfect.
 */
typedef struct {
    Metronome metronome;    /* where the clicks are */
    double deltas[CALIBRATION_TAPS];  /* each tap minus its nearest click */
    int count;
    long long last_beat;    /* click of the latest tap, against double taps */
    bool done;
    bool success;
    double offset;          /* seconds, the result once done */
} Calibration;

void calibration_reset(Calibration *c);
/* A tap at clock time t (no offset applied). */
void calibration_tap(Calibration *c, double t);
/* Ends the session: median of the taps, if there are enough. */
void calibration_finish(Calibration *c);
/* Median of the taps so far: one stray tap barely moves it. */
double calibration_median(const Calibration *c);

#endif
