#include <rlu/rlu.h>
#include "../modules.h"

#define PLAYER_SPEED 650
#define PLAYER_SIZE 25
#define ENEMY_SIZE 15
#define BULLET_SIZE 5

struct enemy_data_tag;

typedef void (* shoot_new_func)(int x, int y);
typedef void (* enemy_move_func)(struct enemy_data_tag *en);

typedef struct player_data_tag {
	int x;
	int y;
	int hp;
	shoot_new_func shoot;
} player_data;

typedef struct enemy_data_tag {
	shoot_new_func shoot;
	enemy_move_func move;
	int x;
	int y;
	int hp;
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
	shoot_new_func shoot;
	enemy_move_func move;
	float x; // percent -> pixels
	float y; // percent -> pixels
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
} encounter;

#define DEFINE_ENCOUNTER(EnemyDef, Time) \
	{ \
		.definition=(EnemyDef), \
		.spawntime=(Time), \
	}

typedef struct bullet_data_tag {
	int x;
	int y;
	float speed;
	float direction;
} bullet_data;

static void encounter_clear(void);
static void encounter_next(void);
static bool encounter_done(void);

static void bullet_insert(int x, int y, float speed, float direction);

static void shoot_downstraight(int x, int y);
static void shoot_downstraightdouble(int x, int y);
static void shoot_downstraighttriple(int x, int y);
static void shoot_downspread(int x, int y);
static void shoot_downspreaddouble(int x, int y);
static void shoot_downspreadtriple(int x, int y);
static void shoot_downspreadquad(int x, int y);
static void shoot_downleft(int x, int y);
static void shoot_downright(int x, int y);

static void shoot_upstraight(int x, int y);
static void shoot_upstraightdouble(int x, int y);
static void shoot_upstraighttriple(int x, int y);
static void shoot_upspread(int x, int y);
static void shoot_upspreaddouble(int x, int y);
static void shoot_upspreadtriple(int x, int y);
static void shoot_upspreadquad(int x, int y);
static void shoot_upleft(int x, int y);
static void shoot_upright(int x, int y);

static void shoot_horzright(int x, int y);
static void shoot_horzleft(int x, int y);
static void shoot_sin(int x, int y);
static void shoot_flower(int x, int y);

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
	.shoot = shoot_upstraight,
	.hp = 30,
};
static enemy_data enemies[32];
static bullet_data bullets[1024];

static int bulletndx = 0;
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
	.shoot=shoot_downstraight,
	.move=enemy_move_downleft,
	.x=1.0,
	.y=0.0,
	.hp=3,
	.speed=10,
	.shotperiod=0.1,
};

static encounter encounter0[] = {
	DEFINE_ENCOUNTER(&left_drifter_def, 0),
	DEFINE_ENCOUNTER(&left_drifter_def, 2),
	DEFINE_ENCOUNTER(&left_drifter_def, 4),
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

	for (i = 0; i < ARRAY_SIZE(bullets); i++) {
		// offscreen
		if (bullets[i].y > screen_height || bullets[i].y <= -BULLET_SIZE) {
			continue;
		}

		// move bullets
		bullets[i].x += bullets[i].speed * cosf(bullets[i].direction);
		bullets[i].y += bullets[i].speed * -sinf(bullets[i].direction);

		// hit player?
		if (bullets[i].x >= player.x - PLAYER_SIZE / 2 && bullets[i].x <= player.x + PLAYER_SIZE / 2 &&
		    bullets[i].y <= player.y + PLAYER_SIZE / 2 && bullets[i].y >= player.y - PLAYER_SIZE / 2)
		{
			bullets[i].y = -BULLET_SIZE;
			player.hp--;
			continue;
		}

		// hit enemy?
		for (j = 0; j < enemy_count; j++) {
			if (bullets[i].x >= enemies[j].x - PLAYER_SIZE / 2 && bullets[i].x <= enemies[j].x + PLAYER_SIZE / 2 &&
			    bullets[i].y <= enemies[j].y + PLAYER_SIZE / 2 && bullets[i].y >= enemies[j].y - PLAYER_SIZE / 2)
			{
				enemies[j].hp--;
				break; // NOTE: This should skip to the next outer for loop iteration. Nothing after this inner one otherwise goto
			}
		}
	}
}

