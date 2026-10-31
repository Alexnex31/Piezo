#ifndef RESULTS_H
#define RESULTS_H

#include <SDL3/SDL.h>
#include "game/game.h"

/* End of song: score, accuracy, counts, max combo and average timing. */
void results_draw(SDL_Renderer *r, const Game *g);

#endif
