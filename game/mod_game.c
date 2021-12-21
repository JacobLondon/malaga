#include <rlu/rlu.h>
#include "modules.h"
#include "data.h"
#include "map.h"
#include "score.h"
#include "atmos.h"
#include "drop.h"
#include "drop_manager.h"

#define PLAYER_DEFAULT_SPEED 650
#define PLAYER_SIZE 25
#define ENEMY_SIZE 35
#define ENEMIES_MAX 32

#define ENCOUNTER_DEFAULT -1
#define PLAYER_DAMAGE_DEFAULT 1
#define TROUBLE_MODE_MULT_DEFAULT 2

static void encounter_clear(void);
static void encounter_next(void);
static bool encounter_done(void);

static void player_new(player_data *self);
static void player_del(player_data *self);
static void player_eat_drop(player_data *self, item_drop drop);
static void enemy_new(enemy_data *self, encounter *enc);
static void enemy_del(enemy_data *self);
static void detonations_cleanup(void);
static void detonations_update(void);
static void detonations_init(void);
static void detonation_cb1(ko *self, so *object);
static void detonation_cb2(ko *self, so *object);
static void enemy_detonate(enemy_data *self);

static size_t roundno = 0;
static struct game_message game_data;
static int encounterndx = ENCOUNTER_DEFAULT;
static double encounter_starttime = 0.0;
static int enemy_count = 0; // number of loaded enemies
static int screen_height = 0;
static int screen_width = 0;
static double now = 0.0;
static bool gamewon = false;
static bool gamelost = false;
static int player_damage_mult = 1;

static enemy_data enemies[ENEMIES_MAX];
static bool detonation_keys[ENEMIES_MAX];
static ko *detonation_objects[ENEMIES_MAX];
static bool use_detonations = true;

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
static encounter **encounters;

void game_init(void)
{
	struct MapHeader header;

	encounter_clear();
	screen_height = GetScreenHeight();
	screen_width = GetScreenWidth();
	now = GetTime();
	encounterndx = ENCOUNTER_DEFAULT;
	gamewon = false;
	gamelost = false;

	if (game_data.trouble_mode) {
		player_damage_mult = TROUBLE_MODE_MULT_DEFAULT;
	}
	else {
		player_damage_mult = 1;
	}

	roundno = 0;

	texture_man_new(&texman);
	animan = anim_man_new();
	player_new(&player);
	detonations_init();
	drop_manager_init();

	if (game_data.mapdir[0] == 0) {
		char file[256];
		snprintf(file, sizeof(file), "%s/%s", DATA_MAPS_DIR, DATA_ASSET_MAP);
		encounters = map_init(file, &header);
	}
	else {
		encounters = map_init(game_data.mapdir, &header);
	}

	score_init();
	atmos_init(header.atmosphere);
	// TODO: header.music...

	if (game_data.endless_mode) {
		msg_default("%s ACTIVATED", "ENDLESS MODE");
	}
	if (game_data.trouble_mode) {
		msg_default("%s ACTIVATED", "2x TROUBLE MODE");
	}
	if (game_data.god_mode) {
		msg_default("%s ACTIVATED", "GOD MODE");
	}
}

void game_cleanup(void)
{
	int i;
	drop_manager_cleanup();
	detonations_cleanup();
	player_del(&player);
	for (i = 0; i < enemy_count; i++) {
		enemy_del(&enemies[i]);
	}
	anim_man_del(animan);
	texture_man_del(&texman);

	encounter_clear();
	map_cleanup();
	atmos_cleanup();
}

