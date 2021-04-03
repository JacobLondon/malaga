#include <rlu/rlu.h>
#include "bullet.h"

#define ENEMY_HITTABLES 64

#define PLAYER_HITTABLES 8
#define SIZE_STRAIGHT_WIDTH 50
#define SIZE_STRAIGHT_HEIGHT 50

typedef void (* move_func)(bullet_data *bullet);

static void move_straight(bullet_data *bullet);
static void move_sin(bullet_data *bullet);
static void move_big(bullet_data *bullet);
static void move_beam(bullet_data *bullet);

static int track_hittable(void *hittable, HITTABLE_OBJECT *array[], size_t size);
static int do_move_and_hit(HITTABLE_OBJECT *targets[], size_t targets_len, move_func move, bullet_data bullets[], size_t bullets_len, int bullet_width, int bullet_height);
static void do_draw(bullet_data bullets[], size_t bullets_len, int bullet_width, int bullet_height);
static void insert_bullet(int x, int y, float meta, bullet_data array[], size_t len, int *ndx);

static HITTABLE_OBJECT *enemies[ENEMY_HITTABLES];
static HITTABLE_OBJECT *players[PLAYER_HITTABLES];

static bullet_data bullets_enemy_straight[512];
static bullet_data bullets_enemy_sin[512];
static bullet_data bullets_enemy_sin_wide[64];
static bullet_data bullets_enemy_big[256];
static bullet_data bullets_enemy_beam[512];
static bullet_data bullets_player_straight[256];
static bullet_data bullets_player_sin[256];
static bullet_data bullets_player_sin_wide[32];
static bullet_data bullets_player_big[128];
static bullet_data bullets_player_beam[256];
static int ndx_enemy_straight = 0;
static int ndx_enemy_sin = 0;
static int ndx_enemy_sin_wide = 0;
static int ndx_enemy_big = 0;
static int ndx_enemy_beam = 0;
static int ndx_player_straight = 0;
static int ndx_player_sin = 0;
static int ndx_player_sin_widt = 0;
static int ndx_player_big = 0;
static int ndx_player_beam = 0;

static double now;
static float frametime;
static int players_max; // every index before this is a player in hittables
static int screen_height;
static int screen_width;

void bullet_init(int nplayers)
{
	assert(nplayers < PLAYER_HITTABLES);
	players_max = nplayers;
	memset(enemies, 0, sizeof(enemies));
	memset(players, 0, sizeof(players));
	memset(bullets_enemy_straight, 0, sizeof(bullets_enemy_straight));
	memset(bullets_enemy_sin, 0, sizeof(bullets_enemy_sin));
	memset(bullets_enemy_big, 0, sizeof(bullets_enemy_big));
	memset(bullets_enemy_beam, 0, sizeof(bullets_enemy_beam));
	memset(bullets_player_straight, 0, sizeof(bullets_player_straight));
	memset(bullets_player_sin, 0, sizeof(bullets_player_sin));
	memset(bullets_player_big, 0, sizeof(bullets_player_big));
	memset(bullets_player_beam, 0, sizeof(bullets_player_beam));
}

void bullet_cleanup(void)
{
	;
}

void bullet_update(void)
{
	int i, j;

	now = GetTime();
	frametime = GetFrameTime();
	screen_height = GetScreenHeight();
	screen_width = GetScreenWidth();

	do_move_and_hit(players, players_max, move_straight, bullets_enemy_straight, ARRAY_SIZE(bullets_enemy_straight), SIZE_STRAIGHT_WIDTH, SIZE_STRAIGHT_HEIGHT);
	do_move_and_hit(players, players_max, move_sin, bullets_enemy_sin, ARRAY_SIZE(bullets_enemy_sin), 50, 50);
	do_move_and_hit(players, players_max, move_big, bullets_enemy_big, ARRAY_SIZE(bullets_enemy_big), 50, 50);
	do_move_and_hit(players, players_max, move_beam, bullets_enemy_beam, ARRAY_SIZE(bullets_enemy_beam), 50, 50);

	do_move_and_hit(enemies, ARRAY_SIZE(enemies), move_straight, bullets_player_straight, ARRAY_SIZE(bullets_player_straight), SIZE_STRAIGHT_WIDTH, SIZE_STRAIGHT_HEIGHT);
	do_move_and_hit(enemies, ARRAY_SIZE(enemies), move_sin, bullets_player_sin, ARRAY_SIZE(bullets_player_sin), 50, 50);
	do_move_and_hit(enemies, ARRAY_SIZE(enemies), move_big, bullets_player_big, ARRAY_SIZE(bullets_player_big), 50, 50);
	do_move_and_hit(enemies, ARRAY_SIZE(enemies), move_beam, bullets_player_beam, ARRAY_SIZE(bullets_player_beam), 50, 50);
}

