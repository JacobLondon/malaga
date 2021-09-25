/**
 * TODO: Music manager in this
 */

#include <assert.h>
#include "rlu.h"

#define DEF_VECTOR_SCENEDEF
#define DEF_VECTOR_SCENESET
#include "atmosphere.gen.h"

struct atmosphere_tag {
	texture_manager textureman;
	scene_manager sceneman;

	struct VectorSceneDef scenes;
	struct VectorSceneSet sets;

	bool managers_loaded;
	char *mp3_owned;
};

atmosphere_t *atmosphere_new(char *mp3_nullable)
{
	atmosphere_t *atm = allocate(sizeof(struct atmosphere_tag));
	assert(atm != NULL);

	memset(atm, 0, sizeof(*atm));

	if (mp3_nullable != NULL)
	{
		atm->mp3_owned = strdup(mp3_nullable);
		assert(atm->mp3_owned);
	}

	atm->scenes = vector_scene_def_init();
	atm->sets = vector_scene_set_init();

	return atm;
}

void atmosphere_del(atmosphere_t *atm)
{
	assert(atm != NULL);

	if (atm->managers_loaded)
	{
		//free_definition_names(atm->scenes);
		//free_set_names(atm->sets);
		scene_man_del(&atm->sceneman);
		texture_man_del(&atm->textureman);
	}

	if (atm->mp3_owned != NULL)
	{
		dealloc(atm->mp3_owned);
	}

	vector_scene_def_deinit(&atm->scenes);
	vector_scene_set_deinit(&atm->sets);

	(void)memset(atm, 0, sizeof(*atm));
	dealloc(atm);
}

int atmosphere_insert_definition(atmosphere_t *atm, char *scene_name, size_t max_objects, scene_cb init, void *client)
{
	char *copy;

	assert(atm != NULL);
	assert(scene_name != NULL);
	assert(init != NULL);
	assert(max_objects != 0);

	copy = strdup(scene_name);
	assert(copy);

	scene_definition sd = {
		.name = copy,
		.max_objects = max_objects,
		.init = init,
		.client = client,
	};
	vector_scene_def_push(&atm->scenes, sd);

	return 0;
}

int atmosphere_insert_set(atmosphere_t *atm, char *set_name)
{
	char *copy;

	assert(atm != NULL);
	assert(set_name != NULL);

	copy = strdup(set_name);
	assert(copy);

	scene_set ss = {
		.name = copy,
		.scene_names = {NULL},
	};

	vector_scene_set_push(&atm->sets, ss);

	return 0;
}

int atmosphere_insert_set_scene(atmosphere_t *atm, char *set_name, char *scene_name)
{
	unsigned i;
	scene_set *find = NULL;
	scene_set *ss;
	char *s = NULL;
	assert(atm != NULL);
	assert(set_name != NULL);
	assert(scene_name != NULL);

	for (ss = vector_scene_set_iter(&atm->sets);
	     ss;
	     ss = vector_scene_set_next(&atm->sets, ss))
	{
		if (streq(ss->name, set_name))
		{
			find = ss;
			break;
		}
	}

	if (find == NULL)
	{
		return 1;
	}

	for (i = 0; i < ARRAY_SIZE(find->scene_names) - 2; i++)
	{
		if (find->scene_names[i] == NULL)
		{
			s = strdup(scene_name);
			assert(s != NULL);
			find->scene_names[i] = s;
			break;
		}
	}

	if (s == NULL)
	{
		return 1;
	}

	return 0;
}

/**
 * After this has been called, _insert_ functions shouldn't be used
 */
void atmosphere_finish_inserting(atmosphere_t *atm)
{
	assert(atm != NULL);

	vector_scene_def_push(&atm->scenes, (scene_definition){0});

	scene_man_new(&atm->sceneman, atm->scenes.buf, atm->sets.buf);
	texture_man_new(&atm->textureman);
	atm->managers_loaded = true;
}

void atmosphere_load(atmosphere_t *atm, const char *set_name)
{
	assert(atm != NULL);
	assert(set_name != NULL);

	scene_man_load_set(&atm->sceneman, set_name);
}

void atmosphere_update(atmosphere_t *atm)
{
	assert(atm != NULL);
	scene_man_update(&atm->sceneman);
}

void atmosphere_draw(atmosphere_t *atm)
{
	assert(atm != NULL);
	scene_man_draw(&atm->sceneman);
}

const char *atmosphere_get_setname_or_default(atmosphere_t *atm, char *set_name, char *def)
{
	scene_set *ss;
	char *p = NULL;
	assert(atm != NULL);

	if (set_name == NULL)
	{
		return def;
	}

	for (ss = vector_scene_set_iter(&atm->sets);
	     ss;
	     ss = vector_scene_set_next(&atm->sets, ss))
	{
		if (streq(set_name, ss->name))
		{
			p = set_name;
			break;
		}
	}

	if (p == NULL)
	{
		p = def;
	}

	return p;
}

scene_manager *atmosphere_get_sceneman(atmosphere_t *atm)
{
	assert(atm != NULL);
	return &atm->sceneman;
}

texture_manager *atmosphere_get_textureman(atmosphere_t *atm)
{
	assert(atm != NULL);
	return &atm->textureman;
}
