#ifndef JUDGE_H
#define JUDGE_H

/* Timing windows, in seconds either side of the note. */
#define WINDOW_PERFECT 0.040
#define WINDOW_GREAT   0.080
#define WINDOW_GOOD    0.120    /* also the Miss limit: later is a Miss */

typedef enum {
    JUDGE_NONE,         /* not judged yet */
    JUDGE_PERFECT,
    JUDGE_GREAT,
    JUDGE_GOOD,
    JUDGE_MISS,
    JUDGE_COUNT
} Judgement;

/* delta = press time - note time (> 0 is late), within WINDOW_GOOD. */
Judgement judge_delta(double delta);
/* Share of a note's points: 1 for Perfect down to 0 for Miss. */
double judge_weight(Judgement j);
const char *judge_name(Judgement j);

#endif
