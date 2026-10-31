#include "core/app.h"
#include "core/config.h"
#include "core/options.h"
#include "audio/clicks.h"
#include "render/screen.h"

#define DEFAULT_SCROLL_SPEED 600.0
#define CLICKS_LENGTH        40.0   /* seconds: count-in, taps and slack */

static bool create_window(App *app)
{
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
    /* Everything is drawn with alpha blending: flash, fades, dimmed notes. */
    SDL_SetRenderDrawBlendMode(app->renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderLogicalPresentation(app->renderer, WIN_W, WIN_H,
                                     SDL_LOGICAL_PRESENTATION_LETTERBOX);
    return true;
}

static bool load_chart(App *app, const char *path)
{
    if (!chart_load(&app->chart, path) || !game_init(&app->game, &app->chart))
        return false;
    app->metronome.bpm = app->chart.bpm;
    app->metronome.offset = app->chart.offset;
    return music_load(&app->music, app->chart.song_path);
}

static bool load_calibration(App *app)
{
    calibration_reset(&app->calibration);
    app->metronome = app->calibration.metronome;
    return clicks_generate(&app->music, CALIBRATION_BPM, CLICKS_LENGTH);
}

bool app_init(App *app, int argc, char *argv[])
{
    Options opt;
    Config cfg;

    if (!options_parse(&opt, argc, argv))
        return false;
    config_load(&cfg);
    app->global_offset = cfg.global_offset;
    app->saved_offset = cfg.global_offset;
    app->scroll_speed = DEFAULT_SCROLL_SPEED;
    app->mode = opt.calibrate ? MODE_CALIBRATE : MODE_PLAY;
    if (!(opt.calibrate ? load_calibration(app) : load_chart(app, opt.chart_path)))
        return false;
    return create_window(app) && music_play(&app->music);
}

static void save_offset(App *app)
{
    Config cfg = { app->global_offset };

    if (config_save(&cfg))
        app->saved_offset = app->global_offset;
}

/* Taps are done (or the clicks ran out): keep the result, stop the clicks. */
static void end_calibration(App *app)
{
    calibration_finish(&app->calibration);
    music_pause(&app->music);
    if (!app->calibration.success)
        return;
    app->global_offset = app->calibration.offset;
    save_offset(app);
}

void app_update(App *app)
{
    app->raw_time = music_raw_time(&app->music);
    app->finished = music_finished(&app->music);
    if (!app->finished)
        clock_update(&app->clock, app->raw_time, SDL_GetTicksNS());
    if (app->mode == MODE_CALIBRATE) {
        if (!app->calibration.done && (app->finished
            || app->calibration.count == CALIBRATION_TAPS))
            end_calibration(app);
    } else if (app->finished) {
        game_finish(&app->game);
    } else {
        game_update(&app->game, app_song_time(app));
    }
}

void app_restart(App *app)
{
    if (app->mode == MODE_CALIBRATE)
        calibration_reset(&app->calibration);
    else
        game_reset(&app->game);
    music_restart(&app->music);
    SDL_zero(app->clock);
    app->raw_time = 0.0;
    app->finished = false;
}

double app_song_time(const App *app)
{
    return app->clock.time - app->global_offset;
}

double app_song_time_at(const App *app, Uint64 ns)
{
    return clock_time_at(&app->clock, ns) - app->global_offset;
}

void app_quit(App *app)
{
    /* An offset changed with +/- during a game is kept too. */
    if (app->mode == MODE_PLAY && app->global_offset != app->saved_offset)
        save_offset(app);
    music_free(&app->music);
    game_free(&app->game);
    chart_free(&app->chart);
    SDL_DestroyRenderer(app->renderer);
    SDL_DestroyWindow(app->window);
}
