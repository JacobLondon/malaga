#include <rlu/rlu.h>
#include "atmos.h"
#include "data.h"

static void init_cb_background(scene *self, void *client);
static void init_cb_starfield(scene *self, void *client);
static void init_cb_planetfield(scene *self, void *client);
static void init_cb_spacefield(scene *self, void *client);
static void init_cb_planet(scene *self, void *client);
static void init_cb_asteroids(scene *self, void *client);
static void init_cb_traffic(scene *self, void *client);
static void init_cb_darkrise(scene *self, void *client);

static atmosphere_t *atmosphere;
static scene_manager *manager;
static texture_manager *textureman;

void atmos_init(char *setname)
{
	const char *p;

	atmosphere = atmosphere_new(NULL);
	atmosphere_insert_definition(atmosphere, "Background", 1, init_cb_background, NULL);
	atmosphere_insert_definition(atmosphere, "Starfield", 90, init_cb_starfield, NULL);
	atmosphere_insert_definition(atmosphere, "Planetfield", 10, init_cb_planetfield, NULL);
	atmosphere_insert_definition(atmosphere, "Spacefield", 1, init_cb_spacefield, NULL);
	atmosphere_insert_definition(atmosphere, "Planet", 1, init_cb_planet, NULL);
	atmosphere_insert_definition(atmosphere, "Asteroids", 100, init_cb_asteroids, NULL);
	atmosphere_insert_definition(atmosphere, "Traffic", 100, init_cb_traffic, NULL);
	atmosphere_insert_definition(atmosphere, "BackgroundDarkrise", 1, init_cb_darkrise, NULL);

	atmosphere_insert_set(atmosphere, "Default");
	atmosphere_insert_set_scene(atmosphere, "Default", "Background");
	atmosphere_insert_set_scene(atmosphere, "Default", "Planetfield");
	atmosphere_insert_set_scene(atmosphere, "Default", "Starfield");

	atmosphere_insert_set(atmosphere, "Asteroid");
	atmosphere_insert_set_scene(atmosphere, "Asteroid", "Spacefield");
	atmosphere_insert_set_scene(atmosphere, "Asteroid", "Planet");
	atmosphere_insert_set_scene(atmosphere, "Asteroid", "Asteroids");

	atmosphere_insert_set(atmosphere, "Dark");
	atmosphere_insert_set_scene(atmosphere, "Dark", "BackgroundDarkrise");
	atmosphere_insert_set_scene(atmosphere, "Dark", "Traffic");

	atmosphere_finish_inserting(atmosphere);
	manager = atmosphere_get_sceneman(atmosphere);
	textureman = atmosphere_get_textureman(atmosphere);

	p = atmosphere_get_setname_or_default(atmosphere, setname, "Default");
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
	(void)client;
	
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
	(void)client;

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
	Texture2D *t;
	anim *a;
	so *s;
	(void)client;

	t = texture_man_load_or_default(textureman, tempbuf("%s/%s", context_get_assetdir(), "planet.png"), TEXTURE_GEN(35, 35, GRAY));
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
	(void)client;

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
	(void)client;

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
	(void)client;

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

static void init_cb_traffic(scene *self, void *client)
{
	int i;
	(void)client;
	Texture2D *t1 = texture_man_load_or_default(textureman, tempbuf("%s/%s", context_get_assetdir(), "ship_lakota.png"), TEXTURE_GEN(50, 50, GRAY));
	Texture2D *t2 = texture_man_load_or_default(textureman, tempbuf("%s/%s", context_get_assetdir(), "ship_brokenarm.png"), TEXTURE_GEN(75, 75, GRAY));
	Texture2D *t3 = texture_man_load_or_default(textureman, tempbuf("%s/%s", context_get_assetdir(), "ship_dreadnought.png"), TEXTURE_GEN(100, 100, GRAY));
	anim *a1 = anim_man_load(scene_man_get_anim_man(manager), t1, 1, 1);
	anim *a2 = anim_man_load(scene_man_get_anim_man(manager), t2, 1, 1);
	anim *a3 = anim_man_load(scene_man_get_anim_man(manager), t3, 1, 1);

	so *tmp;
	so *s1 = so_new(a1);
	so *s2 = so_new(a2);
	so *s3 = so_new(a3);

	so_newmov(s1, so_cb_loop_down, 2, NULL);
	so_newmov(s1, so_cb_loop_left, 5, NULL);
	so_set_scale(s1, 0.05);

	for (i = rand_range(20, 40); i >= 0; i--) {
		tmp = so_copy(s1);
		so_set_pos(tmp, rand_uniform() * GetScreenWidth(), rand_uniform() * GetScreenHeight());
		so_newmov(tmp, so_cb_bob_hrz, rand_range(3, 6), NULL);
		scene_load_object(self, tmp);
	}

	so_newmov(s2, so_cb_loop_down, 2.5, NULL);
	so_newmov(s2, so_cb_loop_left, 9, NULL);
	so_set_scale(s2, 0.08);

	for (i = rand_range(20, 40); i >= 0; i--) {
		tmp = so_copy(s2);
		so_set_pos(tmp, rand_uniform() * GetScreenWidth(), rand_uniform() * GetScreenHeight());
		so_newmov(tmp, so_cb_bob_vrt, rand_range(4, 8), NULL);
		scene_load_object(self, tmp);
	}

	so_newmov(s3, so_cb_loop_down, 0.9, NULL);
	so_newmov(s3, so_cb_loop_left, 1.6, NULL);
	so_set_scale(s3, 0.5);
	so_set_pos(s3, GetScreenWidth(), GetScreenHeight());
	scene_load_object(self, s3);
}

static void init_cb_darkrise(scene *self, void *client)
{
	char path[128];
	Texture2D *t;
	anim *a;
	so *s;
	(void)client;

	snprintf(path, sizeof(path), "%s/%s", context_get_assetdir(), "background_dark.png");
	t = texture_man_load_or_default(textureman, path, TEXTURE_GEN(1000, 1000, BLACK));
	a = anim_man_load(scene_man_get_anim_man(manager), t, 1, 1);
	s = so_new(a);
	so_set_pos(s, -t->width / 2, 0);
	scene_load_object(self, s);
}
