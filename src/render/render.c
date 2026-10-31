#include "render/render.h"
#include "render/flash.h"
#include "render/hud.h"
#include "render/playfield.h"

void render_frame(const App *app)
{
    SDL_Renderer *r = app->renderer;
    double t = app_song_time(app);

    SDL_SetRenderDrawColor(r, 18, 18, 26, 255);
    SDL_RenderClear(r);
    /* The clock freezes at the end: a flash caught mid-fade would stay on. */
    if (!app->finished)
        flash_draw(r, &app->metronome, t);
    playfield_draw(r, &app->chart, t, app->scroll_speed);
    hud_draw(r, app);
    SDL_RenderPresent(r);
}