void game_update(void)
{
	size_t i;
	float dir;

	screen_width = GetScreenWidth();
	screen_height = GetScreenHeight();
	now = GetTime();

	anim_man_update(animan);

	// move player
	if (!gamelost) {
		rlu_input_prescan(0);
		dir = rlu_input_axis(0, RLU_KEY_STICK_LEFT_X);
		if (dir < 0) {
			if ((player.x >= player.width / 2)) {
				player.x += (GetFrameTime() * player.speed * dir);
				if (player.x < player.width / 2) {
					player.x = player.width / 2;
				}
			}
			else {
				player.x = player.width / 2;
			}
		}
		else if (dir > 0)
		{
			if (player.x + player.width / 2 <= screen_width) {
				player.x += (GetFrameTime() * player.speed * dir);
				if (player.x + player.width / 2 > screen_width) {
					player.x = screen_width - player.width / 2;
				}
			}
			else {
				player.x = screen_width - player.width / 2;
			}
		}

		dir = rlu_input_axis(0, RLU_KEY_STICK_LEFT_Y);
		if (dir < 0) {
			if ((player.y >= player.height / 2)) {
				player.y += (GetFrameTime() * player.speed * dir);
				if (player.y < player.height / 2) {
					player.y = player.height / 2;
				}
			}
			else {
				player.y = player.height / 2;
			}
		}
		else if (dir > 0)
		{
			if (player.y + player.height / 2 <= screen_height) {
				player.y += (GetFrameTime() * player.speed * dir);
				if (player.y + player.height / 2 > screen_height) {
					player.y = screen_height - player.height / 2;
				}
			}
			else {
				player.y = screen_height - player.height / 2;
			}
		}

		if (player.x + player.width > screen_width + player.width / 2) {
			player.x = screen_width - player.width / 2;
		}
		else if (player.x < player.width / 2) {
			player.x = player.width / 2;
		}
	}

	if (IsKeyPressed(KEY_R)) {
		context_switch("GAME");
	}
	if (IsKeyPressed(KEY_ENTER)) {
		context_push("PAUSE");
	}

	if      (IsKeyPressed(KEY_ONE))   player_eat_drop(&player, drop_bullet(BULLET_PLAYER_STRAIGHT));
	else if (IsKeyPressed(KEY_TWO))   player_eat_drop(&player, drop_bullet(BULLET_PLAYER_LEFT));
	else if (IsKeyPressed(KEY_THREE)) player_eat_drop(&player, drop_bullet(BULLET_PLAYER_RIGHT));
	else if (IsKeyPressed(KEY_FOUR))  player_eat_drop(&player, drop_bullet(BULLET_PLAYER_SPIN));
	else if (IsKeyPressed(KEY_FIVE))  player_eat_drop(&player, drop_bullet(BULLET_PLAYER_FLOWER));
	else if (IsKeyPressed(KEY_SIX))   player_eat_drop(&player, drop_bullet(BULLET_PLAYER_SIN));
	else if (IsKeyPressed(KEY_SEVEN)) player_eat_drop(&player, drop_bullet(BULLET_PLAYER_SIN_WIDE));
	else if (IsKeyPressed(KEY_EIGHT)) player_eat_drop(&player, drop_bullet(BULLET_PLAYER_COS));
	else if (IsKeyPressed(KEY_NINE))  player_eat_drop(&player, drop_bullet(BULLET_PLAYER_COS_WIDE));
	else if (IsKeyPressed(KEY_ZERO))  player_eat_drop(&player, drop_bullet(BULLET_PLAYER_BIG));
	else if (IsKeyPressed(KEY_MINUS)) player_eat_drop(&player, drop_bullet(BULLET_PLAYER_BEAM));
	else if (IsKeyPressed(KEY_EQUAL)) player_eat_drop(&player, drop_bullet(BULLET_PLAYER_PARABOLA));

	if (IsKeyPressed(KEY_PERIOD)) {
		player.level += 1;
	}
	else if (IsKeyPressed(KEY_COMMA)) {
		if (player.level - 1 >= 0) {
			player.level -= 1;
		}
	}

	// move it after the player tries to eat the last frame instance
	player_eat_drop(&player, drop_manager_pickup(&player));
	drop_manager_update();

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
			(enemies[i].y > screen_height + 5 * enemies[i].height) ||
			(enemies[i].y < -5 * enemies[i].height) ||
			(enemies[i].x > screen_width + 5 * enemies[i].width) ||
			(enemies[i].x < -5 * enemies[i].width)
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
				player_took_damage(&player);
			}
		}
	}

	bullet_update();
	atmos_update();

	if (player.hp <= 0) {
		gamelost = true;
		player.y = 10000;
	}

	if (gamewon || gamelost) {
		struct hiscore_message msg = {
			.won = gamewon && !gamelost,
		};
		(void)snprintf(msg.scorefile, sizeof(msg.scorefile), "%s", game_data.mapdir);

		// replace .mg with .sc for the score file
		char *p = strstr(msg.scorefile, DATA_MAPS_EXT);
		if (p) {
			(void)snprintf(p, sizeof(DATA_SCORE_EXT), "%s", DATA_SCORE_EXT);
		}

		hiscore_conf(&msg);
		context_push("HISCORE");
	}
}

