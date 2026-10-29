#ifndef INPUT_H
#define INPUT_H

#include <SDL3/SDL.h>
#include "core/app.h"

/* Esc / window close quit, +/- nudge the metronome offset. */
SDL_AppResult input_handle(App *app, const SDL_Event *event);

#endif
