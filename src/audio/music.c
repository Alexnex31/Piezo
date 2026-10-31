#include <vorbis/vorbisfile.h>
#include "audio/music.h"

#define READ_CHUNK 4096

static int frame_bytes(const Music *m)
{
    return m->channels * (int)sizeof(Sint16);
}

/* Called by SDL from the audio thread when the stream needs more data. */
static void SDLCALL feed(void *userdata, SDL_AudioStream *stream,
                         int additional, int total)
{
    Music *m = userdata;
    int fb = frame_bytes(m);
    int want = (additional + fb - 1) / fb;
    int left = m->frames - m->pushed;

    (void)total;
    if (want > left)
        want = left;
    if (want <= 0)
        return;
    SDL_PutAudioStreamData(stream, m->samples + (size_t)m->pushed * m->channels,
                           want * fb);
    m->pushed += want;
}

bool music_load(Music *m, const char *path)
{
    OggVorbis_File vf;
    vorbis_info *info;
    char *dst;
    size_t size;
    size_t done = 0;
    long got;
    int section;

    SDL_zerop(m);
    if (ov_fopen(path, &vf) != 0) {
        SDL_Log("Could not open '%s' (expected OGG Vorbis)", path);
        return false;
    }
    info = ov_info(&vf, -1);
    m->channels = info->channels;
    m->rate = (int)info->rate;
    m->frames = (int)ov_pcm_total(&vf, -1);
    size = (size_t)m->frames * frame_bytes(m);
    m->samples = m->frames > 0 ? SDL_malloc(size) : NULL;
    if (!m->samples) {
        SDL_Log("Could not decode '%s'", path);
        ov_clear(&vf);
        return false;
    }
    /* ov_read hands out at most a few KB per call: loop until the end. */
    dst = (char *)m->samples;
    while (done < size) {
        got = ov_read(&vf, dst + done, (int)SDL_min(size - done, READ_CHUNK),
                      SDL_BYTEORDER == SDL_BIG_ENDIAN, 2, 1, &section);
        if (got == 0)
            break;
        if (got == OV_HOLE)     /* corrupt page: skip it and carry on */
            continue;
        if (got < 0) {
            SDL_Log("Decoding error in '%s'", path);
            ov_clear(&vf);
            music_free(m);
            return false;
        }
        done += (size_t)got;
    }
    ov_clear(&vf);
    m->frames = (int)(done / frame_bytes(m));
    return true;
}

bool music_play(Music *m)
{
    SDL_AudioSpec spec = { SDL_AUDIO_S16, m->channels, m->rate };

    m->stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK,
                                          &spec, feed, m);
    if (!m->stream) {
        SDL_Log("Could not open audio: %s", SDL_GetError());
        return false;
    }
    return SDL_ResumeAudioStreamDevice(m->stream);
}

void music_restart(Music *m)
{
    /* Under the lock: the audio thread must not feed in the middle. */
    SDL_LockAudioStream(m->stream);
    SDL_ClearAudioStream(m->stream);
    m->pushed = 0;
    SDL_UnlockAudioStream(m->stream);
    SDL_ResumeAudioStreamDevice(m->stream);
}

void music_pause(Music *m)
{
    SDL_PauseAudioStreamDevice(m->stream);
}

/*
 * Playback position = frames pushed - frames still queued.
 * The sound card still has its own buffer after the stream: that constant
 * delay will be compensated by the calibration offset (milestone 5).
 */
double music_raw_time(Music *m)
{
    int queued;
    int played;

    if (!m->stream)
        return 0.0;
    SDL_LockAudioStream(m->stream);
    queued = SDL_GetAudioStreamQueued(m->stream) / frame_bytes(m);
    played = m->pushed - queued;
    SDL_UnlockAudioStream(m->stream);
    return played / (double)m->rate;
}

double music_duration(const Music *m)
{
    return m->frames / (double)m->rate;
}

bool music_finished(Music *m)
{
    bool done;

    if (!m->stream)
        return true;
    SDL_LockAudioStream(m->stream);
    done = m->pushed >= m->frames && SDL_GetAudioStreamQueued(m->stream) == 0;
    SDL_UnlockAudioStream(m->stream);
    return done;
}

void music_free(Music *m)
{
    SDL_DestroyAudioStream(m->stream);
    SDL_free(m->samples);
    SDL_zerop(m);
}