void game_draw(void)
{
	static char health[128];
	static char round[64];
	static char shoot[128];
	int i;

	//DrawFPS(20, 20);
	atmos_draw();
	bullet_draw();
	drop_manager_draw();
	detonations_update();
	detonations_update();

	for (i = 0; i < enemy_count; i++) {
		if (enemies[i].hp >= 0 && (now - encounter_starttime >= enemies[i].spawntime)) {
			so_set_pos(enemies[i].object, enemies[i].x - enemies[i].width / 2, enemies[i].y - enemies[i].height / 2);
			so_draw(enemies[i].object);
		}
	}

	if (gamelost == false) {
		so_set_pos(player.object, player.x - player.width / 2, player.y - player.height / 2);
		so_draw(player.object);
	}

	// top left
	const int FONTSIZE = 30;
	snprintf(health, sizeof(health), "HP %d", player.hp);
	DrawText(health, 5, 5, FONTSIZE, WHITE);

	// top right
	snprintf(round, sizeof(round), "Round %zu", roundno);
	DrawText(round, screen_width - 5 - MeasureText(round, FONTSIZE), 5, FONTSIZE, WHITE);
	DrawText(score_get_string(), screen_width - 5 - MeasureText(score_get_string(), FONTSIZE), 40, FONTSIZE, WHITE);
	DrawText(score_get_multiplier(), screen_width - 5 - MeasureText(score_get_multiplier(), FONTSIZE), 75, FONTSIZE, WHITE);

	// bottom right
	if (game_data.endless_mode) {
		DrawText("ENDLESS", screen_width - 5 - MeasureText("ENDLESS", FONTSIZE), screen_height - FONTSIZE, FONTSIZE, WHITE);
	}
	if (game_data.trouble_mode) {
		DrawText("2x TROUBLE", screen_width - 5 - MeasureText("2x TROUBLE", FONTSIZE), screen_height - 2 * FONTSIZE, FONTSIZE, WHITE);
	}
	if (game_data.god_mode) {
		DrawText("INVINCIBLE", screen_width - 5 - MeasureText("INVINCIBLE", FONTSIZE), screen_height - 3 * FONTSIZE, FONTSIZE, WHITE);
	}

	// bottom left
	snprintf(shoot, sizeof(shoot), "%s: %d", &player.shoot_name[sizeof("player_bullet")], player.level);
	DrawText(shoot, 5, screen_height - FONTSIZE - 5, FONTSIZE, WHITE);
}

void game_conf(struct game_message *msg)
{
	assert(msg);

	snprintf(game_data.mapdir, sizeof(game_data.mapdir), "%s", msg->mapdir);
	game_data.endless_mode = msg->endless_mode;
	game_data.trouble_mode = msg->trouble_mode;
	game_data.god_mode = msg->god_mode;
	game_data.playership = msg->playership;
}

static void encounter_clear(void)
{
	;
}

