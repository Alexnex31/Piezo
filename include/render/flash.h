#ifndef FLASH_H
#define FLASH_H

#include <SDL3/SDL.h>
#include "game/metronome.h"

/*
 * Full-screen flash on every beat, fading out quickly. The first beat of
 * each bar is brighter and warmer. Computed from the time alone, no state.
 */
void flash_draw(SDL_Renderer *r, const Metronome *m, double t);

#endif
