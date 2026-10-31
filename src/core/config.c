#include "core/config.h"

#define CONFIG_FILE "config.txt"

/* No organization name: the folder is just "piezo". */
static char *config_path(void)
{
    char *dir = SDL_GetPrefPath("", "piezo");
    char *path = NULL;

    if (!dir) {
        SDL_Log("No folder for the config: %s", SDL_GetError());
        return NULL;
    }
    if (SDL_asprintf(&path, "%s%s", dir, CONFIG_FILE) < 0)
        path = NULL;
    SDL_free(dir);
    return path;
}

void config_load(Config *cfg)
{
    char *path = config_path();
    char *text = path ? SDL_LoadFile(path, NULL) : NULL;
    char *line;
    char *next;
    double ms;

    SDL_zerop(cfg);
    SDL_free(path);
    if (!text)
        return;
    for (line = text; line; line = next) {
        next = SDL_strchr(line, '\n');
        if (next)
            *next++ = '\0';
        if (SDL_sscanf(line, " global_offset %lf", &ms) == 1)
            cfg->global_offset = ms / 1000.0;
    }
    SDL_free(text);
}

bool config_save(const Config *cfg)
{
    char *path = config_path();
    SDL_IOStream *io = path ? SDL_IOFromFile(path, "w") : NULL;
    bool ok;

    if (!io) {
        SDL_Log("Could not save the config: %s", SDL_GetError());
        SDL_free(path);
        return false;
    }
    ok = SDL_IOprintf(io, "# Piezo settings\n"
                          "global_offset %.0f    # ms, from calibration or +/-\n",
                      cfg->global_offset * 1000.0) > 0;
    ok = SDL_CloseIO(io) && ok;
    if (ok)
        SDL_Log("Saved the global offset to %s", path);
    SDL_free(path);
    return ok;
}
