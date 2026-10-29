#include "game/metronome.h"

double metronome_beat(const Metronome *m, double t)
{
    return (t - m->offset) / metronome_beat_length(m);
}

double metronome_beat_length(const Metronome *m)
{
    return 60.0 / m->bpm;
}