static int do_move_and_hit(HITTABLE_OBJECT *targets[], size_t targets_len, move_func move, bullet_data bullets[], size_t bullets_len, int bullet_width, int bullet_height)
{
	int i, j;
	assert(targets);
	assert(bullets);

	for (i = 0; i < bullets_len; i++) {
		// offscreen
		if (bullets[i].y > screen_height || bullets[i].y <= -bullet_height) {
			continue;
		}

		move(&bullets[i]);

		// hit? (try to get that SIMD performance, hp can be <= 0, but hittable won't matter anyway at that point)
		for (j = 0; j < targets_len; j++) {
			targets[j]->hp -=
			    ((targets[j]->x >= bullets[i].x - bullet_width / 2) &&
			     (targets[i]->x <= bullets[i].x + bullet_width / 2) &&
			     (targets[j]->y <= bullets[i].y + bullet_height / 2) &&
			     (targets[i]->y >= bullets[i].x - bullet_height / 2));
		}
	}
}

void bullet_draw(void)
{
	do_draw(bullets_enemy_straight, ARRAY_SIZE(bullets_enemy_straight), 50, 50);
	do_draw(bullets_enemy_sin, ARRAY_SIZE(bullets_enemy_sin), 50, 50);
	do_draw(bullets_enemy_sin_wide, ARRAY_SIZE(bullets_enemy_sin_wide), 50, 50);
	do_draw(bullets_enemy_big, ARRAY_SIZE(bullets_enemy_big), 50, 50);
	do_draw(bullets_enemy_beam, ARRAY_SIZE(bullets_enemy_beam), 50, 50);
	do_draw(bullets_player_straight, ARRAY_SIZE(bullets_player_straight), 50, 50);
	do_draw(bullets_player_sin, ARRAY_SIZE(bullets_player_sin), 50, 50);
	do_draw(bullets_player_sin_wide, ARRAY_SIZE(bullets_player_sin_wide), 50, 50);
	do_draw(bullets_player_big, ARRAY_SIZE(bullets_player_big), 50, 50);
	do_draw(bullets_player_beam, ARRAY_SIZE(bullets_player_beam), 50, 50);
}

static void do_draw(bullet_data bullets[], size_t bullets_len, int bullet_width, int bullet_height)
{
	int i;
	assert(bullets);
	for (i = 0; i < bullets_len; i++) {
		if (bullets[i].y > screen_height || bullets[i].y <= -bullet_height) {
			continue;
		}
		DrawRectangle(bullets[i].x, bullets[i].y, bullet_width, bullet_height, YELLOW);
	}
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

static void move_straight(bullet_data *bullet)
{
	assert(bullet);
	bullet->x += frametime * 100 * fast_fcosf(bullet->meta.direction);
	bullet->y += frametime * 100 * -fast_fsinf(bullet->meta.direction);
}

static void move_sin(bullet_data *bullet)
{
	assert(bullet);
}

static void move_big(bullet_data *bullet)
{
	assert(bullet);
}

static void move_beam(bullet_data *bullet)
{
	assert(bullet);
}

static void insert_bullet(int x, int y, float meta, bullet_data array[], size_t len, int *ndx)
{
	assert(array);
	assert(ndx);

	array[*ndx].x = x;
	array[*ndx].y = y;
	array[*ndx].meta.time = meta;
	*ndx = (*ndx + 1) % len;
}

///////////////////////////////////////////////////////////////////////////////

void bullet_enemy_straight(int x, int y)
{
	insert_bullet(x, y + SIZE_STRAIGHT_HEIGHT, PI * 3.f / 2.f, bullets_enemy_straight, ARRAY_SIZE(bullets_enemy_straight), &ndx_enemy_straight);
}

void bullet_enemy_straightdouble(int x, int y)
{

}

void bullet_enemy_straighttriple(int x, int y)
{

}

void bullet_enemy_spread(int x, int y)
{

}

void bullet_enemy_spreaddouble(int x, int y)
{

}

void bullet_enemy_spreadtriple(int x, int y)
{

}

void bullet_enemy_spreadquad(int x, int y)
{

}

void bullet_enemy_left(int x, int y)
{

}

void bullet_enemy_right(int x, int y)
{

}

void bullet_enemy_spin(int x, int y)
{

}

void bullet_enemy_flower(int x, int y)
{

}

// straight bullets, shot by player
void bullet_player_straight(int x, int y)
{
	insert_bullet(x, y - SIZE_STRAIGHT_HEIGHT, PI / 2.f, bullets_player_straight, ARRAY_SIZE(bullets_player_straight), &ndx_player_straight);
}

void bullet_player_straightdouble(int x, int y)
{

}

void bullet_player_straighttriple(int x, int y)
{

}

void bullet_player_spread(int x, int y)
{

}

void bullet_player_spreaddouble(int x, int y)
{

}

void bullet_player_spreadtriple(int x, int y)
{

}

void bullet_player_spreadquad(int x, int y)
{

}

void bullet_player_left(int x, int y)
{

}

void bullet_player_right(int x, int y)
{

}

void bullet_player_spin(int x, int y)
{

}

void bullet_player_flower(int x, int y)
{

}

// sine / cosine bullets, shot by enemy
void bullet_enemy_sin(int x, int y)
{

}

void bullet_enemy_sin_wide(int x, int y)
{

}

// sine / cosine bullets, shot by player
void bullet_player_sin(int x, int y)
{

}

void bullet_player_sin_wide(int x, int y)
{

}

// big bullets, straight
void bullet_enemy_big(int x, int y)
{

}

void bullet_player_big(int x, int y)
{

}

// beam, (fast widish, straight)
void bullet_enemy_beam(int x, int y)
{

}

void bullet_player_beam(int x, int y)
{

}
