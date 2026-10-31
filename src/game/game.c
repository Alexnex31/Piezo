#include "game/game.h"

#define MAX_SCORE 1000000.0

/* First unjudged note of `column` from index `from`, or the note count. */
static int find_next(const Game *g, int column, int from)
{
    const Chart *c = g->chart;

    while (from < c->count && (c->notes[from].column != column
                               || g->results[from] != JUDGE_NONE))
        from++;
    return from;
}

static void judge_note(Game *g, int i, Judgement j, double delta, double t)
{
    int column = g->chart->notes[i].column;

    g->results[i] = j;
    g->counts[j]++;
    g->weight_sum += judge_weight(j);
    if (j == JUDGE_MISS) {
        g->combo = 0;
    } else {
        g->combo++;
        g->max_combo = SDL_max(g->max_combo, g->combo);
        g->delta_sum += delta;
    }
    g->last = j;
    g->last_delta = delta;
    g->last_time = t;
    g->next[column] = find_next(g, column, i + 1);
}

bool game_init(Game *g, const Chart *chart)
{
    SDL_zerop(g);
    g->chart = chart;
    g->results = SDL_malloc(SDL_max(chart->count, 1) * sizeof(Judgement));
    if (!g->results)
        return false;
    game_reset(g);
    return true;
}

void game_reset(Game *g)
{
    const Chart *chart = g->chart;
    Judgement *results = g->results;
    int i;

    SDL_zerop(g);
    g->chart = chart;
    g->results = results;
    for (i = 0; i < chart->count; i++)
        g->results[i] = JUDGE_NONE;
    for (i = 0; i < CHART_COLUMNS; i++)
        g->next[i] = find_next(g, i, 0);
}

void game_free(Game *g)
{
    SDL_free(g->results);
    SDL_zerop(g);
}

void game_press(Game *g, int column, double t)
{
    int i;
    double delta;

    game_update(g, t);
    i = g->next[column];
    if (i >= g->chart->count)
        return;
    delta = t - g->chart->notes[i].time;
    /* Too early for this note: a stray tap, not a Miss. */
    if (delta >= -WINDOW_GOOD)
        judge_note(g, i, judge_delta(delta), delta, t);
}

void game_update(Game *g, double t)
{
    const Chart *c = g->chart;
    double late;
    int col;
    int i;

    for (col = 0; col < CHART_COLUMNS; col++) {
        for (i = g->next[col]; i < c->count; i = g->next[col]) {
            late = c->notes[i].time + WINDOW_GOOD;
            if (t <= late)
                break;
            judge_note(g, i, JUDGE_MISS, 0.0, late);
        }
    }
}

void game_finish(Game *g)
{
    int col;

    for (col = 0; col < CHART_COLUMNS; col++)
        while (g->next[col] < g->chart->count)
            judge_note(g, g->next[col], JUDGE_MISS, 0.0, g->last_time);
}

int game_score(const Game *g)
{
    if (g->chart->count == 0)
        return 0;
    return (int)(MAX_SCORE * g->weight_sum / g->chart->count + 0.5);
}

double game_accuracy(const Game *g)
{
    int judged = game_hits(g) + g->counts[JUDGE_MISS];

    return judged ? g->weight_sum / judged : 1.0;
}

int game_hits(const Game *g)
{
    return g->counts[JUDGE_PERFECT] + g->counts[JUDGE_GREAT]
         + g->counts[JUDGE_GOOD];
}
