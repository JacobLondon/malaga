#include <rlu/rlu.h>
#include "scorefile.h"

struct ScorefileStruct {
	long length;
	char *filename;
	Score *scores;
};

/** fp must be open for reading in binary */
static FILE *open_file(const char *filename);
static int score_cmp(const void *a, const void *b);

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
	if (s->length != 0) {
		s->scores = malloc(s->length);
		assert(s->scores);
		fread(s->scores, 1, s->length, fp);
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
		*count = s->length / sizeof(Score);
	}
	return s->scores;
}

void scorefile_add_score(Scorefile *s, const char *name, score_t score)
{
	Score newscore;
	Score *p;

	assert(s);
	assert(name);

	(void)snprintf(newscore.name, sizeof(newscore.name), "%s", name);
	newscore.score = score;

	// at least one element
	if (s->scores) {
		p = realloc(s->scores, s->length + sizeof(Score));
		assert(p);

		// now 1 longer, so length is the end element
		s->scores = p;
		s->scores[s->length / sizeof(Score)] = newscore;
		s->length += sizeof(Score);

		// needs sorting
		qsort(s->scores, s->length / sizeof(Score), sizeof(Score), score_cmp);
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

	(void)fwrite(s->scores, 1, s->length, fp);
	(void)fclose(fp);

	return 0;
}

void score_draw(Score *s, float x, float y, int fontsize, Color color)
{
	int measure;
	char buf[32];
	assert(s);

	(void)snprintf(buf, sizeof(buf), "%s  %llu", s->name, s->score);
	measure = MeasureText(buf, fontsize);
	DrawText(buf, GetScreenWidth() * x - measure / 2, GetScreenHeight() * y - fontsize / 2, fontsize, color);
}

static FILE *open_file(const char *filename)
{
	FILE *fp;
	assert(filename);

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
	Score *_a = a;
	Score *_b = b;

	if (_a->score > _b->score) {
		return 1;
	}
	else if (_a->score == _b->score) {
		return 0;
	}
	return -1;
}
