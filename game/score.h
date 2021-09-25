#ifndef MG_SCORE_H
#define MG_SCORE_H

void score_init(void);
unsigned long long score_get(void);
char *score_get_string(void);
char *score_get_multiplier(void);
void score_increase_points(void);
void score_increase_multiplier(void);
void score_decrease_multiplier(void);

#endif // MG_SCORE_H
