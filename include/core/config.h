#ifndef CONFIG_H
#define CONFIG_H

#include <SDL3/SDL.h>

/*
 * Settings kept between runs, in a small text file in SDL's per-user data
 * folder (~/.local/share/piezo/config.txt on Linux):
 *
 *     global_offset 23    # ms
 */
typedef struct {
    double global_offset;   /* seconds */
} Config;

/* A missing or unreadable file just gives the defaults. */
void config_load(Config *cfg);
bool config_save(const Config *cfg);

#endif