void game_draw(void)
{
	int i;
	for (i = 0; i < enemy_count; i++) {
		if (enemies[i].hp >= 0 && (now - encounter_starttime >= enemies[i].spawntime)) {
			DrawRectangle(enemies[i].x, enemies[i].y, ENEMY_SIZE, ENEMY_SIZE, RED);
		}
	}

	DrawRectangle(player.x, player.y, PLAYER_SIZE, PLAYER_SIZE, BLUE);

	for (i = 0; i < ARRAY_SIZE(bullets); i++) {
		// offscreen
		if (bullets[i].y > screen_height || bullets[i].y <= -BULLET_SIZE) {
			continue;
		}

		DrawRectangle(bullets[i].x, bullets[i].y, BULLET_SIZE, BULLET_SIZE, YELLOW);
	}
}

static void encounter_clear(void)
{
	int i;
	(void)memset(enemies, 0, sizeof(enemies));
	for (i = 0; i < ARRAY_SIZE(bullets); i++) {
		bullets[i].y = -BULLET_SIZE;
	}
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

	encounterndx++;
	for (enc = encounters[encounterndx]; enc->definition != NULL; enc++) {
		enemies[enemy_count].shoot = enc->definition->shoot;
		enemies[enemy_count].move = enc->definition->move;
		enemies[enemy_count].x = (int)(enc->definition->x * (float)screen_width);
		enemies[enemy_count].y = (int)(enc->definition->y * (float)screen_height);
		enemies[enemy_count].hp = enc->definition->hp;
		memcpy(&enemies[enemy_count].movedat, &enc->definition->movedat, sizeof(enemies[enemy_count].movedat));
		enemies[enemy_count].speed = enc->definition->speed;
		enemies[enemy_count].shotperiod = enc->definition->shotperiod;
		enemies[enemy_count].spawntime = enc->spawntime;
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

static void bullet_insert(int x, int y, float speed, float direction)
{
	bullets[bulletndx].x = x;
	bullets[bulletndx].y = y;
	bullets[bulletndx].speed = speed;
	bullets[bulletndx].direction = direction;
	bulletndx = (bulletndx + 1) % ARRAY_SIZE(bullets);
}

static void shoot_downstraight(int x, int y)
{
	bullet_insert(x, y + ENEMY_SIZE, 10, PI * 3 / 2);
}

static void shoot_downstraightdouble(int x, int y)
{
	
}

static void shoot_downstraighttriple(int x, int y)
{
	
}

static void shoot_downspread(int x, int y)
{
	
}

static void shoot_downspreaddouble(int x, int y)
{
	
}

static void shoot_downspreadtriple(int x, int y)
{
	
}

static void shoot_downspreadquad(int x, int y)
{
	
}

static void shoot_downleft(int x, int y)
{

}

static void shoot_downright(int x, int y)
{

}

static void shoot_upstraight(int x, int y)
{
	bullet_insert(x + PLAYER_SIZE / 2 - BULLET_SIZE / 2, y - PLAYER_SIZE, 10, PI / 2.0f);
}

static void shoot_upstraightdouble(int x, int y)
{

}

static void shoot_upstraighttriple(int x, int y)
{

}

static void shoot_upspread(int x, int y)
{

}

static void shoot_upspreaddouble(int x, int y)
{

}

static void shoot_upspreadtriple(int x, int y)
{

}

static void shoot_upspreadquad(int x, int y)
{

}

static void shoot_upleft(int x, int y)
{

}

static void shoot_upright(int x, int y)
{

}

static void shoot_horzright(int x, int y)
{

}

static void shoot_horzleft(int x, int y)
{

}

static void shoot_sin(int x, int y)
{

}

static void shoot_flower(int x, int y)
{

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