static void encounter_next(void)
{
	encounter *enc;

	// map wasn't loaded properly, so do nothing
	if (!encounters) {
		return;
	}

	encounter_starttime = now;
	enemy_count = 0;
	(void)memset(enemies, 0, sizeof(enemies));

	// we done
	if (encounters[encounterndx + 1] == NULL) {
		if (game_data.endless_mode) {
			encounterndx = ENCOUNTER_DEFAULT;
		}
		else {
			// clear bullets on win, remove those enemy bullets!
			if (gamewon == false) {
				bullet_clear();
			}
			gamewon = true;
			return;
		}
	}

	// TODO: Can you shoot an enemy who isn't showing up yet? Sort of, they are just not moving
	bullet_init(1);

	// no free lunch
	if (!game_data.god_mode) {
		score_increase_multiplier();
	}

	roundno++;
	bullet_track_hittable_player(&player);
	encounterndx++;
	for (enc = encounters[encounterndx];
	    enc->definition != NULL;
	    enc++)
	{
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

void player_took_damage(player_data *pd)
{
	assert(pd);

	if (!game_data.god_mode) {
		score_decrease_multiplier();
		pd->hp -= PLAYER_DAMAGE_DEFAULT * player_damage_mult;
	}
}

void enemy_took_damage(enemy_data *ed)
{
	assert(ed);
	score_increase_points();
	ed->hp -= 1;

	if (game_data.trouble_mode) {
		score_increase_points();
	}
}

void enemy_took_death(enemy_data *ed)
{
	assert(ed);
	score_increase_points();
	score_increase_points();

	if (game_data.trouble_mode) {
		score_increase_points();
		score_increase_points();
	}

	drop_manager_dropit(ed);
	enemy_detonate(ed);
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
	item_drop shooter_drop;
	char buf[256];
	Texture2D *tex;
	assert(self);

	//(void)snprintf(buf, sizeof(buf), "%s", game_data.playership);
	self->x = screen_width / 2;
	self->y = screen_height * 3 / 4;

	switch (game_data.playership) {
	default:
		game_data.playership = 0;
		// fallthrough
	case 0:
		shooter_drop = drop_bullet(BULLET_PLAYER_STRAIGHT);
		self->hp = 30;
		self->speed = PLAYER_DEFAULT_SPEED;
		break;
	case 1:
		shooter_drop = drop_bullet(BULLET_PLAYER_PARABOLA);
		self->hp = 40;
		self->speed = PLAYER_DEFAULT_SPEED * 1.2;
		break;
	case 2:
		shooter_drop = drop_bullet(BULLET_PLAYER_SIN);
		self->hp = 40;
		self->speed = PLAYER_DEFAULT_SPEED;
		break;
	case 3:
		shooter_drop = drop_bullet(BULLET_PLAYER_SPIN);
		self->hp = 25;
		self->speed = PLAYER_DEFAULT_SPEED * 1.1;
		break;
	case 4:
		shooter_drop = drop_bullet(BULLET_PLAYER_BIG);
		self->hp = 45;
		self->speed = PLAYER_DEFAULT_SPEED * 0.75;
		break;
	case 5:
		shooter_drop = drop_bullet(BULLET_PLAYER_BEAM);
		self->hp = 15;
		self->speed = PLAYER_DEFAULT_SPEED * 1.25;
		break;
	}

	self->level = 0;
	(void)snprintf(buf, sizeof(buf), "%s/%s%d.png", context_get_skindir(), "default_player", game_data.playership);
	self->shoot = bullet_lookup_shoot((char *)drop_to_string(&shooter_drop));
	(void)snprintf(self->shoot_name, sizeof(self->shoot_name), "%s", drop_to_string(&shooter_drop));
	self->shotperiod = bullet_lookup_timeout(self->shoot);

	tex = texture_man_load_or_default(&texman, buf, TEXTURE_GEN(PLAYER_SIZE, PLAYER_SIZE, BLUE));
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

static void player_eat_drop(player_data *self, item_drop drop)
{
	char *tmp;
	assert(self);
	switch (drop.type) {
	case DROP_NONE:
		break;
	case DROP_LEVEL:
		self->level += drop.spec.level_amount;
		break;
	case DROP_BULLET:
		tmp = (char *)drop_to_string(&drop);
		self->shoot = bullet_lookup_shoot(tmp);
		(void)snprintf(self->shoot_name, sizeof(self->shoot_name), "%s", tmp);
		self->shotperiod = bullet_lookup_timeout(self->shoot);
		break;
	case DROP_HEALTH:
		self->hp += drop.spec.health_amount;
		break;
	case DROP_SPEED:
		self->speed += drop.spec.speed_amount;
		break;
	default:
		msg_assert(0, "Invalid path");
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

static void detonations_cleanup(void)
{
	size_t i;
	if (!use_detonations) {
		return;
	}

	for (i = 0; i < ARRAY_SIZE(detonation_objects); i++) {
		ko_del(detonation_objects[i]);
	}
	memset(&detonation_keys, 0, sizeof(detonation_keys));
}

static void detonations_update(void)
{
	size_t i;
	if (!use_detonations) {
		return;
	}

	for (i = 0; i < ARRAY_SIZE(detonation_objects); i++) {
		ko_update(detonation_objects[i]);
	}
}

static void detonations_init(void)
{
	char path[128];
	Texture2D *explosion;
	so *s;
	ko *k;
	size_t i;

	snprintf(path, sizeof(path), "%s/%s", context_get_skindir(), "explosion.png");
	if (!FileExists(path)) {
		use_detonations = false;
		return;
	}

	memset(detonation_keys, 0, sizeof(detonation_keys));
	explosion = texture_man_load(&texman, path);
	for (i = 0; i < ARRAY_SIZE(detonation_objects); i++) {
		s = so_new_own(anim_new(explosion, 4, 4));
		// set off screen to start with
		so_set_pos(s, screen_width + 20000, 0);
		k = detonation_objects[i] = ko_new();
		ko_add(k, NULL, detonation_cb1, &detonation_keys[i]);
		ko_add_rate(k, s, detonation_cb2, NULL, 10);
	}
}

static void detonation_cb1(ko *self, so *object)
{
	// do nothing, ko_set_key is tied to detonation_keys
	(void)self;
	(void)object;
}

static void detonation_cb2(ko *self, so *object)
{
	(void)object;

	// animate explosion then move off screen
	if (ko_get_frame(self) >= ko_get_max_frames(self)) {
		ko_set_key(self, true);
		ko_set_pos(self, screen_width + 20000, 0);
	}
}

static void enemy_detonate(enemy_data *self)
{
	size_t i;
	assert(self);

	// get index of the enemy, relating to detonation keys
	if (use_detonations) {
		i = (self - enemies);
		ko_set_pos(detonation_objects[i], self->x, self->y);
		detonation_keys[i] = true;
	}
}
