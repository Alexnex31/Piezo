#include "audio/clicks.h"
#include "game/metronome.h"

#define RATE        44100
#define CHANNELS    2
#define CLICK_TIME  0.030   /* seconds of sound per click */
#define DECAY       0.008   /* seconds for the click to fade by e (~63%) */
#define AMPLITUDE   (0.6 * 32767)

/* A short sine burst that fades out fast: sharp attack, easy to tap on. */
static void add_click(Music *m, int start, float freq)
{
    int length = (int)(CLICK_TIME * RATE);
    Sint16 v;
    int i;

    for (i = 0; i < length && start + i < m->frames; i++) {
        v = (Sint16)(AMPLITUDE * SDL_expf(-i / (DECAY * RATE))
                     * SDL_sinf(2.0f * SDL_PI_F * freq * i / RATE));
        m->samples[(start + i) * CHANNELS] = v;
        m->samples[(start + i) * CHANNELS + 1] = v;
    }
}

bool clicks_generate(Music *m, double bpm, double seconds)
{
    double beat_frames = 60.0 / bpm * RATE;
    int start;
    int beat;

    SDL_zerop(m);
    m->rate = RATE;
    m->channels = CHANNELS;
    m->frames = (int)(seconds * RATE);
    m->samples = SDL_calloc((size_t)m->frames * CHANNELS, sizeof(Sint16));
    if (!m->samples)
        return false;
    for (beat = 0; ; beat++) {
        start = (int)SDL_round(beat * beat_frames);
        if (start >= m->frames)
            break;
        add_click(m, start, beat % BEATS_PER_BAR == 0 ? 1500.0f : 1000.0f);
    }
    return true;
}
