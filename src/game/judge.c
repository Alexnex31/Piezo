#include <SDL3/SDL.h>
#include "game/judge.h"

Judgement judge_delta(double delta)
{
    double d = SDL_fabs(delta);

    if (d <= WINDOW_PERFECT)
        return JUDGE_PERFECT;
    if (d <= WINDOW_GREAT)
        return JUDGE_GREAT;
    if (d <= WINDOW_GOOD)
        return JUDGE_GOOD;
    return JUDGE_MISS;
}

double judge_weight(Judgement j)
{
    switch (j) {
    case JUDGE_PERFECT:
        return 1.0;
    case JUDGE_GREAT:
        return 0.7;
    case JUDGE_GOOD:
        return 0.4;
    default:
        return 0.0;
    }
}

const char *judge_name(Judgement j)
{
    switch (j) {
    case JUDGE_PERFECT:
        return "PERFECT";
    case JUDGE_GREAT:
        return "GREAT";
    case JUDGE_GOOD:
        return "GOOD";
    case JUDGE_MISS:
        return "MISS";
    default:
        return "";
    }
}
