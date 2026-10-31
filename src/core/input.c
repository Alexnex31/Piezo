#include "core/input.h"

#define NUDGE_STEP 0.005    /* seconds per +/- press */
#define SPEED_STEP 100.0    /* pixels per second per Up/Down press */
#define SPEED_MIN  200.0
#define SPEED_MAX  3000.0

SDL_AppResult input_handle(App *app, const SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT)
        return SDL_APP_SUCCESS;
    if (event->type != SDL_EVENT_KEY_DOWN)
        return SDL_APP_CONTINUE;
    switch (event->key.key) {
    case SDLK_ESCAPE:
        return SDL_APP_SUCCESS;
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
    default:
        break;
    }
    return SDL_APP_CONTINUE;
}
