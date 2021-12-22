#ifndef MG_MODULES_H
#define MG_MODULES_H

void mg_init(void);
void mg_cleanup(void);
void mg_update(void);
void mg_draw(void);

struct game_message {
	int playership;
	char mapdir[128];
	int endless_mode;
	int trouble_mode;
	int god_mode;
};

void game_init(void);
void game_cleanup(void);
void game_update(void);
void game_draw(void);
void game_conf(struct game_message *msg);

void pause_init(void);
void pause_cleanup(void);
void pause_update(void);
void pause_draw(void);

void levelselect_init(void);
void levelselect_cleanup(void);
void levelselect_update(void);
void levelselect_draw(void);

void skins_init(void);
void skins_cleanup(void);
void skins_update(void);
void skins_draw(void);

struct hiscore_message {
	int won;
	char scorefile[128];
	unsigned long long score;
};

void hiscore_init(void);
void hiscore_cleanup(void);
void hiscore_update(void);
void hiscore_draw(void);
void hiscore_conf(struct hiscore_message *msg);

void scorelist_init(void);
void scorelist_cleanup(void);
void scorelist_update(void);
void scorelist_draw(void);

#endif // MG_MODULES_H
