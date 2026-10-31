#ifndef PLAYFIELD_H
#define PLAYFIELD_H

#include <SDL3/SDL.h>
#include "game/chart.h"

/*
 * The 4 columns and the notes scrolling down to the judgement line.
 * The judgement line is a row of target circles; a note fills its target
 * exactly at its time.
 */
void playfield_draw(SDL_Renderer *r, const Chart *c, double t, double speed);

#endif
