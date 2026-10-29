#include "render/hud.h"
#include "render/screen.h"

#define TEXT_SCALE 2.0f

static void draw_progress(SDL_Renderer *r, double t, double duration)
{
    SDL_FRect back = { 40, WIN_H - 60, WIN_W - 80, 12 };
    SDL_FRect fill = back;

    fill.w = (float)(back.w * SDL_clamp(t / duration, 0.0, 1.0));
    SDL_SetRenderDrawColor(r, 50, 50, 70, 255);
    SDL_RenderFillRect(r, &back);
    SDL_SetRenderDrawColor(r, 120, 200, 255, 255);
    SDL_RenderFillRect(r, &fill);
}

/* "bar.beat", both counted from 1 like musicians do: 1.1, 1.2 ... 2.1 */
static void draw_beat(SDL_Renderer *r, float y, const Metronome *m, double t)
{
    double beat = metronome_beat(m, t);
    long long index = (long long)SDL_floor(beat);

    if (beat < 0.0) {
        SDL_RenderDebugText(r, 20, y, "beat      : -");
        return;
    }
    SDL_RenderDebugTextFormat(r, 20, y, "beat      : %lld.%lld",
                              index / BEATS_PER_BAR + 1,
                              index % BEATS_PER_BAR + 1);
}

void hud_draw(SDL_Renderer *r, const App *app)
{
    double t = app->clock.time;

    SDL_SetRenderScale(r, TEXT_SCALE, TEXT_SCALE);
    SDL_SetRenderDrawColor(r, 230, 230, 240, 255);
    SDL_RenderDebugText(r, 20, 20, "PIEZO");
    SDL_RenderDebugTextFormat(r, 20, 50, "raw audio : %8.3f s", app->raw_time);
    SDL_RenderDebugTextFormat(r, 20, 65, "clock     : %8.3f s", t);
    SDL_RenderDebugTextFormat(r, 20, 80, "drift     : %+6.1f ms",
                              (app->raw_time - t) * 1000.0);
    SDL_RenderDebugTextFormat(r, 20, 95, "duration  : %8.3f s",
                              music_duration(&app->music));
    SDL_RenderDebugTextFormat(r, 20, 120, "bpm       : %8.2f",
                              app->metronome.bpm);
    SDL_RenderDebugTextFormat(r, 20, 135, "offset    : %+6.0f ms  (+/- to adjust)",
                              app->metronome.offset * 1000.0);
    draw_beat(r, 150, &app->metronome, t);
    if (app->finished)
        SDL_RenderDebugText(r, 20, 175, "end of song - press Esc to quit");
    SDL_SetRenderScale(r, 1.0f, 1.0f);

    draw_progress(r, t, music_duration(&app->music));
}
