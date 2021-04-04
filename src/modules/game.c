#include <rlu/rlu.h>
#include "../modules.h"
#include "map.h"

#define PLAYER_SPEED 650
#define PLAYER_SIZE 25
#define ENEMY_SIZE 15
#define ENEMIES_MAX 32

static void encounter_clear(void);
static void encounter_next(void);
static bool encounter_done(void);

static int encounterndx = -1;
static double encounter_starttime = 0.0;
static int enemy_count = 0; // number of loaded enemies
static int screen_height = 0;
static int screen_width = 0;
static double now = 0.0;

static enemy_data enemies[ENEMIES_MAX];

static enemy_move_lookup move_lookup[] = {
	{"down", enemy_move_down},
	{"downleft", enemy_move_downleft},
	{"downright", enemy_move_downright},
	{"downsin", enemy_move_downsin},
	{"downcos", enemy_move_downcos},
	{"downstop", enemy_move_downstop},
	{"horzright", enemy_move_horzright},
	{"horzleft", enemy_move_horzleft},
	{"horzrightstop", enemy_move_horzrightstop},
	{"horzleftstop", enemy_move_horzleftstop},
	{NULL, NULL}
};

static player_data player = {
	//.shoot=bullet_player_parabola,
	.shoot=bullet_player_sin_wide,
	.hp=30,
	.level=2,
};

/**
 * Enemy Prototypes
 */

static encounter **encounters;

void game_init(void)
{
	encounter_clear();
	screen_height = GetScreenHeight();
	screen_width = GetScreenWidth();
	now = GetTime();
	encounterndx = -1;
	player.x = screen_width / 2;
	player.y = screen_height * 3 / 4;
	player.shotperiod = bullet_lookup_timeout(player.shoot);

	encounters = map_init(NULL);
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
	if (dir != -1.0f && (now - player.lastshottime > player.shotperiod)) {
		player.shoot(player.x, player.y, player.level);
		player.lastshottime = now;
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
				enemies[i].shoot(enemies[i].x, enemies[i].y, enemies[i].level);
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
			DrawRectangle(enemies[i].x - ENEMY_SIZE / 2, enemies[i].y - ENEMY_SIZE / 2, ENEMY_SIZE, ENEMY_SIZE, RED);
		}
	}

	DrawRectangle(player.x - PLAYER_SIZE / 2, player.y - PLAYER_SIZE / 2, PLAYER_SIZE, PLAYER_SIZE, BLUE);
	DrawFPS(20, 20);
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
		enemies[enemy_count].level = enc->definition->level;
		memcpy(&enemies[enemy_count].meta, &enc->definition->meta, sizeof(enemies[enemy_count].meta));
		enemies[enemy_count].speed = enc->definition->speed;
		enemies[enemy_count].shotperiod = bullet_lookup_timeout(enc->definition->shoot);
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

enemy_move_func lookup_enemy_move(char *name)
{
	int i;
	assert(name);
	for (i = 0; move_lookup[i].name != NULL; i++) {
		if (strcmp(name, move_lookup[i].name) == 0) {
			return move_lookup[i].move;
		}
	}
	return NULL;
}

// ENEMY MOVE FUNCS ///////////////////////////////////////////////////////////

void enemy_move_down(struct enemy_data_tag *en)
{
	en->y += en->speed;
}

void enemy_move_downleft(struct enemy_data_tag *en)
{
	en->y += en->speed / 2;
	en->x -= en->speed / 2;
}

void enemy_move_downright(struct enemy_data_tag *en)
{

}

void enemy_move_downsin(struct enemy_data_tag *en)
{

}

void enemy_move_downcos(struct enemy_data_tag *en)
{

}

void enemy_move_downstop(struct enemy_data_tag *en)
{

}

void enemy_move_horzright(struct enemy_data_tag *en)
{

}

void enemy_move_horzleft(struct enemy_data_tag *en)
{

}

void enemy_move_horzrightstop(struct enemy_data_tag *en)
{

}

void enemy_move_horzleftstop(struct enemy_data_tag *en)
{

}
