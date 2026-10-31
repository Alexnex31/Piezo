#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h>
#include "game/chart.h"
#include "game/judge.h"

/*
 * One play of a chart: which notes were judged and how, score and combo.
 * All times are song times in seconds.
 */
typedef struct {
    const Chart *chart;
    Judgement *results;         /* one per chart note */
    int next[CHART_COLUMNS];    /* first unjudged note of each column */
    int counts[JUDGE_COUNT];
    int combo;
    int max_combo;
    double weight_sum;          /* judge_weight of every judged note */
    double delta_sum;           /* timing of every hit, for the average */
    Judgement last;             /* latest judgement, for the feedback text */
    double last_delta;
    double last_time;           /* when it happened */
    bool held[CHART_COLUMNS];   /* keys currently down */
} Game;

bool game_init(Game *g, const Chart *chart);
void game_reset(Game *g);
void game_free(Game *g);

/* A key went down in `column` at time t: hits that column's next note. */
void game_press(Game *g, int column, double t);
/* Notes more than WINDOW_GOOD late become Misses. */
void game_update(Game *g, double t);
/* End of song: every note left becomes a Miss. */
void game_finish(Game *g);

int game_score(const Game *g);          /* 0 to 1 000 000 */
double game_accuracy(const Game *g);    /* 0 to 1, over judged notes */
int game_hits(const Game *g);           /* Perfect + Great + Good */

#endif
