#include <rlu/rlu.h>
#include "bullet.h"
#include "score.h"
#include "game.h"

#define ENEMY_HITTABLES 64
#define PLAYER_HITTABLES 4
#define BULLET_OFFSCREEN 0x3F3F
#define SIZE_STRAIGHT_WIDTH 20
#define SIZE_STRAIGHT_HEIGHT 20
#define VEL_STRAIGHT 500

#define SHOOT_FAST .15f
#define SHOOT_MED .33f
#define SHOOT_INT .4f
#define SHOOT_SLOW .5f

typedef void (* move_func)(bullet_data *bullet);

typedef struct timeout_lookup_tag {
	shoot_func move;
	float timeout;
} timeout_lookup;

typedef struct wrapper_tag {
	move_func move;
	HITTABLE_OBJECT **array;
	int arraylen;
	int ndx;
	int width;
	int height;
	bullet_data bullets[256];
	float timeout;
	shoot_func shoot;
	const char *name;
	Texture2D *tex;
} wrapper;

#define DEFINE_WRAPPER(SHOOT_FUNC, MOVE_FUNC, HITTABLES, WIDTH, HEIGHT, TIMEOUT) \
{ \
	.move=(move_func)(MOVE_FUNC), \
	.array=(HITTABLES), \
	.arraylen=ARRAY_SIZE(HITTABLES), \
	.ndx=0, \
	.width=(WIDTH), \
	.height=(HEIGHT), \
	.bullets={0}, \
	.timeout=(TIMEOUT), \
	.shoot=(SHOOT_FUNC), \
	.name=#SHOOT_FUNC, \
}

static void wrapper_init(wrapper *self);
static void wrapper_update(wrapper *self);
static void wrapper_draw(wrapper *self);

static void move_straight(bullet_data *bullet);
static void move_sin(bullet_data *bullet);
static void move_sin_wide(bullet_data *bullet);
static void move_cos(bullet_data *bullet);
static void move_cos_wide(bullet_data *bullet);
static void move_big(bullet_data *bullet);
static void move_beam(bullet_data *bullet);
static void move_parabola(bullet_data *bullet);

static int track_hittable(void *hittable, HITTABLE_OBJECT *array[], size_t size);
static void do_move_and_hit(HITTABLE_OBJECT *targets[], size_t targets_len, move_func move, bullet_data bullets[], size_t bullets_len, int bullet_width, int bullet_height);
static void do_draw(bullet_data bullets[], size_t bullets_len, Texture2D *tex);
static void insert_bullet(int x, int y, float direction, wrapper *wrap);

static HITTABLE_OBJECT *enemies[ENEMY_HITTABLES];
static HITTABLE_OBJECT *players[PLAYER_HITTABLES];

