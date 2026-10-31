#ifndef FEEDBACK_H
#define FEEDBACK_H

#include <SDL3/SDL.h>
#include "game/game.h"

/*
 * Over the playfield while playing: the combo, and the latest judgement
 * with its timing (early / late), fading out.
 */
void feedback_draw(SDL_Renderer *r, const Game *g, double t);

/* One color per judgement, shared with the results screen. */
void feedback_set_color(SDL_Renderer *r, Judgement j, float alpha);

#endif
