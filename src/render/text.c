#include "render/text.h"

void text_centered(SDL_Renderer *r, float center_x, float y, float scale,
                   const char *s)
{
    float width = SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * scale * SDL_strlen(s);

    /* Under a render scale, positions are in scaled units too. */
    SDL_SetRenderScale(r, scale, scale);
    SDL_RenderDebugText(r, (center_x - width / 2.0f) / scale, y / scale, s);
    SDL_SetRenderScale(r, 1.0f, 1.0f);
}
