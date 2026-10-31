#ifndef TEXT_H
#define TEXT_H

#include <SDL3/SDL.h>

/*
 * SDL's debug font is 8x8 pixels: `scale` enlarges it. Uses the current
 * draw color, alpha included.
 */
void text_centered(SDL_Renderer *r, float center_x, float y, float scale,
                   const char *s);

#endif
