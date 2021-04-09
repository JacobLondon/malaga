#include <rlu/rlu.h>
#include "../modules.h"
#include "map.h"
#include "score.h"
#include "atmos.h"

#define WIN_TEXT "- WINNER -"
#define LOSE_TEXT "- LOSER -"
#define PLAYER_SPEED 650
#define PLAYER_SIZE 25
#define ENEMY_SIZE 35
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
static bool gamewon = false;
static bool gamelost = false;

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
	.lasthp=30,
	.level=2,
	.width=20,
	.height=20,
};

/**
 * Enemy Prototypes
 */

static encounter **encounters;
static Image icon;

void game_init(void)
{
	encounter_clear();
	screen_height = GetScreenHeight();
	screen_width = GetScreenWidth();
	now = GetTime();
	encounterndx = -1;
	player.x = screen_width / 2;
	player.y = screen_height * 3 / 4;
	gamewon = false;
	gamelost = false;
	player.shotperiod = bullet_lookup_timeout(player.shoot);

	//encounters = map_init(NULL);
	encounters = map_init("maps/test.mg");
	score_init();
	atmos_init("assets");

	icon = texture_man_img_load_or_default("icon.png", 30, 30, BLUE);
	SetWindowIcon(icon);

	// TODO: This is a hack
	player.hp = 30;
	player.lasthp = 30;
}

void game_cleanup(void)
{
	encounter_clear();
	map_cleanup();
	atmos_cleanup();
	UnloadImage(icon);
}

