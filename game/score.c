#include <stdio.h>
#include "score.h"

static score_t multiplier = 100;
static score_t points = 0;

void score_init(void)
{
	multiplier = 100;
	points = 0;
}

score_t score_get(void)
{
	return points;
}

char *score_get_string(void)
{
	static char buf[256];
	static score_t points_last = 1;
	if (points_last != points) {
		(void)snprintf(buf, sizeof(buf), "%llu", points);
		points_last = points;
	}
	return buf;
}

char *score_get_multiplier(void)
{
	static char buf[256];
	static score_t mult_last = 0;
	if (mult_last != multiplier) {
		(void)snprintf(buf, sizeof(buf), "x%.2lf", (float)multiplier / 100.0);
		mult_last = multiplier;
	}
	return buf;
}

void score_increase_points(void)
{
	points += multiplier;
}

void score_increase_multiplier(void)
{
	multiplier = (score_t)(multiplier * 1.5);
}

void score_decrease_multiplier(void)
{
	multiplier = (score_t)(multiplier * 0.9);
	multiplier = (multiplier < 100) ? 100 : multiplier;
}
