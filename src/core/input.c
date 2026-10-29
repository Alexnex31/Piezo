#include "core/input.h"

#define NUDGE_STEP 0.005    /* seconds per +/- press */

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
        app->metronome.offset += NUDGE_STEP;
        break;
    case SDLK_MINUS:
    case SDLK_KP_MINUS:
        app->metronome.offset -= NUDGE_STEP;
        break;
    default:
        break;
    }
    return SDL_APP_CONTINUE;
}
