#ifndef MG_BULLET_H
#define MG_BULLET_H

/**
 * ALL HITTABLES MUST CONFORM TO THIS BASE STRUCT
 */
typedef struct HITTABLE_OBJECT_TAG {
	int x;
	int y;
	int hp;
} HITTABLE_OBJECT;

typedef struct bullet_data_tag {
	short x;
	short y;
	union {
		float direction; // straight bullets
		float time; // all other bullets
	} meta;
} bullet_data;

typedef void (* shoot_func)(int x, int y);

void bullet_init(int nplayers);
void bullet_cleanup(void);
void bullet_update(void);
void bullet_draw(void);

// will decrement the HP, untrack automatically hit HP <= 0 or offscreen (player should never be removed)
void bullet_track_hittable_enemy(void *hittable);
void bullet_track_hittable_player(void *hittable);

// straight bullets, shot by enemy
void bullet_enemy_straight(int x, int y);
void bullet_enemy_straightdouble(int x, int y);
void bullet_enemy_straighttriple(int x, int y);
void bullet_enemy_spread(int x, int y);
void bullet_enemy_spreaddouble(int x, int y);
void bullet_enemy_spreadtriple(int x, int y);
void bullet_enemy_spreadquad(int x, int y);
void bullet_enemy_left(int x, int y);
void bullet_enemy_right(int x, int y);
void bullet_enemy_spin(int x, int y); // circular
void bullet_enemy_flower(int x, int y); // 8 directions
// straight bullets, shot by player
void bullet_player_straight(int x, int y);
void bullet_player_straightdouble(int x, int y);
void bullet_player_straighttriple(int x, int y);
void bullet_player_spread(int x, int y);
void bullet_player_spreaddouble(int x, int y);
void bullet_player_spreadtriple(int x, int y);
void bullet_player_spreadquad(int x, int y);
void bullet_player_left(int x, int y);
void bullet_player_right(int x, int y);
void bullet_player_spin(int x, int y); // circular
void bullet_player_flower(int x, int y); // 8 directions

// sine / cosine bullets, shot by enemy
void bullet_enemy_sin(int x, int y);
void bullet_enemy_sin_wide(int x, int y);
// sine / cosine bullets, shot by player
void bullet_player_sin(int x, int y);
void bullet_player_sin_wide(int x, int y);

// big bullets, straight
void bullet_enemy_big(int x, int y);
void bullet_player_big(int x, int y);

// beam, (fast widish, straight)
void bullet_enemy_beam(int x, int y);
void bullet_player_beam(int x, int y);

#endif MG_BULLET_H