static wrapper wrap_enemy_straight = DEFINE_WRAPPER(bullet_enemy_straight, move_straight, players, 20, 20, SHOOT_INT);
static wrapper wrap_enemy_left = DEFINE_WRAPPER(bullet_enemy_left, move_straight, players, 20, 20, SHOOT_INT);
static wrapper wrap_enemy_right = DEFINE_WRAPPER(bullet_enemy_right, move_straight, players, 20, 20,SHOOT_INT);
static wrapper wrap_enemy_spin = DEFINE_WRAPPER(bullet_enemy_spin, move_straight, players, 20, 20, SHOOT_SLOW);
static wrapper wrap_enemy_flower = DEFINE_WRAPPER(bullet_enemy_flower, move_straight, players, 20, 20, SHOOT_INT);
static wrapper wrap_enemy_sin = DEFINE_WRAPPER(bullet_enemy_sin, move_sin, players, 20, 20, SHOOT_INT);
static wrapper wrap_enemy_sin_wide = DEFINE_WRAPPER(bullet_enemy_sin_wide, move_sin_wide, players, 20, 20, SHOOT_INT);
static wrapper wrap_enemy_cos = DEFINE_WRAPPER(bullet_enemy_sin, move_cos, players, 20, 20, SHOOT_INT);
static wrapper wrap_enemy_cos_wide = DEFINE_WRAPPER(bullet_enemy_sin_wide, move_cos_wide, players, 20, 20, SHOOT_INT);
static wrapper wrap_enemy_big = DEFINE_WRAPPER(bullet_enemy_big, move_big, players, 20, 20, SHOOT_SLOW);
static wrapper wrap_enemy_beam = DEFINE_WRAPPER(bullet_enemy_beam, move_beam, players, 20, 20, SHOOT_FAST);
static wrapper wrap_player_straight = DEFINE_WRAPPER(bullet_player_straight, move_straight, enemies, 20, 20, SHOOT_FAST);
static wrapper wrap_player_left = DEFINE_WRAPPER(bullet_player_left, move_straight, enemies, 20, 20, SHOOT_FAST);
static wrapper wrap_player_right = DEFINE_WRAPPER(bullet_player_right, move_straight, enemies, 20, 20, SHOOT_FAST);
static wrapper wrap_player_spin = DEFINE_WRAPPER(bullet_player_spin, move_straight, enemies, 20, 20, SHOOT_FAST);
static wrapper wrap_player_flower = DEFINE_WRAPPER(bullet_player_flower, move_straight, enemies, 20, 20, SHOOT_FAST);
static wrapper wrap_player_sin = DEFINE_WRAPPER(bullet_player_sin, move_sin, enemies, 20, 20, SHOOT_FAST);
static wrapper wrap_player_sin_wide = DEFINE_WRAPPER(bullet_player_sin_wide, move_sin_wide, enemies, 20, 20, SHOOT_FAST);
static wrapper wrap_player_cos = DEFINE_WRAPPER(bullet_player_sin, move_cos, enemies, 20, 20, SHOOT_FAST);
static wrapper wrap_player_cos_wide = DEFINE_WRAPPER(bullet_player_sin_wide, move_cos_wide, enemies, 20, 20, SHOOT_FAST);
static wrapper wrap_player_big = DEFINE_WRAPPER(bullet_player_big, move_big, enemies, 20, 20, SHOOT_FAST);
static wrapper wrap_player_beam = DEFINE_WRAPPER(bullet_player_beam, move_beam, enemies, 20, 20, SHOOT_FAST);
static wrapper wrap_player_parabola = DEFINE_WRAPPER(bullet_player_parabola, move_parabola, enemies, 20, 20, SHOOT_FAST);

static wrapper *wrappers[] = {
	&wrap_enemy_straight,
	&wrap_enemy_left,
	&wrap_enemy_right,
	&wrap_enemy_spin,
	&wrap_enemy_flower,
	&wrap_player_straight,
	&wrap_player_left,
	&wrap_player_right,
	&wrap_player_spin,
	&wrap_player_flower,
	&wrap_enemy_sin,
	&wrap_enemy_sin_wide,
	&wrap_enemy_cos,
	&wrap_enemy_cos_wide,
	&wrap_player_sin,
	&wrap_player_sin_wide,
	&wrap_player_cos,
	&wrap_player_cos_wide,
	&wrap_enemy_big,
	&wrap_player_big,
	&wrap_enemy_beam,
	&wrap_player_beam,
	&wrap_player_parabola,
	NULL
};

static double now;
static float frametime;
static int players_max; // every index before this is a player in hittables
static int screen_height;
static int screen_width;
static texture_manager texman;

static void wrapper_init(wrapper *self)
{
	Color bullet_color = YELLOW;
	char buf[128];
	assert(self);
	// each byte==1, will put it offscreen somewhere
	(void)memset(self->bullets, BULLET_OFFSCREEN, sizeof(self->bullets));

	(void)snprintf(buf, sizeof(buf), "%s/%s.png", context_get_assetdir(), self->name);
	if (self->array == players) {
		bullet_color = YELLOW;
	}
	else {
		bullet_color = GREEN;
	}
	self->tex = texture_man_load_or_default(&texman, buf, TEXTURE_GEN(self->width, self->height, bullet_color));
}

static void wrapper_update(wrapper *self)
{
	assert(self);
	do_move_and_hit(self->array, self->arraylen, self->move, self->bullets, ARRAY_SIZE(self->bullets), self->width, self->height);
}

static void wrapper_draw(wrapper *self)
{
	assert(self);
	do_draw(self->bullets, ARRAY_SIZE(self->bullets), self->tex);
}

