#ifndef INPUT_H
#define INPUT_H

#include <SDL3/SDL.h>
#include "core/app.h"

/*
 * D F J K play the 4 columns (by key position, any keyboard layout).
 * Esc / window close quit, +/- nudge the global offset, Up/Down change the
 * scroll speed, Enter replays from the results screen. When calibrating,
 * any other key is a tap.
 */
SDL_AppResult input_handle(App *app, const SDL_Event *event);

#endif
