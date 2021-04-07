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
	(void)snprintf(buf, sizeof(buf), "%llu", points);
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
