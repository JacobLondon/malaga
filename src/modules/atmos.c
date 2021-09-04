#include <rlu/rlu.h>
#include "atmos.h"
#include "data.h"

static void init_cb_background(scene *self, void *client);
static void init_cb_starfield(scene *self, void *client);
static void init_cb_planetfield(scene *self, void *client);
static void init_cb_spacefield(scene *self, void *client);
static void init_cb_planet(scene *self, void *client);
static void init_cb_asteroids(scene *self, void *client);

static atmosphere_t *atmosphere;
static scene_manager *manager;
static texture_manager *textureman;

void atmos_init(char *setname)
{
	const char *p = NULL;
	int i;

	if (!p)
	{
		p = "Default";
	}

	atmosphere = atmosphere_new(NULL);
	atmosphere_insert_definition(atmosphere, "Background", 1, init_cb_background, NULL);
	atmosphere_insert_definition(atmosphere, "Starfield", 90, init_cb_starfield, NULL);
	atmosphere_insert_definition(atmosphere, "Planetfield", 10, init_cb_planetfield, NULL);
	atmosphere_insert_definition(atmosphere, "Spacefield", 1, init_cb_spacefield, NULL);
	atmosphere_insert_definition(atmosphere, "Planet", 1, init_cb_planet, NULL);
	atmosphere_insert_definition(atmosphere, "Asteroids", 100, init_cb_asteroids, NULL);

	atmosphere_insert_set(atmosphere, "Default");
	atmosphere_insert_set_scene(atmosphere, "Default", "Background");
	atmosphere_insert_set_scene(atmosphere, "Default", "Planetfield");
	atmosphere_insert_set_scene(atmosphere, "Default", "Starfield");

	atmosphere_insert_set(atmosphere, "Asteroid");
	atmosphere_insert_set_scene(atmosphere, "Asteroid", "Spacefield");
	atmosphere_insert_set_scene(atmosphere, "Asteroid", "Planet");
	atmosphere_insert_set_scene(atmosphere, "Asteroid", "Asteroids");

	atmosphere_finish_inserting(atmosphere);
	manager = atmosphere_get_sceneman(atmosphere);
	textureman = atmosphere_get_textureman(atmosphere);

	atmosphere_load(atmosphere, p);
}

void atmos_cleanup(void)
{
	atmosphere_del(atmosphere);
	atmosphere = NULL;
	manager = NULL;
	textureman = NULL;
}

void atmos_update(void)
{
	atmosphere_update(atmosphere);
}

void atmos_draw(void)
{
	atmosphere_draw(atmosphere);
}

/**
 * Static Functions
 */

static void init_cb_background(scene *self, void *client)
{
	char path[128];
	Texture2D *t;
	anim *a;
	so *s;

	snprintf(path, sizeof(path), "%s/%s", context_get_assetdir(), "background.png");
	t = texture_man_load_or_default(textureman, path, TEXTURE_GEN(1000, 1000, BLACK));
	a = anim_man_load(scene_man_get_anim_man(manager), t, 1, 1);
	s = so_new(a);
	so_set_pos(s, 0, 0);
	scene_load_object(self, s);
}

static void init_cb_starfield(scene *self, void *client)
{
	int i;
	char path[128];
	Texture2D *t;
	anim *a;
	so *s;

	// 90 items max...

	snprintf(path, sizeof(path), "%s/%s", context_get_assetdir(), "starfar.png");
	t = texture_man_load_or_default(textureman, path, TEXTURE_GEN(1, 1, WHITE));
	a = anim_man_load(scene_man_get_anim_man(manager), t, 1, 1);
	for (i = 0; i < 30; i++) {
		s = so_new(a);
		so_set_pos(s, GetScreenWidth() * rand_uniform(), GetScreenHeight() * rand_uniform());
		so_newmov(s, so_cb_loop_down, 2, NULL);
		scene_load_object(self, s);
	}

	snprintf(path, sizeof(path), "%s/%s", context_get_assetdir(), "starmed.png");
	t = texture_man_load_or_default(textureman, path, TEXTURE_GEN(2, 2, WHITE));
	a = anim_man_load(scene_man_get_anim_man(manager), t, 1, 1);
	for (i = 0; i < 30; i++) {
		s = so_new(a);
		so_set_pos(s, GetScreenWidth() * rand_uniform(), GetScreenHeight() * rand_uniform());
		so_newmov(s, so_cb_loop_down, 4, NULL);
		scene_load_object(self, s);
	}

	snprintf(path, sizeof(path), "%s/%s", context_get_assetdir(), "starnear.png");
	t = texture_man_load_or_default(textureman, path, TEXTURE_GEN(4, 4, WHITE));
	a = anim_man_load(scene_man_get_anim_man(manager), t, 1, 1);
	for (i = 0; i < 30; i++) {
		s = so_new(a);
		so_set_pos(s, GetScreenWidth() * rand_uniform(), GetScreenHeight() * rand_uniform());
		so_newmov(s, so_cb_loop_down, 6, NULL);
		scene_load_object(self, s);
	}
}

static void init_cb_planetfield(scene *self, void *client)
{
	char path[128];
	Texture2D *t;
	anim *a;
	so *s;

	snprintf(path, sizeof(path), "%s/%s", context_get_assetdir(), "planet.png");
	t = texture_man_load_or_default(textureman, path, TEXTURE_GEN(35, 35, GRAY));
	a = anim_man_load(scene_man_get_anim_man(manager), t, 1, 1);
	s = so_new(a);
	so_set_pos(s, GetScreenWidth() * rand_uniform(), GetScreenHeight() * rand_uniform());
	so_newmov(s, so_cb_loop_down, 1, NULL);
	scene_load_object(self, s);
}

static void init_cb_spacefield(scene *self, void *client)
{
	char path[128];
	Texture2D *t;
	anim *a;
	so *s;

	snprintf(path, sizeof(path), "%s/%s", context_get_assetdir(), "background2.png");
	t = texture_man_load_or_default(textureman, path, TEXTURE_GEN(1000, 1000, BLACK));
	a = anim_man_load(scene_man_get_anim_man(manager), t, 1, 1);
	s = so_new(a);
	so_set_pos(s, 0, 0);
	scene_load_object(self, s);
}

static void init_cb_planet(scene *self, void *client)
{
	char path[128];
	Texture2D *t;
	anim *a;
	so *s;

	snprintf(path, sizeof(path), "%s/%s", context_get_assetdir(), "planet2.png");
	t = texture_man_load_or_default(textureman, path, TEXTURE_GEN(500, 500, GRAY));
	a = anim_man_load(scene_man_get_anim_man(manager), t, 1, 1);
	s = so_new(a);
	so_set_pos(s, 0, 0);
	scene_load_object(self, s);
}

static void init_cb_asteroids(scene *self, void *client)
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
		t = texture_man_load_or_default(textureman, path, TEXTURE_GEN(50, 50, BROWN));
		a = anim_man_load(scene_man_get_anim_man(manager), t, 1, 1);

		for (j = 0; j < 10; j++) {
			s = so_new(a);
			so_set_pos(s, GetScreenWidth() * rand_uniform(), GetScreenHeight() * rand_uniform());
			so_newmov(s, so_cb_loop_down, (i + rand_uniform()) * .5, NULL);
			scene_load_object(self, s);
		}
	}
}