void bullet_init(int nplayers)
{
	int i;
	assert(nplayers < PLAYER_HITTABLES);
	players_max = nplayers;
	memset(enemies, 0, sizeof(enemies));
	memset(players, 0, sizeof(players));

	if (nplayers > 0) {
		texture_man_new(&texman);
		for (i = 0; wrappers[i] != NULL; i++) {
			wrapper_init(wrappers[i]);
		}
	}
}

void bullet_cleanup(void)
{
	texture_man_del(&texman);
}

void bullet_clear(void)
{
	int i;
	for (i = 0; wrappers[i] != NULL; i++) {
		(void)memset(wrappers[i]->bullets, BULLET_OFFSCREEN, sizeof(wrappers[i]->bullets));
	}
}

void bullet_update(void)
{
	int i;

	now = GetTime();
	frametime = GetFrameTime();
	screen_height = GetScreenHeight();
	screen_width = GetScreenWidth();

	for (i = 0; wrappers[i] != NULL; i++) {
		wrapper_update(wrappers[i]);
	}
}

static void do_move_and_hit(HITTABLE_OBJECT *targets[], size_t targets_len, move_func move, bullet_data bullets[], size_t bullets_len, int bullet_width, int bullet_height)
{
	Rectangle bullet_rec;
	Rectangle entity_rec;
	int i, j, tmp;
	assert(targets);
	assert(bullets);

	for (i = 0; i < bullets_len; i++) {
		// offscreen
		if (bullets[i].y > screen_height || bullets[i].y <= -bullet_height) {
			bullets[i].y = BULLET_OFFSCREEN;
			continue;
		}

		move(&bullets[i]);
		bullet_rec = (Rectangle){
			bullets[i].x - bullet_width / 2, bullets[i].y - bullet_height / 2, bullet_width, bullet_height
		};

		// hit? hp can be <= 0, but hittable won't matter anyway at that point
		for (j = 0; j < targets_len; j++) {
			if (targets[j] == NULL) {
				continue;
			}

			entity_rec = (Rectangle){
				targets[j]->x - targets[j]->width / 2,
				targets[j]->y - targets[j]->height / 2,
				targets[j]->width,
				targets[j]->height
			};
			tmp = CheckCollisionRecs(bullet_rec, entity_rec);

			// consume the bullet if hit
			if (tmp) {
				if (targets[j]->hp > 0) {
					targets[j]->hp -= tmp;

					if (targets == players) {
						// enemy did damage!
						player_took_damage(targets[j]);
					}
					else {
						// player did damage!
						enemy_took_damage(targets[j]);
						if (targets[j]->hp <= 0) {
							enemy_took_death(targets[j]);
						}
					}
				}
				bullets[i].y = BULLET_OFFSCREEN;
				break;
			}
		}
	}
}

void bullet_draw(void)
{
	int i;
	for (i = 0; wrappers[i] != NULL; i++) {
		wrapper_draw(wrappers[i]);
	}
}

static void do_draw(bullet_data bullets[], size_t bullets_len, Texture2D *tex)
{
	int i;
	assert(bullets);
	for (i = 0; i < bullets_len; i++) {
		if (bullets[i].y > screen_height || bullets[i].y < -tex->height) {
			continue;
		}
		texture_man_draw_tex(tex, bullets[i].x - tex->width / 2, bullets[i].y - tex->height / 2);
	}
}

float bullet_lookup_timeout(shoot_func bullet_func)
{
	int i;
	assert(bullet_func);
	for (i = 0; wrappers[i] != NULL; i++) {
		if (bullet_func == wrappers[i]->shoot) {
			return wrappers[i]->timeout;
		}
	}
	msg_assert(0, "Could not look up a bullet fire timeout");
	return 0.f;
}

shoot_func bullet_lookup_shoot(char *name)
{
	int i;
	assert(name);
	for (i = 0; wrappers[i] != NULL; i++) {
		if (strcmp(name, wrappers[i]->name) == 0) {
			return wrappers[i]->shoot;
		}
	}
	return NULL;
}

void bullet_track_hittable_enemy(void *hittable)
{
	int rv = track_hittable(hittable, enemies, ARRAY_SIZE(enemies));
	msg_assert(rv == 0, "Not enough room for enemy hittables");
}

