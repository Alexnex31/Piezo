#ifndef HUD_H
#define HUD_H

#include <SDL3/SDL.h>
#include "core/app.h"

/* Debug text (clock, drift, tempo, beat counter) and the progress bar. */
void hud_draw(SDL_Renderer *r, const App *app);

#endif
