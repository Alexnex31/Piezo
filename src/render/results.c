#include "render/results.h"
#include "render/feedback.h"
#include "render/screen.h"
#include "render/text.h"

#define CENTER (WIN_W / 2.0f)

static void draw_counts(SDL_Renderer *r, const Game *g)
{
    char line[32];
    Judgement j;

    for (j = JUDGE_PERFECT; j <= JUDGE_MISS; j++) {
        SDL_snprintf(line, sizeof(line), "%-8s %5d", judge_name(j),
                     g->counts[j]);
        feedback_set_color(r, j, 1.0f);
        text_centered(r, CENTER, 230 + (j - JUDGE_PERFECT) * 30, 2.0f, line);
    }
}

/* Average timing of the hits: tells if you tend to play early or late. */
static void draw_average(SDL_Renderer *r, const Game *g)
{
    int hits = game_hits(g);
    double average = hits ? g->delta_sum / hits : 0.0;
    char line[48];

    if (hits == 0)
        SDL_snprintf(line, sizeof(line), "average  -");
    else
        SDL_snprintf(line, sizeof(line), "average  %+.0f ms (%s)",
                     average * 1000.0, average < 0.0 ? "early" : "late");
    text_centered(r, CENTER, 400, 2.0f, line);
}

void results_draw(SDL_Renderer *r, const Game *g)
{
    char line[48];

    SDL_SetRenderDrawColor(r, 230, 230, 240, 255);
    text_centered(r, CENTER, 40, 3.0f, "RESULTS");
    SDL_snprintf(line, sizeof(line), "%07d", game_score(g));
    text_centered(r, CENTER, 90, 6.0f, line);
    SDL_snprintf(line, sizeof(line), "%.2f %%", game_accuracy(g) * 100.0);
    text_centered(r, CENTER, 160, 3.0f, line);
    draw_counts(r, g);
    SDL_SetRenderDrawColor(r, 230, 230, 240, 255);
    SDL_snprintf(line, sizeof(line), "max combo  %d", g->max_combo);
    text_centered(r, CENTER, 370, 2.0f, line);
    draw_average(r, g);
    SDL_SetRenderDrawColor(r, 130, 130, 150, 255);
    text_centered(r, CENTER, 480, 2.0f, "enter  play again     esc  quit");
}