void bullet_track_hittable_player(void *hittable)
{
	int rv = track_hittable(hittable, players, ARRAY_SIZE(players));
	msg_assert(rv == 0, "Not enough room for player hittables");
}

// return 1 on failure, 0 on success
static int track_hittable(void *hittable, HITTABLE_OBJECT *array[], size_t size)
{
	int i;
	assert(hittable);
	assert(array);
	for (i = 0; i < size; i++) {
		if (array[i] == NULL) {
			array[i] = hittable;
			return 0;
		}
	}
	return 1;
}

///////////////////////////////////////////////////////////////////////////////

static void move_straight(bullet_data *bullet)
{
	assert(bullet);
	bullet->x += frametime * VEL_STRAIGHT * fast_fcosf(bullet->direction);
	bullet->y += frametime * VEL_STRAIGHT * -fast_fsinf(bullet->direction);
}

static void move_sin(bullet_data *bullet)
{
	assert(bullet);
	bullet->x += frametime * VEL_STRAIGHT * fast_fsinf((now - bullet->time) * 10);
	bullet->y += frametime * VEL_STRAIGHT * -fast_fsinf(bullet->direction);
}

static void move_sin_wide(bullet_data *bullet)
{
	assert(bullet);
	bullet->x += frametime * 2 * VEL_STRAIGHT * fast_fsinf((now - bullet->time) * 10);
	bullet->y += frametime * 2 * VEL_STRAIGHT * -fast_fsinf(bullet->direction);
}

static void move_cos(bullet_data *bullet)
{
	assert(bullet);
	bullet->x += frametime * VEL_STRAIGHT * fast_fcosf((now - bullet->time) * 10 + PI / 2.0);
	bullet->y += frametime * VEL_STRAIGHT * -fast_fsinf(bullet->direction);
}

static void move_cos_wide(bullet_data *bullet)
{
	assert(bullet);
	bullet->x += frametime * 2 * VEL_STRAIGHT * fast_fcosf((now - bullet->time) * 10 + PI / 2.0);
	bullet->y += frametime * 2 * VEL_STRAIGHT * -fast_fsinf(bullet->direction);
}

static void move_big(bullet_data *bullet)
{
	assert(bullet);
	bullet->x += frametime * 0.33 * VEL_STRAIGHT * fast_fcosf(bullet->direction);
	bullet->y += frametime * 0.33 * VEL_STRAIGHT * -fast_fsinf(bullet->direction);
}

static void move_beam(bullet_data *bullet)
{
	assert(bullet);
	bullet->x += frametime * 5 * VEL_STRAIGHT * fast_fcosf(bullet->direction);
	bullet->y += frametime * 5 * VEL_STRAIGHT * -fast_fsinf(bullet->direction);
}

static void move_parabola(bullet_data *bullet)
{
	const float a = 5;
	const float b = -5;
	assert(bullet);

	/**
	 * The parabola is like:
	 * 0     /
	 *  \   /
	 *   \_/
	 *
	 * We start going down then go up.
	 */

	// player
	if (bullet->direction == 1.f) {
		// right
		const float t = now - bullet->time;
		const float m = 2.f * a * t + b; // derivative of parabola: ax^2 + bx + c
		const float deltax = 1.5f; // trust me, this is correct for this.
		const float deltay = m;
		bullet->x += 100.f * deltax * frametime;
		bullet->y -= 100.f * deltay * frametime;
	}
	else if (bullet->direction == 2.f) {
		// left
		const float t = now - bullet->time;
		const float m = 2.f * a * t + b; // derivative of parabola: ax^2 + bx + c
		const float deltax = -1.f; // this one too
		const float deltay = m;
		bullet->x += 100.f * deltax * frametime;
		bullet->y -= 100.f * deltay * frametime;
	}
	// enemy
	else if (bullet->direction == -1.f) {
		// right
		msg_assert(0, "TODO");
	}
	else if (bullet->direction == -2.f) {
		// left
		msg_assert(0, "TODO");
	}
}

///////////////////////////////////////////////////////////////////////////////

