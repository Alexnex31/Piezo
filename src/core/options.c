#include "core/options.h"

#define DEFAULT_CHART "assets/song.chart"

bool options_parse(Options *opt, int argc, char *argv[])
{
    if (argc > 2) {
        SDL_Log("Usage: %s [chart]", argv[0]);
        return false;
    }
    opt->chart_path = argc > 1 ? argv[1] : DEFAULT_CHART;
    return true;
}
