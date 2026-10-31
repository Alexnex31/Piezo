#ifndef CLICKS_H
#define CLICKS_H

#include "audio/music.h"

/*
 * Fills `m` with a click track made in memory instead of loaded from a
 * file: a click starts exactly on every beat, the first of each bar
 * higher. It then plays like any song, through the same stream and clock.
 */
bool clicks_generate(Music *m, double bpm, double seconds);

#endif
