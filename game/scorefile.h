#ifndef MG_SCOREFILE_H
#define MG_SCOREFILE_H

#include <raylib.h>
#include <stdlib.h>
#include "score.h"

typedef struct ScorefileStruct Scorefile;

typedef struct ScoreStruct {
	score_t score;
	char name[4];
} Score;

/** open a score file */
Scorefile *scorefile_open(const char *filename);
/** close a score file */
void scorefile_close(Scorefile *s);
/** get a pointer to the highest score, NULL if none */
Score *scorefile_get_hiscore(Scorefile *s);
/** get a pointer to an array of sorted scores. NULL if none */
Score *scorefile_get_scores(Scorefile *s, size_t *count);
/** insert a score to the score file */
void scorefile_add_score(Scorefile *s, const char *name, score_t score);
/** save all changes made to the score file */
int scorefile_save(Scorefile *s);

/** draw a Score, give percentages on screen for location */
void score_draw(Score *s, float x, float y, int fontsize, Color color);

#endif // MG_SCOREFILE_H
