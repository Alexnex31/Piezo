#include "render/flash.h"
#include "render/screen.h"

#define FLASH_LENGTH 0.100  /* seconds of fade after each beat */

void flash_draw(SDL_Renderer *r, const Metronome *m, double t)
{
    SDL_FRect screen = { 0, 0, WIN_W, WIN_H };
    double beat = metronome_beat(m, t);
    double index = SDL_floor(beat);
    double since = (beat - index) * metronome_beat_length(m);
    float strength;

    if (beat < 0.0 || since >= FLASH_LENGTH)
        return;
    strength = (float)(1.0 - since / FLASH_LENGTH);
    if ((long long)index % BEATS_PER_BAR == 0)
        SDL_SetRenderDrawColor(r, 255, 170, 60, (Uint8)(strength * 200));
    else
        SDL_SetRenderDrawColor(r, 120, 200, 255, (Uint8)(strength * 110));
    SDL_RenderFillRect(r, &screen);
}
