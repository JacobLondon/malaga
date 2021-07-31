#ifndef MG_GAME_H
#define MG_GAME_H

#include <rlu/rlu.h>
#include "bullet.h"

struct enemy_data_tag;

typedef void (* enemy_move_func)(struct enemy_data_tag *en);
typedef struct enemy_move_lookup_tag {
	const char *name;
	enemy_move_func move;
} enemy_move_lookup;

/**
 * CONFORMING TO HITTABLE_OBJECT
 */
typedef struct player_data_tag {
	// begin HITTABLE_OBJECT
	int x;
	int y;
	int width;
	int height;
	int hp;
	// end HITTABLE_OBJECT
	int speed; // move speed
	int level;
	shoot_func shoot;
	float lastshottime;
	float shotperiod;
	float lasttouchtime; // last time player touched enemy
	so *object;
} player_data;

/**
 * CONFORMING TO HITTABLE_OBJECT
 */
typedef struct enemy_data_tag {
	// begin HITTABLE_OBJECT
	int x;
	int y;
	int width;
	int height;
	int hp;
	// end HITTABLE_OBJECT
	int level;
	shoot_func shoot;
	enemy_move_func move;
	union {
		float downstop;
		float horzrightstop;
		float horzleftstop;
	} meta;
	float speed; // units / s
	float shotperiod; // seconds per bullet
	float lastshottime;
	float spawntime; // s
	float _timestamp;
	so *object;
} enemy_data;

typedef struct enemy_definition_tag {
	shoot_func shoot;
	enemy_move_func move;
	int width;
	int height;
	int hp;
	int level;
	// percent -> pixels
	union {
		float downstop;
		float horzrightstop;
		float horzleftstop;
	} meta;
	float speed; // units / s
	char *name;
	char *pngname;
} enemy_definition;

/**
 * An encounter with a single enemy
 */
typedef struct encounter_tag {
	enemy_definition *definition;
	float spawntime;
	float x; // percent -> pixels
	float y; // percent -> pixels
} encounter;

#define DEFINE_ENCOUNTER(EnemyDef, Time, X, Y) \
	{ \
		.definition=(EnemyDef), \
		.spawntime=(Time), \
		.x=(X), \
		.y=(Y), \
	}

void player_took_damage(player_data *player);
void enemy_took_damage(enemy_data *enemy);
void enemy_took_death(enemy_data *enemy);

enemy_move_func lookup_enemy_move(char *name);

void enemy_move_down(struct enemy_data_tag *en);
void enemy_move_downleft(struct enemy_data_tag *en);
void enemy_move_downright(struct enemy_data_tag *en);
void enemy_move_downsin(struct enemy_data_tag *en);
void enemy_move_downcos(struct enemy_data_tag *en);
void enemy_move_downstop(struct enemy_data_tag *en);
void enemy_move_horzright(struct enemy_data_tag *en);
void enemy_move_horzleft(struct enemy_data_tag *en);
void enemy_move_horzrightstop(struct enemy_data_tag *en);
void enemy_move_horzleftstop(struct enemy_data_tag *en);

#endif // MG_GAME_H
