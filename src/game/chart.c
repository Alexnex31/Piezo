#include "game/chart.h"

/* Cuts the line at '#' and trims spaces on both ends, in place. */
static char *clean_line(char *line)
{
    char *hash = SDL_strchr(line, '#');
    char *end;

    if (hash)
        *hash = '\0';
    while (SDL_isspace(*line))
        line++;
    end = line + SDL_strlen(line);
    while (end > line && SDL_isspace(end[-1]))
        end--;
    *end = '\0';
    return line;
}

/* Whole string must be a number: "120" is fine, "120abc" is not. */
static bool parse_number(const char *s, double *out)
{
    char *end;

    *out = SDL_strtod(s, &end);
    return end != s && *end == '\0';
}

/* "click120.ogg" next to "assets/x.chart" gives "assets/click120.ogg". */
static char *song_path_from(const char *chart_path, const char *song)
{
    const char *slash = SDL_strrchr(chart_path, '/');
    char *path = NULL;

    if (song[0] == '/' || !slash)
        return SDL_strdup(song);
    if (SDL_asprintf(&path, "%.*s/%s", (int)(slash - chart_path),
                     chart_path, song) < 0)
        return NULL;
    return path;
}

static bool add_note(Chart *c, double time_ms, int column)
{
    Note *grown;
    int capacity;

    if (c->count == c->capacity) {
        capacity = c->capacity ? c->capacity * 2 : 256;
        grown = SDL_realloc(c->notes, capacity * sizeof(Note));
        if (!grown)
            return false;
        c->notes = grown;
        c->capacity = capacity;
    }
    c->notes[c->count].time = time_ms / 1000.0;
    c->notes[c->count].column = column - 1;
    c->count++;
    return true;
}

/* "<time_ms> <column>" */
static bool parse_note(Chart *c, char *line)
{
    char *end;
    double time_ms = SDL_strtod(line, &end);
    long column;

    if (end == line || !SDL_isspace(*end))
        return false;
    line = end;
    column = SDL_strtol(line, &end, 10);
    if (end == line || *end != '\0')
        return false;
    if (time_ms < 0.0 || column < 1 || column > CHART_COLUMNS)
        return false;
    return add_note(c, time_ms, (int)column);
}

/* "<key> <value>" */
static bool parse_header(Chart *c, const char *chart_path, char *line)
{
    char *value = line;
    double ms;

    while (*value && !SDL_isspace(*value))
        value++;
    if (*value)
        *value++ = '\0';
    while (SDL_isspace(*value))
        value++;
    if (SDL_strcmp(line, "song") == 0 && *value) {
        SDL_free(c->song_path);
        c->song_path = song_path_from(chart_path, value);
        return c->song_path != NULL;
    }
    if (SDL_strcmp(line, "bpm") == 0)
        return parse_number(value, &c->bpm) && c->bpm > 0.0;
    if (SDL_strcmp(line, "offset") == 0 && parse_number(value, &ms)) {
        c->offset = ms / 1000.0;
        return true;
    }
    return false;
}

static int compare_notes(const void *a, const void *b)
{
    const Note *na = a;
    const Note *nb = b;

    if (na->time != nb->time)
        return na->time < nb->time ? -1 : 1;
    return na->column - nb->column;
}

bool chart_load(Chart *c, const char *path)
{
    char *text = SDL_LoadFile(path, NULL);
    char *line;
    char *next;
    int number = 0;
    bool ok = true;

    SDL_zerop(c);
    if (!text) {
        SDL_Log("Could not read chart '%s': %s", path, SDL_GetError());
        return false;
    }
    for (line = text; ok && line; line = next) {
        next = SDL_strchr(line, '\n');
        if (next)
            *next++ = '\0';
        number++;
        line = clean_line(line);
        if (*line == '\0')
            continue;
        ok = SDL_isalpha(*line) ? parse_header(c, path, line)
                                : parse_note(c, line);
        if (!ok)
            SDL_Log("%s:%d: invalid line", path, number);
    }
    SDL_free(text);
    if (ok && (!c->song_path || c->bpm <= 0.0)) {
        SDL_Log("%s: needs a 'song' and a 'bpm' line", path);
        ok = false;
    }
    if (!ok) {
        chart_free(c);
        return false;
    }
    SDL_qsort(c->notes, c->count, sizeof(Note), compare_notes);
    return true;
}

void chart_free(Chart *c)
{
    SDL_free(c->song_path);
    SDL_free(c->notes);
    SDL_zerop(c);
}
