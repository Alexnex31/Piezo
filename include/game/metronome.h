#ifndef METRONOME_H
#define METRONOME_H

#define BEATS_PER_BAR 4

/*
 * Constant tempo only. Beat 0 lands on `offset`, then one beat every
 * 60 / bpm seconds.
 */
typedef struct {
    double bpm;
    double offset;      /* time of beat 0, in seconds */
} Metronome;

/* Beat position at song time t: 12.37 = beat 12, 37% of the way to 13. */
double metronome_beat(const Metronome *m, double t);
double metronome_beat_length(const Metronome *m);   /* seconds */

#endif
