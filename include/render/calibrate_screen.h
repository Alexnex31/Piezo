#ifndef CALIBRATE_SCREEN_H
#define CALIBRATE_SCREEN_H

#include <SDL3/SDL.h>
#include "game/calibration.h"

/*
 * Instructions, tap count and the offset found. Nothing moves in time
 * with the clicks on purpose: the player must tap on the sound alone.
 */
void calibrate_screen_draw(SDL_Renderer *r, const Calibration *c, double t);

#endif
