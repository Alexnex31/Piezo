#include "render/calibrate_screen.h"
#include "render/screen.h"
#include "render/text.h"

#define CENTER (WIN_W / 2.0f)

static void set_gray(SDL_Renderer *r)
{
    SDL_SetRenderDrawColor(r, 130, 130, 150, 255);
}

static void set_white(SDL_Renderer *r)
{
    SDL_SetRenderDrawColor(r, 230, 230, 240, 255);
}

static void draw_tapping(SDL_Renderer *r, const Calibration *c, double t)
{
    char line[48];

    set_gray(r);
    text_centered(r, CENTER, 110, 2.0f, "tap any key on each click");
    text_centered(r, CENTER, 135, 2.0f, "listen to the sound, not the screen");
    set_white(r);
    if (metronome_beat(&c->metronome, t) < CALIBRATION_COUNT_IN - 0.5) {
        text_centered(r, CENTER, 230, 4.0f, "listen...");
        return;
    }
    SDL_snprintf(line, sizeof(line), "%d / %d", c->count, CALIBRATION_TAPS);
    text_centered(r, CENTER, 220, 5.0f, line);
    if (c->count == 0)
        return;
    set_gray(r);
    SDL_snprintf(line, sizeof(line), "last tap  %+.0f ms",
                 c->deltas[c->count - 1] * 1000.0);
    text_centered(r, CENTER, 310, 2.0f, line);
    SDL_snprintf(line, sizeof(line), "offset so far  %+.0f ms",
                 calibration_median(c) * 1000.0);
    text_centered(r, CENTER, 335, 2.0f, line);
}

static void draw_done(SDL_Renderer *r, const Calibration *c)
{
    char line[48];

    if (!c->success) {
        SDL_SetRenderDrawColor(r, 240, 80, 80, 255);
        SDL_snprintf(line, sizeof(line), "not enough taps (%d, need %d)",
                     c->count, CALIBRATION_MIN_TAPS);
        text_centered(r, CENTER, 230, 2.0f, line);
        return;
    }
    set_gray(r);
    text_centered(r, CENTER, 130, 2.0f, "your global offset");
    set_white(r);
    SDL_snprintf(line, sizeof(line), "%+.0f ms", c->offset * 1000.0);
    text_centered(r, CENTER, 170, 6.0f, line);
    set_gray(r);
    text_centered(r, CENTER, 260, 2.0f, c->offset >= 0.0
                  ? "the sound reaches you that late"
                  : "you tap ahead of the sound");
    text_centered(r, CENTER, 300, 2.0f, "saved: every chart now uses it");
}

void calibrate_screen_draw(SDL_Renderer *r, const Calibration *c, double t)
{
    set_white(r);
    text_centered(r, CENTER, 40, 3.0f, "CALIBRATION");
    if (c->done)
        draw_done(r, c);
    else
        draw_tapping(r, c, t);
    set_gray(r);
    text_centered(r, CENTER, 480, 2.0f, c->done ? "enter  redo     esc  quit"
                                                : "esc  quit");
}
