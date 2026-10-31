#ifndef PLAYFIELD_H
#define PLAYFIELD_H

#include <SDL3/SDL.h>
#include "game/game.h"

#define COLUMN_W 90
#define FIELD_X  480        /* left edge; the HUD text lives on the left */
#define FIELD_W  (COLUMN_W * CHART_COLUMNS)
#define JUDGE_Y  460        /* center of the target circles, from the top */

/*
 * The 4 columns and the notes scrolling down to the judgement line.
 * The judgement line is a row of target circles; a note fills its target
 * exactly at its time. Hit notes vanish, missed ones keep falling, dimmed.
 */
void playfield_draw(SDL_Renderer *r, const Game *g, double t, double speed);

#endif
