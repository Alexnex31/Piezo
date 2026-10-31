#include "render/playfield.h"
#include "render/screen.h"
#include "render/shapes.h"

#define RADIUS   36.0f      /* notes and targets: a note fills its target */
#define RING_W   4.0f       /* target outline thickness */

static float column_center(int column)
{
    return FIELD_X + column * COLUMN_W + COLUMN_W / 2.0f;
}

static void draw_columns(SDL_Renderer *r, const Game *g)
{
    SDL_FRect field = { FIELD_X, 0, FIELD_W, WIN_H };
    SDL_FRect sep = { 0, 0, 1, WIN_H };
    SDL_FColor ring = { 0.75f, 0.75f, 0.82f, 1.0f };
    SDL_FColor pressed = { 0.75f, 0.75f, 0.82f, 0.30f };
    SDL_FPoint center;
    int i;

    SDL_SetRenderDrawColor(r, 28, 28, 40, 255);
    SDL_RenderFillRect(r, &field);
    SDL_SetRenderDrawColor(r, 60, 60, 80, 255);
    for (i = 0; i <= CHART_COLUMNS; i++) {
        sep.x = FIELD_X + i * COLUMN_W;
        SDL_RenderFillRect(r, &sep);
    }
    /* The judgement line: one target circle per column, lit while held. */
    for (i = 0; i < CHART_COLUMNS; i++) {
        center = (SDL_FPoint){ column_center(i), JUDGE_Y };
        if (g->held[i])
            shape_circle(r, center, RADIUS, pressed);
        shape_ring(r, center, RADIUS, RING_W, ring);
    }
}

/* Outer columns white, inner columns blue, like most 4-key games. */
static void draw_note(SDL_Renderer *r, const Note *n, float y, bool missed)
{
    SDL_FColor white = { 0.90f, 0.90f, 0.94f, 1.0f };
    SDL_FColor blue = { 0.47f, 0.78f, 1.0f, 1.0f };
    bool outer = n->column == 0 || n->column == CHART_COLUMNS - 1;
    SDL_FColor color = outer ? white : blue;

    if (missed)
        color.a = 0.3f;
    shape_circle(r, (SDL_FPoint){ column_center(n->column), y }, RADIUS, color);
}

void playfield_draw(SDL_Renderer *r, const Game *g, double t, double speed)
{
    const Chart *c = g->chart;
    Judgement j;
    float y;
    int i;

    draw_columns(r, g);
    for (i = 0; i < c->count; i++) {
        y = (float)(JUDGE_Y - (c->notes[i].time - t) * speed);
        if (y < -RADIUS)
            break;          /* sorted: every later note is higher still */
        j = g->results[i];
        if (y <= WIN_H + RADIUS && (j == JUDGE_NONE || j == JUDGE_MISS))
            draw_note(r, &c->notes[i], y, j == JUDGE_MISS);
    }
}
