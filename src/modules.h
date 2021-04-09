#ifndef MG_MODULES_H
#define MG_MODULES_H

void mg_init(void);
void mg_cleanup(void);
void mg_update(void);
void mg_draw(void);

void game_init(void);
void game_cleanup(void);
void game_update(void);
void game_draw(void);

void pause_init(void);
void pause_cleanup(void);
void pause_update(void);
void pause_draw(void);

#endif // MG_MODULES_H
