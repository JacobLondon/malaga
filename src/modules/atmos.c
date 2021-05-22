#include <rlu/rlu.h>
#include "atmos.h"
#include "data.h"

static void init_cb_background(scene *self);
static void init_cb_starfield(scene *self);
static void init_cb_planetfield(scene *self);
static void init_cb_spacefield(scene *self);
static void init_cb_planet(scene *self);
static void init_cb_asteroids(scene *self);

static texture_manager textureman;
static scene_manager manager;

static scene_definition scenes[] = {
	{"Background", 1, init_cb_background},
	{"Starfield", 90, init_cb_starfield},
	{"Planetfield", 10, init_cb_planetfield},
	{"Spacefield", 1, init_cb_spacefield},
	{"Planet", 1, init_cb_planet},
	{"Asteroids", 100, init_cb_asteroids},
	{NULL}
};

static scene_set sets[] = {
	{"Default", {"Background", "Planetfield", "Starfield", NULL}},
	{"Asteroid", {"Spacefield", "Planet", "Asteroids", NULL}},
	{NULL}
};

void atmos_init()
{
	scene_man_new(&manager, scenes, sets);
	texture_man_new(&textureman);

	scene_man_load_set(&manager, "Asteroid");
}

void atmos_cleanup(void)
{
	scene_man_del(&manager);
	texture_man_del(&textureman);
}

void atmos_update(void)
{
	scene_man_update(&manager);
}

void atmos_draw(void)
{
	scene_man_draw(&manager);
}

/**
 * Static Functions
 */

static void init_cb_background(scene *self)
{
	char path[128];
	Texture2D *t;
	anim *a;
	so *s;

	snprintf(path, sizeof(path), "%s/%s", context_get_assetdir(), "background.png");
	t = texture_man_load_or_default(&textureman, path, TEXTURE_GEN(1000, 1000, BLACK));
	a = anim_man_load(scene_man_get_anim_man(&manager), t, 1, 1);
	s = so_new(a);
	so_set_pos(s, 0, 0);
	scene_load_object(self, s);
}

static void init_cb_starfield(scene *self)
{
	int i;
	char path[128];
	Texture2D *t;
	anim *a;
	so *s;

	// 90 items max...

	snprintf(path, sizeof(path), "%s/%s", context_get_assetdir(), "starfar.png");
	t = texture_man_load_or_default(&textureman, path, TEXTURE_GEN(1, 1, WHITE));
	a = anim_man_load(scene_man_get_anim_man(&manager), t, 1, 1);
	for (i = 0; i < 30; i++) {
		s = so_new(a);
		so_set_pos(s, GetScreenWidth() * rand_uniform(), GetScreenHeight() * rand_uniform());
		so_newmov(s, so_cb_loop_down, 2, NULL);
		scene_load_object(self, s);
	}

	snprintf(path, sizeof(path), "%s/%s", context_get_assetdir(), "starmed.png");
	t = texture_man_load_or_default(&textureman, path, TEXTURE_GEN(2, 2, WHITE));
	a = anim_man_load(scene_man_get_anim_man(&manager), t, 1, 1);
	for (i = 0; i < 30; i++) {
		s = so_new(a);
		so_set_pos(s, GetScreenWidth() * rand_uniform(), GetScreenHeight() * rand_uniform());
		so_newmov(s, so_cb_loop_down, 4, NULL);
		scene_load_object(self, s);
	}

	snprintf(path, sizeof(path), "%s/%s", context_get_assetdir(), "starnear.png");
	t = texture_man_load_or_default(&textureman, path, TEXTURE_GEN(4, 4, WHITE));
	a = anim_man_load(scene_man_get_anim_man(&manager), t, 1, 1);
	for (i = 0; i < 30; i++) {
		s = so_new(a);
		so_set_pos(s, GetScreenWidth() * rand_uniform(), GetScreenHeight() * rand_uniform());
		so_newmov(s, so_cb_loop_down, 6, NULL);
		scene_load_object(self, s);
	}
}

static void init_cb_planetfield(scene *self)
{
	char path[128];
	Texture2D *t;
	anim *a;
	so *s;

	snprintf(path, sizeof(path), "%s/%s", context_get_assetdir(), "planet.png");
	t = texture_man_load_or_default(&textureman, path, TEXTURE_GEN(35, 35, GRAY));
	a = anim_man_load(scene_man_get_anim_man(&manager), t, 1, 1);
	s = so_new(a);
	so_set_pos(s, GetScreenWidth() * rand_uniform(), GetScreenHeight() * rand_uniform());
	so_newmov(s, so_cb_loop_down, 1, NULL);
	scene_load_object(self, s);
}

static void init_cb_spacefield(scene *self)
{
	char path[128];
	Texture2D *t;
	anim *a;
	so *s;

	snprintf(path, sizeof(path), "%s/%s", context_get_assetdir(), "background2.png");
	t = texture_man_load_or_default(&textureman, path, TEXTURE_GEN(1000, 1000, BLACK));
	a = anim_man_load(scene_man_get_anim_man(&manager), t, 1, 1);
	s = so_new(a);
	so_set_pos(s, 0, 0);
	scene_load_object(self, s);
}

static void init_cb_planet(scene *self)
{
	char path[128];
	Texture2D *t;
	anim *a;
	so *s;

	snprintf(path, sizeof(path), "%s/%s", context_get_assetdir(), "planet2.png");
	t = texture_man_load_or_default(&textureman, path, TEXTURE_GEN(500, 500, GRAY));
	a = anim_man_load(scene_man_get_anim_man(&manager), t, 1, 1);
	s = so_new(a);
	so_set_pos(s, 0, 0);
	scene_load_object(self, s);
}

static void init_cb_asteroids(scene *self)
{
	int i, j;
	char path[128];
	Texture2D *t;
	anim *a;
	so *s;

	// sorted by size...
	const char *asteroids[] = {
		"asteroid4.png",
		"asteroid5.png",

		"asteroid1.png",

		"asteroid0.png",
		"asteroid6.png",

		"asteroid2.png",
		"asteroid3.png",
		"asteroid7.png",

		"asteroid8.png",
		"asteroid9.png",
		NULL,
	};

	for (i = 0; asteroids[i] != NULL; i++) {
		snprintf(path, sizeof(path), "%s/%s", context_get_assetdir(), asteroids[i]);
		t = texture_man_load_or_default(&textureman, path, TEXTURE_GEN(50, 50, BROWN));
		a = anim_man_load(scene_man_get_anim_man(&manager), t, 1, 1);

		for (j = 0; j < 10; j++) {
			s = so_new(a);
			so_set_pos(s, GetScreenWidth() * rand_uniform(), GetScreenHeight() * rand_uniform());
			so_newmov(s, so_cb_loop_down, (i + rand_uniform()) * .5, NULL);
			scene_load_object(self, s);
		}
	}
}
