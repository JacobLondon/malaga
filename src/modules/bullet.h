#ifndef MG_BULLET_H
#define MG_BULLET_H

/**
 * ALL HITTABLES MUST CONFORM TO THIS BASE STRUCT AND
 * THE BEGINNING MUST CONFORM TO RAYLIB RECTANGLE STRUCT
 */
typedef struct HITTABLE_OBJECT_TAG {
	int x;
	int y;
	int width;
	int height;
	int hp;
} HITTABLE_OBJECT;

typedef struct bullet_data_tag {
	short x;
	short y;
	float direction;
	float time;
} bullet_data;

typedef void (* shoot_func)(int x, int y, int level);

void bullet_init(int nplayers);
void bullet_cleanup(void);
void bullet_clear(void);
void bullet_update(void);
void bullet_draw(void);

// all bullet_[enemy|player].* Really only do once on load then save result
float bullet_lookup_timeout(shoot_func bullet_func);
shoot_func bullet_lookup_shoot(char *name);

// will decrement the HP, untrack automatically hit HP <= 0 or offscreen (player should never be removed)
void bullet_track_hittable_enemy(void *hittable);
void bullet_track_hittable_player(void *hittable);

// SHOOT FUNCS ////////////////////////////////////////////////////////////////

// straight bullets, shot by enemy
void bullet_enemy_straight(int x, int y, int level);
void bullet_enemy_left(int x, int y, int level);
void bullet_enemy_right(int x, int y, int level);
void bullet_enemy_spin(int x, int y, int level); // circular
void bullet_enemy_flower(int x, int y, int level); // 8 directions
// straight bullets, shot by player
void bullet_player_straight(int x, int y, int level);
void bullet_player_left(int x, int y, int level);
void bullet_player_right(int x, int y, int level);
void bullet_player_spin(int x, int y, int level); // circular
void bullet_player_flower(int x, int y, int level); // 8 directions

// sine / cosine bullets, shot by enemy
void bullet_enemy_sin(int x, int y, int level);
void bullet_enemy_sin_wide(int x, int y, int level);
// sine / cosine bullets, shot by player
void bullet_player_sin(int x, int y, int level);
void bullet_player_sin_wide(int x, int y, int level);

// big bullets, straight
void bullet_enemy_big(int x, int y, int level);
void bullet_player_big(int x, int y, int level);

// beam, (fast widish, straight)
void bullet_enemy_beam(int x, int y, int level);
void bullet_player_beam(int x, int y, int level);

void bullet_player_parabola(int x, int y, int level);

#endif // MG_BULLET_H
