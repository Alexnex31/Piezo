#include "core/options.h"

#define DEFAULT_SONG      "assets/song.ogg"
#define DEFAULT_BPM       120.0
#define DEFAULT_OFFSET_MS 0.0

/* Whole string must be a number: "120" is fine, "120abc" is not. */
static bool parse_number(const char *s, double *out)
{
    char *end;

    *out = SDL_strtod(s, &end);
    return end != s && *end == '\0';
}

bool options_parse(Options *opt, int argc, char *argv[])
{
    double offset_ms = DEFAULT_OFFSET_MS;
    bool ok = argc <= 4;

    opt->song_path = argc > 1 ? argv[1] : DEFAULT_SONG;
    opt->bpm = DEFAULT_BPM;
    if (ok && argc > 2)
        ok = parse_number(argv[2], &opt->bpm) && opt->bpm > 0.0;
    if (ok && argc > 3)
        ok = parse_number(argv[3], &offset_ms);
    if (!ok) {
        SDL_Log("Usage: %s [song.ogg] [bpm] [offset_ms]", argv[0]);
        return false;
    }
    opt->offset = offset_ms / 1000.0;
    return true;
}
