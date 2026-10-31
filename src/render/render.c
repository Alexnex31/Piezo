#include "render/render.h"
#include "render/calibrate_screen.h"
#include "render/feedback.h"
#include "render/flash.h"
#include "render/hud.h"
#include "render/playfield.h"
#include "render/results.h"

void render_frame(const App *app)
{
    SDL_Renderer *r = app->renderer;
    double t = app_song_time(app);

    SDL_SetRenderDrawColor(r, 18, 18, 26, 255);
    SDL_RenderClear(r);
    if (app->mode == MODE_CALIBRATE) {
        /* The bare clock: calibration measures the offset, can't use it. */
        calibrate_screen_draw(r, &app->calibration, app->clock.time);
    } else if (app->finished) {
        results_draw(r, &app->game);
    } else {
        flash_draw(r, &app->metronome, t);
        playfield_draw(r, &app->game, t, app->scroll_speed);
        feedback_draw(r, &app->game, t);
        hud_draw(r, app);
    }
    SDL_RenderPresent(r);
}
