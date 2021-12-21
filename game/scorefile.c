#include <rlu/rlu.h>
#include "scorefile.h"

struct ScorefileStruct {
	long length;
	char *filename;
	Score *scores;
};

static FILE *open_file(const char *filename);
static int score_cmp(const void *a, const void *b);
static size_t sfcount(Scorefile *s);

Scorefile *scorefile_open(const char *filename)
{
	FILE *fp;
	Scorefile *s;
	assert(filename);

	// make sure file exists first
	fp = open_file(filename);
	if (!fp) {
		return NULL;
	}

	s = malloc(sizeof(Scorefile));
	assert(s);
	(void)memset(s, 0, sizeof(Scorefile));

	// read the scores
	(void)fseek(fp, 0, SEEK_END);
	s->length = ftell(fp);
	(void)fseek(fp, 0, SEEK_SET);
	if (s->length != 0) {
		s->scores = malloc((size_t)s->length);
		assert(s->scores);
		(void)fread(s->scores, sizeof(Score), sfcount(s), fp);
	}
	else {
		s->scores = NULL;
	}
	(void)fclose(fp);

	s->filename = strdup(filename);
	assert(s->filename);

	return s;
}

void scorefile_close(Scorefile *s)
{
	assert(s);

	if (s->filename) {
		free(s->filename);
	}

	if (s->scores) {
		free(s->scores);
	}

	free(s);
}

Score *scorefile_get_hiscore(Scorefile *s)
{
	assert(s);

	if (!s->scores) {
		return NULL;
	}

	// the scorefile is always in order of highest to lowest
	return &s->scores[0];
}

Score *scorefile_get_scores(Scorefile *s, size_t *count)
{
	assert(s);

	if (s->length == 0) {
		if (count) {
			*count = 0;
		}
		return NULL;
	}

	if (count) {
		*count = sfcount(s);
	}
	return s->scores;
}

void scorefile_add_score(Scorefile *s, const char *name, score_t score)
{
	size_t newsize;
	Score newscore;
	Score *p;

	assert(s);
	assert(name);

	(void)snprintf(newscore.name, sizeof(newscore.name), "%s", name);
	newscore.score = score;

	// at least one element
	if (s->scores) {
		newsize = s->length + sizeof(Score);
		p = realloc(s->scores, newsize);
		assert(p);

		// now 1 longer, so length is the end element
		s->scores = p;
		s->scores[sfcount(s)] = newscore;
		s->length = (long)newsize;

		// needs sorting
		//qsort(s->scores, s->length / sizeof(Score), sizeof(Score), score_cmp);
	}
	// no scores yet
	else {
		s->scores = malloc(sizeof(Score));
		assert(s->scores);

		s->scores[0] = newscore;
		s->length = sizeof(Score);
	}
}

int scorefile_save(Scorefile *s)
{
	FILE *fp;
	assert(s);
	assert(s->filename);

	// nothing to do
	if (!s->scores) {
		return 0;
	}

	fp = fopen(s->filename, "wb");
	if (!fp) {
		return 1;
	}

	(void)fwrite(s->scores, sizeof(Score), sfcount(s), fp);
	(void)fclose(fp);

	return 0;
}

void score_draw(Score *s, float x, float y, int fontsize, Color color)
{
	int measure;
	char buf[32];
	assert(s);

	(void)snprintf(buf, sizeof(buf), "%s - %llu", s->name, s->score);
	measure = MeasureText(buf, fontsize);
	DrawText(buf, GetScreenWidth() * x - measure / 2, GetScreenHeight() * y, fontsize, color);
}

static FILE *open_file(const char *filename)
{
	FILE *fp;
	assert(filename);

	// open and/or create file if necessary
	fp = fopen(filename, "rb");
	if (!fp) {
		fp = fopen(filename, "wb");
		if (!fp) {
			return NULL;
		}
		(void)fclose(fp);

		fp = fopen(filename, "rb");
		if (!fp) {
			return NULL;
		}
	}
	return fp;
}

static int score_cmp(const void *a, const void *b)
{
	Score *_a = (Score *)a;
	Score *_b = (Score *)b;

	if (_a->score > _b->score) {
		return 1;
	}
	else if (_a->score == _b->score) {
		return 0;
	}
	return -1;
}

static size_t sfcount(Scorefile *s)
{
	assert(s);
	return (size_t)s->length / sizeof(Score);
}
