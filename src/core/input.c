#include "core/input.h"

#define NUDGE_STEP 0.005    /* seconds per +/- press */
#define SPEED_STEP 100.0    /* pixels per second per Up/Down press */
#define SPEED_MIN  200.0
#define SPEED_MAX  3000.0

/* Scancodes: the physical key, so the same spots on AZERTY and QWERTY. */
static const SDL_Scancode column_keys[CHART_COLUMNS] = {
    SDL_SCANCODE_D, SDL_SCANCODE_F, SDL_SCANCODE_J, SDL_SCANCODE_K
};

static int key_column(SDL_Scancode key)
{
    int i;

    for (i = 0; i < CHART_COLUMNS; i++)
        if (column_keys[i] == key)
            return i;
    return -1;
}

/* Judged on the event's own timestamp, not on when we get to process it. */
static void play_key(App *app, const SDL_KeyboardEvent *key)
{
    int column = key_column(key->scancode);

    if (column < 0)
        return;
    app->game.held[column] = key->down;
    if (key->down && !key->repeat && !app->finished && app->clock.started)
        game_press(&app->game, column, app_song_time_at(app, key->timestamp));
}

/*
 * Calibration: Enter redoes it once done, any other key is a tap, timed on
 * the bare clock (no offset).
 */
static void calibration_key(App *app, const SDL_KeyboardEvent *key)
{
    if (key->key == SDLK_RETURN || key->key == SDLK_KP_ENTER) {
        if (app->calibration.done)
            app_restart(app);
        return;
    }
    if (key->repeat || app->finished || !app->clock.started)
        return;
    calibration_tap(&app->calibration,
                    clock_time_at(&app->clock, key->timestamp));
}

static void settings_key(App *app, SDL_Keycode key)
{
    switch (key) {
    /* '=' too: on QWERTY, '+' needs Shift on that same key. */
    case SDLK_PLUS:
    case SDLK_EQUALS:
    case SDLK_KP_PLUS:
        app->global_offset += NUDGE_STEP;
        break;
    case SDLK_MINUS:
    case SDLK_KP_MINUS:
        app->global_offset -= NUDGE_STEP;
        break;
    case SDLK_UP:
        app->scroll_speed = SDL_min(app->scroll_speed + SPEED_STEP, SPEED_MAX);
        break;
    case SDLK_DOWN:
        app->scroll_speed = SDL_max(app->scroll_speed - SPEED_STEP, SPEED_MIN);
        break;
    case SDLK_RETURN:
    case SDLK_KP_ENTER:
        if (app->finished)
            app_restart(app);
        break;
    default:
        break;
    }
}

SDL_AppResult input_handle(App *app, const SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT)
        return SDL_APP_SUCCESS;
    if (event->type != SDL_EVENT_KEY_DOWN && event->type != SDL_EVENT_KEY_UP)
        return SDL_APP_CONTINUE;
    if (app->mode == MODE_PLAY)
        play_key(app, &event->key);
    if (event->type == SDL_EVENT_KEY_UP)
        return SDL_APP_CONTINUE;
    if (event->key.key == SDLK_ESCAPE)
        return SDL_APP_SUCCESS;
    if (app->mode == MODE_CALIBRATE)
        calibration_key(app, &event->key);
    else
        settings_key(app, event->key.key);
    return SDL_APP_CONTINUE;
}
