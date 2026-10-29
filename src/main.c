#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "core/app.h"
#include "core/input.h"
#include "render/render.h"

/*
 * With SDL's callbacks, these four functions are the program's main:
 * SDL calls them, and each one hands the work to its module.
 */

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    App *app = SDL_calloc(1, sizeof(App));

    if (!app)
        return SDL_APP_FAILURE;
    *appstate = app;
    return app_init(app, argc, argv) ? SDL_APP_CONTINUE : SDL_APP_FAILURE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    return input_handle(appstate, event);
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    app_update(appstate);
    render_frame(appstate);
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    (void)result;
    if (!appstate)
        return;
    app_quit(appstate);
    SDL_free(appstate);
}
