#include <stdio.h>
#include "score.h"

static unsigned long long multiplier = 100;
static unsigned long long points = 0;

void score_init(void)
{
	multiplier = 100;
	points = 0;
}

unsigned long long score_get(void)
{
	return points;
}

char *score_get_string(void)
{
	static char buf[256];
	static unsigned long long points_last = 0;
	if (points_last != points) {
		(void)snprintf(buf, sizeof(buf), "%llu", points);
		points_last = points;
	}
	return buf;
}

char *score_get_multiplier(void)
{
	static char buf[256];
	static unsigned long long mult_last = 0;
	if (mult_last != multiplier) {
		(void)snprintf(buf, sizeof(buf), "x%.2f", (float)multiplier / 100.f);
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
	multiplier = (unsigned long long)(multiplier * 1.5);
}

void score_decrease_multiplier(void)
{
	multiplier = (unsigned long long)(multiplier * 0.9);
	multiplier = (multiplier < 100) ? 100 : multiplier;
}
