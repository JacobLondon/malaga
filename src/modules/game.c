#include <rlu/rlu.h>
#include "../modules.h"
#include "bullet.h"

#define PLAYER_SPEED 650
#define PLAYER_SIZE 25
#define ENEMY_SIZE 15
#define BULLET_SIZE 5

struct enemy_data_tag;

typedef void (* enemy_move_func)(struct enemy_data_tag *en);

/**
 * CONFORMING TO HITTABLE_OBJECT
 */
typedef struct player_data_tag {
	// begin HITTABLE_OBJECT
	int x;
	int y;
	int hp;
	// end HITTABLE_OBJECT
	shoot_func shoot;
} player_data;

/**
 * CONFORMING TO HITTABLE_OBJECT
 */
typedef struct enemy_data_tag {
	// begin HITTABLE_OBJECT
	int x;
	int y;
	int hp;
	// end HITTABLE_OBJECT
	shoot_func shoot;
	enemy_move_func move;
	union {
		int downstop;
		int horzrightstop;
		int horzleftstop;
	} movedat;
	float speed; // units / s
	float shotperiod; // seconds per bullet
	float lastshottime;
	float spawntime; // s
} enemy_data;

typedef struct enemy_definition_tag {
	shoot_func shoot;
	enemy_move_func move;
	int hp;
	// percent -> pixels
	union {
		float downstop;
		float horzrightstop;
		float horzleftstop;
	} movedat;
	float speed; // units / s
	float shotperiod; // bullets / s
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

static void encounter_clear(void);
static void encounter_next(void);
static bool encounter_done(void);

static void enemy_move_down(struct enemy_data_tag *en);
static void enemy_move_downleft(struct enemy_data_tag *en);
static void enemy_move_downright(struct enemy_data_tag *en);
static void enemy_move_downsin(struct enemy_data_tag *en);
static void enemy_move_downcos(struct enemy_data_tag *en);
static void enemy_move_downstop(struct enemy_data_tag *en);
static void enemy_move_horzright(struct enemy_data_tag *en);
static void enemy_move_horzleft(struct enemy_data_tag *en);
static void enemy_move_horzrightstop(struct enemy_data_tag *en);
static void enemy_move_horzleftstop(struct enemy_data_tag *en);

static player_data player = {
	.shoot = bullet_player_straight,
	.hp = 30,
};
static enemy_data enemies[32];

static int encounterndx = -1;
static double encounter_starttime = 0.0;
static int enemy_count = 0; // number of loaded enemies
static int screen_height = 0;
static int screen_width = 0;
static double now = 0.0;

/**
 * Enemy Prototypes
 */
static enemy_definition left_drifter_def = {
	.shoot=bullet_enemy_straight,
	.move=enemy_move_downleft,
	.hp=3,
	.speed=10,
	.shotperiod=0.1,
};

static encounter encounter0[] = {
	DEFINE_ENCOUNTER(&left_drifter_def, 0, 1, 0),
	DEFINE_ENCOUNTER(&left_drifter_def, 2, 1, 0),
	DEFINE_ENCOUNTER(&left_drifter_def, 4, 1, 0),
	{NULL}
};

static encounter *encounters[] = {
	encounter0,
	NULL
};

void game_init(void)
{
	encounter_clear();
	screen_height = GetScreenHeight();
	screen_width = GetScreenWidth();
	now = GetTime();
	encounterndx = -1;
}

void game_cleanup(void)
{
	encounter_clear();
}

void game_update(void)
{
	int i;
	int j;
	float dir;

	screen_width = GetScreenWidth();
	screen_height = GetScreenHeight();
	now = GetTime();

	// move player
	rlu_input_prescan(0);
	dir = rlu_input_axis(0, RLU_KEY_STICK_LEFT_X);
	if (dir > 0) {
		if (player.x >= 0) {
			player.x += (GetFrameTime() * PLAYER_SPEED * dir);
		}
		else {
			player.x = 0;
		}
	}
	else if (dir < 0)
	{
		if ((player.x + PLAYER_SIZE <= screen_width)) {
			player.x += (GetFrameTime() * PLAYER_SPEED * dir);
		}
		else {
			player.x = screen_width - PLAYER_SIZE;
		}
	}

	dir = rlu_input_axis(0, RLU_KEY_STICK_LEFT_Y);
	if (dir > 0) {
		if ((player.y >= 0)) {
			player.y += (GetFrameTime() * PLAYER_SPEED * dir);
		}
		else {
			player.y = 0;
		}
	}
	else if (dir < 0)
	{
		if ((player.y + PLAYER_SIZE <= screen_height)) {
			player.y += (GetFrameTime() * PLAYER_SPEED * dir);
		}
		else {
			player.y = screen_height - PLAYER_SIZE;
		}
	}

	/*if (IsGamepadAvailable(0)) {
		if (GetGamepadButtonPressed() != -1) DrawText(TextFormat("DETECTED BUTTON: %i", GetGamepadButtonPressed()), 10, 430, 10, RED);
		else DrawText("DETECTED BUTTON: NONE", 10, 430, 10, GRAY);
	}*/

	dir = rlu_input_axis(0, RLU_KEY_TRIGGER_RIGHT);
	if (dir != -1.0f) {
		player.shoot(player.x, player.y);
	}

	if (encounter_done()) {
		encounter_next();
	}

	// move enemies
	for (i = 0; i < ARRAY_SIZE(enemies); i++) {
		if (enemies[i].hp <= 0) {
			continue;
		}
		else if (enemies[i].y > screen_height) {
			enemies[i].hp = 0;
		}
		else if (now - encounter_starttime > enemies[i].spawntime) {
			enemies[i].move(&enemies[i]);

			// shoot because loaded
			if (now - enemies[i].lastshottime > enemies[i].shotperiod) {
				enemies[i].shoot(enemies[i].x, enemies[i].y);
				enemies[i].lastshottime = now;
			}
		}
	}

	bullet_update();
}

void game_draw(void)
{
	int i;

	bullet_draw();

	for (i = 0; i < enemy_count; i++) {
		if (enemies[i].hp >= 0 && (now - encounter_starttime >= enemies[i].spawntime)) {
			DrawRectangle(enemies[i].x, enemies[i].y, ENEMY_SIZE, ENEMY_SIZE, RED);
		}
	}

	DrawRectangle(player.x, player.y, PLAYER_SIZE, PLAYER_SIZE, BLUE);
}

static void encounter_clear(void)
{
	;
}

static void encounter_next(void)
{
	encounter *enc;

	encounter_starttime = now;
	enemy_count = 0;

	// we done
	if (encounters[encounterndx + 1] == NULL) {
		return;
	}

	// TODO: Can you shoot an enemy who isn't showing up yet?

	bullet_init(1);
	bullet_track_hittable_player(&player);
	encounterndx++;
	for (enc = encounters[encounterndx]; enc->definition != NULL; enc++) {
		enemies[enemy_count].shoot = enc->definition->shoot;
		enemies[enemy_count].move = enc->definition->move;
		enemies[enemy_count].x = (int)(enc->x * (float)screen_width);
		enemies[enemy_count].y = (int)(enc->y * (float)screen_height);
		enemies[enemy_count].hp = enc->definition->hp;
		memcpy(&enemies[enemy_count].movedat, &enc->definition->movedat, sizeof(enemies[enemy_count].movedat));
		enemies[enemy_count].speed = enc->definition->speed;
		enemies[enemy_count].shotperiod = enc->definition->shotperiod;
		enemies[enemy_count].spawntime = enc->spawntime;

		bullet_track_hittable_enemy(&enemies[enemy_count]);
		enemy_count++;
	}
}

static bool encounter_done(void)
{
	int i;

	if (encounterndx == -1) {
		return true;
	}

	// below screen or <= 0 hp means dead
	for (i = 0; i < enemy_count; i++) {
		if (enemies[i].hp > 0) {
			return false;
		}
	}
	return true;
}

static void enemy_move_down(struct enemy_data_tag *en)
{
	en->y += en->speed;
}

static void enemy_move_downleft(struct enemy_data_tag *en)
{
	en->y += en->speed / 2;
	en->x -= en->speed / 2;
}

static void enemy_move_downright(struct enemy_data_tag *en)
{

}

static void enemy_move_downsin(struct enemy_data_tag *en)
{

}

static void enemy_move_downcos(struct enemy_data_tag *en)
{

}

static void enemy_move_downstop(struct enemy_data_tag *en)
{

}

static void enemy_move_horzright(struct enemy_data_tag *en)
{

}

static void enemy_move_horzleft(struct enemy_data_tag *en)
{

}

static void enemy_move_horzrightstop(struct enemy_data_tag *en)
{

}

static void enemy_move_horzleftstop(struct enemy_data_tag *en)
{

}