static void insert_bullet(int x, int y, float direction, wrapper *wrap)
{
	assert(wrap);

	wrap->bullets[wrap->ndx] = (bullet_data){
		.x=x,
		.y=y,
		.direction=direction,
		.time=now,
	};
	wrap->ndx = (wrap->ndx + 1) % ARRAY_SIZE(wrap->bullets);
}

///////////////////////////////////////////////////////////////////////////////

void bullet_enemy_straight(int x, int y, int level)
{
	insert_bullet(x, y + SIZE_STRAIGHT_HEIGHT, PI * 3.f / 2.f, &wrap_enemy_straight);
}

void bullet_enemy_left(int x, int y, int level)
{
	insert_bullet(x - SIZE_STRAIGHT_WIDTH, y, PI, &wrap_enemy_left);
}

void bullet_enemy_right(int x, int y, int level)
{
	insert_bullet(x + SIZE_STRAIGHT_WIDTH, y, 0, &wrap_enemy_right);
}

void bullet_enemy_spin(int x, int y, int level)
{
	float angle;
	switch (level) {
	case 1:
		angle = fmod(now, PI);
	default:
	case 0:
		angle = fmod(now, 2 * PI);
	}

	insert_bullet(x, y, angle, &wrap_enemy_spin);
}

void bullet_enemy_flower(int x, int y, int level)
{
	switch (level) {
	case 2:
		insert_bullet(x, y, 0, &wrap_enemy_flower);
		insert_bullet(x, y, PI / 2.f, &wrap_enemy_flower);
		insert_bullet(x, y, PI, &wrap_enemy_flower);
		insert_bullet(x, y, PI * 3.f / 2.f, &wrap_enemy_flower);
		// fallthrough
	case 1: // X
		insert_bullet(x, y, 0, &wrap_enemy_flower);
		insert_bullet(x, y, PI / 2.f, &wrap_enemy_flower);
		insert_bullet(x, y, PI, &wrap_enemy_flower);
		insert_bullet(x, y, PI * 3.f / 2.f, &wrap_enemy_flower);
		break;
	default: // fallthrough
	case 0: // +
		insert_bullet(x, y, 0, &wrap_enemy_flower);
		insert_bullet(x, y, PI / 2.f, &wrap_enemy_flower);
		insert_bullet(x, y, PI, &wrap_enemy_flower);
		insert_bullet(x, y, PI * 3.f / 2.f, &wrap_enemy_flower);
		break;
	}
}

// straight bullets, shot by player
void bullet_player_straight(int x, int y, int level)
{
	switch (level) {
	case 2:
		insert_bullet(x, y - SIZE_STRAIGHT_HEIGHT, PI / 2.f, &wrap_player_straight);
	case 1: // fallthrough
		insert_bullet(x, y - SIZE_STRAIGHT_HEIGHT, PI / 3.f, &wrap_player_straight);
		insert_bullet(x, y - SIZE_STRAIGHT_HEIGHT, PI * 2.f / 3.f, &wrap_player_straight);
		break;
	default: // fallthrough
	case 0:
		insert_bullet(x, y - SIZE_STRAIGHT_HEIGHT, PI / 2.f, &wrap_player_straight);
	}
}

void bullet_player_left(int x, int y, int level)
{
	insert_bullet(x - SIZE_STRAIGHT_WIDTH, y, PI, &wrap_player_left);
}

void bullet_player_right(int x, int y, int level)
{
	insert_bullet(x + SIZE_STRAIGHT_WIDTH, y, 0, &wrap_player_right);
}

void bullet_player_spin(int x, int y, int level)
{
	float angle = fmod(now, 2 * PI);

	insert_bullet(x, y, angle, &wrap_player_spin);
}

