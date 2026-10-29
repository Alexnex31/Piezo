#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "audio/music.h"
#include "audio/clock.h"

#define WIN_W 960
#define WIN_H 540
#define TEXT_SCALE 2.0f

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    Music music;
    SongClock clock;
} App;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    App *app = SDL_calloc(1, sizeof(App));
    const char *path = argc > 1 ? argv[1] : "assets/song.ogg";

    if (!app)
        return SDL_APP_FAILURE;
    *appstate = app;
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        SDL_Log("SDL_Init: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    if (!SDL_CreateWindowAndRenderer("Piezo", WIN_W, WIN_H, SDL_WINDOW_RESIZABLE,
                                     &app->window, &app->renderer)) {
        SDL_Log("Window: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_SetRenderVSync(app->renderer, 1);
    SDL_SetRenderLogicalPresentation(app->renderer, WIN_W, WIN_H,
                                     SDL_LOGICAL_PRESENTATION_LETTERBOX);
    if (!music_load(&app->music, path) || !music_play(&app->music))
        return SDL_APP_FAILURE;
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    (void)appstate;
    if (event->type == SDL_EVENT_QUIT)
        return SDL_APP_SUCCESS;
    if (event->type == SDL_EVENT_KEY_DOWN && event->key.key == SDLK_ESCAPE)
        return SDL_APP_SUCCESS;
    return SDL_APP_CONTINUE;
}

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

SDL_AppResult SDL_AppIterate(void *appstate)
{
    App *app = appstate;
    SDL_Renderer *r = app->renderer;
    double raw = music_raw_time(&app->music);
    double duration = music_duration(&app->music);

    if (!music_finished(&app->music))
        clock_update(&app->clock, raw, SDL_GetTicksNS());

    SDL_SetRenderDrawColor(r, 18, 18, 26, 255);
    SDL_RenderClear(r);

    SDL_SetRenderScale(r, TEXT_SCALE, TEXT_SCALE);
    SDL_SetRenderDrawColor(r, 230, 230, 240, 255);
    SDL_RenderDebugText(r, 20, 20, "PIEZO");
    SDL_RenderDebugTextFormat(r, 20, 50, "raw audio : %8.3f s", raw);
    SDL_RenderDebugTextFormat(r, 20, 65, "clock     : %8.3f s", app->clock.time);
    SDL_RenderDebugTextFormat(r, 20, 80, "drift     : %+6.1f ms",
                              (raw - app->clock.time) * 1000.0);
    SDL_RenderDebugTextFormat(r, 20, 105, "duration  : %8.3f s", duration);
    if (music_finished(&app->music))
        SDL_RenderDebugText(r, 20, 130, "end of song - press Esc to quit");
    SDL_SetRenderScale(r, 1.0f, 1.0f);

    draw_progress(r, app->clock.time, duration);
    SDL_RenderPresent(r);
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    App *app = appstate;

    (void)result;
    if (!app)
        return;
    music_free(&app->music);
    SDL_DestroyRenderer(app->renderer);
    SDL_DestroyWindow(app->window);
    SDL_free(app);
}