void game_update(void)
{
	int i, j;
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

	if (player.y + PLAYER_SIZE > screen_height) {
		player.y = screen_height - PLAYER_SIZE / 2;
	}
	else if (player.y < PLAYER_SIZE / 2) {
		player.y = PLAYER_SIZE / 2;
	}
	if (player.x + PLAYER_SIZE > screen_width) {
		player.x = screen_width - PLAYER_SIZE / 2;
	}
	else if (player.x < PLAYER_SIZE / 2) {
		player.x = PLAYER_SIZE / 2;
	}

	if (IsKeyPressed(KEY_R)) {
		context_switch("GAME");
	}
	if (IsKeyPressed(KEY_ENTER)) {
		context_push("PAUSE");
	}

	/*if (IsGamepadAvailable(0)) {
		if (GetGamepadButtonPressed() != -1) DrawText(TextFormat("DETECTED BUTTON: %i", GetGamepadButtonPressed()), 10, 430, 10, RED);
		else DrawText("DETECTED BUTTON: NONE", 10, 430, 10, GRAY);
	}*/

	if (gamelost == false) {
		dir = rlu_input_axis(0, RLU_KEY_TRIGGER_RIGHT);
		if (dir != -1.0f && (now - player.lastshottime > player.shotperiod)) {
			player.shoot(player.x, player.y, player.level);
			player.lastshottime = now;
		}
	}

	if (encounter_done()) {
		encounter_next();
	}

	// move enemies
	for (i = 0; i < ARRAY_SIZE(enemies); i++) {
		if (enemies[i].hp <= 0) {
			enemies[i].y = 20000; // wayyyyy off screen, probably
			continue;
		}
		else if (
			(enemies[i].y > screen_height + 5 * ENEMY_SIZE) ||
			(enemies[i].y < -5 * ENEMY_SIZE) ||
			(enemies[i].x > screen_width + 5 * ENEMY_SIZE) ||
			(enemies[i].x < -5 * ENEMY_SIZE)
		)
		{
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

		if (CheckCollisionRecs(*(Rectangle *)&enemies[i], *(Rectangle *)&player)) {
			if (now - player.lasttouchtime > 1.f) {
				player.lasttouchtime = now;
				player.hp--;
			}
		}
	}

	bullet_update();
	atmos_update();

	if (player.lasthp != player.hp) {
		score_decrease_multiplier();
	}
	player.lasthp = player.hp;

	if (player.hp <= 0) {
		gamelost = true;
	}
}

void game_draw(void)
{
	static char health[128];
	int i;

	atmos_draw();
	bullet_draw();

	for (i = 0; i < enemy_count; i++) {
		if (enemies[i].hp >= 0 && (now - encounter_starttime >= enemies[i].spawntime)) {
			DrawRectangle(enemies[i].x - ENEMY_SIZE / 2, enemies[i].y - ENEMY_SIZE / 2, ENEMY_SIZE, ENEMY_SIZE, RED);
		}
	}

	if (gamelost == false) {
		DrawRectangle(player.x - PLAYER_SIZE / 2, player.y - PLAYER_SIZE / 2, PLAYER_SIZE, PLAYER_SIZE, BLUE);
	}

	snprintf(health, sizeof(health), "HP %d", player.hp);
	DrawText(health, 5, 5, 30, WHITE);
	DrawText(score_get_string(), screen_width - 5 - MeasureText(score_get_string(), 30), 5, 30, WHITE);
	DrawText(score_get_multiplier(), screen_width - 5 - MeasureText(score_get_multiplier(), 30), 40, 30, WHITE);
	//DrawFPS(20, 20);
	if (gamewon && !gamelost) {
		DrawRectangle(GetScreenWidth() / 2 - MeasureText(WIN_TEXT, 40) / 2, GetScreenHeight() / 4, MeasureText(WIN_TEXT, 40), 40, BLACK);
		DrawText(WIN_TEXT, GetScreenWidth() / 2 - MeasureText(WIN_TEXT, 40) / 2, GetScreenHeight() / 4, 40, WHITE);
	}
	else if (gamelost && !gamewon) {
		DrawRectangle(GetScreenWidth() / 2 - MeasureText(WIN_TEXT, 40) / 2, GetScreenHeight() / 4, MeasureText(WIN_TEXT, 40), 40, BLACK);
		DrawText(LOSE_TEXT, GetScreenWidth() / 2 - MeasureText(LOSE_TEXT, 40) / 2, GetScreenHeight() / 4, 40, WHITE);
	}
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
	(void)memset(enemies, 0, sizeof(enemies));

	// we done
	if (encounters[encounterndx + 1] == NULL) {
		gamewon = true;
		return;
	}

	// TODO: Can you shoot an enemy who isn't showing up yet? Sort of, they are just not moving
	score_increase_multiplier();
	bullet_init(1);
	bullet_track_hittable_player(&player);
	encounterndx++;
	for (enc = encounters[encounterndx]; enc->definition != NULL; enc++) {
		enemies[enemy_count].shoot = enc->definition->shoot;
		enemies[enemy_count].move = enc->definition->move;
		enemies[enemy_count].x = (int)(enc->x * (float)screen_width);
		enemies[enemy_count].y = (int)(enc->y * (float)screen_height);
		enemies[enemy_count].width = enc->definition->width;
		enemies[enemy_count].height = enc->definition->height;
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
	en->y += en->speed / 2;
	en->x += en->speed / 2;
}

void enemy_move_downsin(struct enemy_data_tag *en)
{
	en->y += en->speed;
	en->x += fast_fsinf(now);
}

void enemy_move_downcos(struct enemy_data_tag *en)
{
	en->y += en->speed;
	en->x += fast_fcosf(now);
}

void enemy_move_downstop(struct enemy_data_tag *en)
{
	if (en->_timestamp == 0.f) {
		en->_timestamp = now;
	}

	if (now - en->_timestamp < en->meta.downstop) {
		en->y += en->speed;
	}
}

void enemy_move_horzright(struct enemy_data_tag *en)
{
	en->x += en->speed;
}

void enemy_move_horzleft(struct enemy_data_tag *en)
{
	en->x -= en->speed;
}

void enemy_move_horzrightstop(struct enemy_data_tag *en)
{
	if (en->_timestamp == 0.f) {
		en->_timestamp = now;
	}

	if (now - en->_timestamp < en->meta.horzrightstop) {
		en->x += en->speed;
	}
}

void enemy_move_horzleftstop(struct enemy_data_tag *en)
{
	if (en->_timestamp == 0.f) {
		en->_timestamp = now;
	}

	if (now - en->_timestamp < en->meta.horzleftstop) {
		en->x -= en->speed;
	}
}
