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

static void player_new(player_data *self);
static void player_del(player_data *self);
static void enemy_new(enemy_data *self, encounter *enc);
static void enemy_del(enemy_data *self);

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

static player_data player;
static texture_manager texman;
static anim_man *animan;

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
	gamewon = false;
	gamelost = false;

	texture_man_new(&texman);
	animan = anim_man_new();
	player_new(&player);

	//encounters = map_init(NULL);
	encounters = map_init("maps/test.mg");
	score_init();
	atmos_init("assets");

	icon = texture_man_img_load_or_default("assets/icon.png", 30, 30, BLUE);
	SetWindowIcon(icon);
}

void game_cleanup(void)
{
	int i;
	player_del(&player);
	for (i = 0; i < enemy_count; i++) {
		enemy_del(&enemies[i]);
	}
	anim_man_del(animan);
	texture_man_del(&texman);

	encounter_clear();
	map_cleanup();
	atmos_cleanup();
	UnloadImage(icon);
}

void game_update(void)
{
	int i;
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
		if ((player.x + player.width <= screen_width)) {
			player.x += (GetFrameTime() * PLAYER_SPEED * dir);
		}
		else {
			player.x = screen_width - player.width;
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
		if ((player.y + player.height <= screen_height)) {
			player.y += (GetFrameTime() * PLAYER_SPEED * dir);
		}
		else {
			player.y = screen_height - player.height;
		}
	}

	if (player.y + player.height > screen_height) {
		player.y = screen_height - player.height / 2;
	}
	else if (player.y < player.height / 2) {
		player.y = player.height / 2;
	}
	if (player.x + player.width > screen_width) {
		player.x = screen_width - player.width / 2;
	}
	else if (player.x < player.width / 2) {
		player.x = player.width / 2;
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
			//DrawRectangle(enemies[i].x - ENEMY_SIZE / 2, enemies[i].y - ENEMY_SIZE / 2, ENEMY_SIZE, ENEMY_SIZE, RED);
			so_set_pos(enemies[i].object, enemies[i].x - enemies[i].width / 2, enemies[i].y - enemies[i].height / 2);
			so_draw(enemies[i].object);
		}
	}

	if (gamelost == false) {
		so_set_pos(player.object, player.x - player.width / 2, player.y - player.height / 2);
		//DrawRectangle(player.x - PLAYER_SIZE / 2, player.y - PLAYER_SIZE / 2, PLAYER_SIZE, PLAYER_SIZE, BLUE);
		so_draw(player.object);
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
	bullet_init(1);
	score_increase_multiplier();
	bullet_track_hittable_player(&player);
	encounterndx++;
	for (enc = encounters[encounterndx]; enc->definition != NULL; enc++) {
		if (enemies[enemy_count].object) {
			enemy_del(&enemies[enemy_count]);
		}
		enemy_new(&enemies[enemy_count], enc);
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

static void player_new(player_data *self)
{
	Texture2D *tex;
	assert(self);
	self->x = screen_width / 2;
	self->y = screen_height * 3 / 4;
	self->shoot = bullet_player_sin_wide;
	self->shotperiod = bullet_lookup_timeout(self->shoot);
	self->hp = self->lasthp = 30;
	self->level = 2;
	tex = texture_man_load_or_default(&texman, "assets/default_player.png", TEXTURE_GEN(self->width, self->height, BLUE));
	self->width = tex->width;
	self->height = tex->height;
	self->object = so_new(
		anim_man_load(
			animan,
			tex,
			1,
			1
		)
	);
}

static void player_del(player_data *self)
{
	assert(self);
	if (self->object) {
		so_del(self->object);
		self->object = NULL;
	}
}

static void enemy_new(enemy_data *self, encounter *enc)
{
	Texture2D *tex;
	assert(self);
	assert(enc);

	self->shoot = enc->definition->shoot;
	self->move = enc->definition->move;
	self->x = (int)(enc->x * (float)screen_width);
	self->y = (int)(enc->y * (float)screen_width);
	self->width = enc->definition->width;
	self->height = enc->definition->height;
	self->hp = enc->definition->hp;
	self->level = enc->definition->level;
	memcpy(&self->meta, &enc->definition->meta, sizeof(self->meta));
	self->speed = enc->definition->speed;
	self->shotperiod = bullet_lookup_timeout(enc->definition->shoot);
	self->spawntime = enc->spawntime;

	tex = texture_man_load_or_default(&texman, enc->definition->pngname, TEXTURE_GEN(self->width, self->height, RED));
	self->width = tex->width;
	self->height = tex->height;

	assert(enc->definition->pngname);
	self->object = so_new(
		anim_man_load(
			animan,
			tex,
			1,
			1
		)
	);
}

static void enemy_del(enemy_data *self)
{
	assert(self);
	if (self->object) {
		so_del(self->object);
		self->object = NULL;
	}
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
