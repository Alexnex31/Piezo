#ifndef MUSIC_H
#define MUSIC_H

#include <SDL3/SDL.h>

/*
 * The song is fully decoded into memory, then fed bit by bit into an
 * SDL_AudioStream. Since we know how many frames we pushed and how many are
 * still waiting in the stream, we know exactly where playback is, down to
 * the sample.
 */
typedef struct {
    SDL_AudioStream *stream;
    Sint16 *samples;    /* interleaved 16-bit PCM */
    int frames;         /* total frame count (1 frame = 1 sample per channel) */
    int channels;
    int rate;
    int pushed;         /* frames already sent to the stream */
} Music;

bool music_load(Music *m, const char *path);
bool music_play(Music *m);
void music_restart(Music *m);       /* back to the start, keeps playing */
double music_raw_time(Music *m);    /* seconds, advances in steps */
double music_duration(const Music *m);
bool music_finished(Music *m);
void music_free(Music *m);

#endif