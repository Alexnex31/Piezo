#include "render/feedback.h"
#include "render/playfield.h"
#include "render/text.h"

#define SHOW_TIME 0.6       /* seconds a judgement stays on screen */

void feedback_set_color(SDL_Renderer *r, Judgement j, float alpha)
{
    Uint8 a = (Uint8)(alpha * 255);

    switch (j) {
    case JUDGE_PERFECT:
        SDL_SetRenderDrawColor(r, 120, 230, 255, a);
        break;
    case JUDGE_GREAT:
        SDL_SetRenderDrawColor(r, 120, 230, 120, a);
        break;
    case JUDGE_GOOD:
        SDL_SetRenderDrawColor(r, 240, 200, 90, a);
        break;
    default:
        SDL_SetRenderDrawColor(r, 240, 80, 80, a);
        break;
    }
}

static void draw_judgement(SDL_Renderer *r, const Game *g, double t)
{
    float center = FIELD_X + FIELD_W / 2.0f;
    double age = t - g->last_time;
    float alpha;
    char timing[32];

    if (g->last == JUDGE_NONE || age < 0.0 || age > SHOW_TIME)
        return;
    alpha = (float)(1.0 - age / SHOW_TIME);
    feedback_set_color(r, g->last, alpha);
    text_centered(r, center, 300, 3.0f, judge_name(g->last));
    if (g->last == JUDGE_MISS)
        return;
    SDL_snprintf(timing, sizeof(timing), "%+.0f ms %s", g->last_delta * 1000.0,
                 g->last_delta < 0.0 ? "early" : "late");
    SDL_SetRenderDrawColor(r, 200, 200, 215, (Uint8)(alpha * 255));
    text_centered(r, center, 335, 2.0f, timing);
}

void feedback_draw(SDL_Renderer *r, const Game *g, double t)
{
    char combo[16];

    if (g->combo >= 2) {
        SDL_snprintf(combo, sizeof(combo), "%d", g->combo);
        SDL_SetRenderDrawColor(r, 230, 230, 240, 200);
        text_centered(r, FIELD_X + FIELD_W / 2.0f, 200, 5.0f, combo);
    }
    draw_judgement(r, g, t);
}
