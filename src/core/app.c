#include "core/app.h"
#include "core/options.h"
#include "render/screen.h"

bool app_init(App *app, int argc, char *argv[])
{
    Options opt;

    if (!options_parse(&opt, argc, argv))
        return false;
    app->metronome.bpm = opt.bpm;
    app->metronome.offset = opt.offset;
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        SDL_Log("SDL_Init: %s", SDL_GetError());
        return false;
    }
    if (!SDL_CreateWindowAndRenderer("Piezo", WIN_W, WIN_H, SDL_WINDOW_RESIZABLE,
                                     &app->window, &app->renderer)) {
        SDL_Log("Window: %s", SDL_GetError());
        return false;
    }
    SDL_SetRenderVSync(app->renderer, 1);
    SDL_SetRenderLogicalPresentation(app->renderer, WIN_W, WIN_H,
                                     SDL_LOGICAL_PRESENTATION_LETTERBOX);
    return music_load(&app->music, opt.song_path) && music_play(&app->music);
}

void app_update(App *app)
{
    app->raw_time = music_raw_time(&app->music);
    app->finished = music_finished(&app->music);
    if (!app->finished)
        clock_update(&app->clock, app->raw_time, SDL_GetTicksNS());
}

void app_quit(App *app)
{
    music_free(&app->music);
    SDL_DestroyRenderer(app->renderer);
    SDL_DestroyWindow(app->window);
}
