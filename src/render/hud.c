#include "render/hud.h"
#include "render/screen.h"

#define TEXT_SCALE 2.0f

/* Under the text only: the playfield takes the right side of the screen. */
static void draw_progress(SDL_Renderer *r, double t, double duration)
{
    SDL_FRect back = { 20, WIN_H - 50, 420, 10 };
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
        SDL_RenderDebugText(r, 10, y, "beat      : -");
        return;
    }
    SDL_RenderDebugTextFormat(r, 10, y, "beat      : %lld.%lld",
                              index / BEATS_PER_BAR + 1,
                              index % BEATS_PER_BAR + 1);
}

static void draw_keys(SDL_Renderer *r)
{
    SDL_SetRenderDrawColor(r, 130, 130, 150, 255);
    SDL_RenderDebugText(r, 10, 180, "+/-      global offset");
    SDL_RenderDebugText(r, 10, 195, "up/down  scroll speed");
    SDL_RenderDebugText(r, 10, 210, "esc      quit");
}

void hud_draw(SDL_Renderer *r, const App *app)
{
    double t = app->clock.time;

    SDL_SetRenderScale(r, TEXT_SCALE, TEXT_SCALE);
    SDL_SetRenderDrawColor(r, 230, 230, 240, 255);
    SDL_RenderDebugText(r, 10, 15, app->finished ? "PIEZO - end of song"
                                                 : "PIEZO");
    SDL_RenderDebugTextFormat(r, 10, 40, "raw audio : %8.3f s", app->raw_time);
    SDL_RenderDebugTextFormat(r, 10, 55, "clock     : %8.3f s", t);
    SDL_RenderDebugTextFormat(r, 10, 70, "drift     : %+6.1f ms",
                              (app->raw_time - t) * 1000.0);
    SDL_RenderDebugTextFormat(r, 10, 85, "duration  : %8.3f s",
                              music_duration(&app->music));
    SDL_RenderDebugTextFormat(r, 10, 105, "bpm       : %8.2f",
                              app->metronome.bpm);
    draw_beat(r, 120, &app->metronome, app_song_time(app));
    SDL_RenderDebugTextFormat(r, 10, 140, "offset    : %+5.0f ms",
                              app->global_offset * 1000.0);
    SDL_RenderDebugTextFormat(r, 10, 155, "speed     : %5.0f px/s",
                              app->scroll_speed);
    draw_keys(r);
    SDL_SetRenderScale(r, 1.0f, 1.0f);

    draw_progress(r, t, music_duration(&app->music));
}