void bullet_player_flower(int x, int y, int level)
{
	switch (level) {
	case 2:
		insert_bullet(x, y, 0, &wrap_player_flower);
		insert_bullet(x, y, PI / 2.f, &wrap_player_flower);
		insert_bullet(x, y, PI, &wrap_player_flower);
		insert_bullet(x, y, PI * 3.f / 2.f, &wrap_player_flower);
		// fallthrough
	case 1: // X
		insert_bullet(x, y, 0, &wrap_player_flower);
		insert_bullet(x, y, PI / 2.f, &wrap_player_flower);
		insert_bullet(x, y, PI, &wrap_player_flower);
		insert_bullet(x, y, PI * 3.f / 2.f, &wrap_player_flower);
		break;
	default: // fallthrough
	case 0: // +
		insert_bullet(x, y, 0, &wrap_player_flower);
		insert_bullet(x, y, PI / 2.f, &wrap_player_flower);
		insert_bullet(x, y, PI, &wrap_player_flower);
		insert_bullet(x, y, PI * 3.f / 2.f, &wrap_player_flower);
		break;
	}
}

// sine / cosine bullets, shot by enemy
void bullet_enemy_sin(int x, int y, int level)
{
	switch (level) {
	case 2:
		insert_bullet(x, y - SIZE_STRAIGHT_HEIGHT, PI / 2.f, &wrap_enemy_straight);
	case 1: // fallthrough
		insert_bullet(x, y - SIZE_STRAIGHT_HEIGHT, PI / 2.f, &wrap_enemy_cos);
	default: // fallthrough
	case 0: // fallthrough
		insert_bullet(x, y - SIZE_STRAIGHT_HEIGHT, PI / 2.f, &wrap_enemy_sin);
	}
}

void bullet_enemy_sin_wide(int x, int y, int level)
{
	switch (level) {
	case 2:
		insert_bullet(x, y - SIZE_STRAIGHT_HEIGHT, PI / 2.f, &wrap_enemy_straight);
	case 1: // fallthrough
		insert_bullet(x, y - SIZE_STRAIGHT_HEIGHT, PI / 2.f, &wrap_enemy_cos_wide);
	default: // fallthrough
	case 0: // fallthrough
		insert_bullet(x, y - SIZE_STRAIGHT_HEIGHT, PI / 2.f, &wrap_enemy_sin_wide);
	}
}

// sine / cosine bullets, shot by player
void bullet_player_sin(int x, int y, int level)
{
	switch (level) {
	case 2:
		insert_bullet(x, y - SIZE_STRAIGHT_HEIGHT, PI / 2.f, &wrap_player_straight);
	case 1: // fallthrough
		insert_bullet(x, y - SIZE_STRAIGHT_HEIGHT, PI / 2.f, &wrap_player_cos);
	default: // fallthrough
	case 0: // fallthrough
		insert_bullet(x, y - SIZE_STRAIGHT_HEIGHT, PI / 2.f, &wrap_player_sin);
	}
}

void bullet_player_sin_wide(int x, int y, int level)
{
	switch (level) {
	case 2:
		insert_bullet(x, y - SIZE_STRAIGHT_HEIGHT, PI / 2.f, &wrap_player_straight);
	case 1: // fallthrough
		insert_bullet(x, y - SIZE_STRAIGHT_HEIGHT, PI / 2.f, &wrap_player_cos_wide);
	default: // fallthrough
	case 0: // fallthrough
		insert_bullet(x, y - SIZE_STRAIGHT_HEIGHT, PI / 2.f, &wrap_player_sin_wide);
	}
}

// big bullets, straight
void bullet_enemy_big(int x, int y, int level)
{
	insert_bullet(x, y - SIZE_STRAIGHT_HEIGHT, PI * 3.f / 2.f, &wrap_enemy_big);
}

void bullet_player_big(int x, int y, int level)
{
	insert_bullet(x, y + SIZE_STRAIGHT_HEIGHT, PI / 2.f, &wrap_player_big);
}

// beam, (fast widish, straight)
void bullet_enemy_beam(int x, int y, int level)
{
	insert_bullet(x, y - SIZE_STRAIGHT_HEIGHT, PI * 3.f / 2.f, &wrap_enemy_beam);
}

void bullet_player_beam(int x, int y, int level)
{
	insert_bullet(x, y + SIZE_STRAIGHT_HEIGHT, PI / 2.f, &wrap_player_beam);
}

void bullet_player_parabola(int x, int y, int level)
{
	insert_bullet(x, y + SIZE_STRAIGHT_HEIGHT, 1.f, &wrap_player_parabola);
	insert_bullet(x, y + SIZE_STRAIGHT_HEIGHT, 2.f, &wrap_player_parabola);
}
