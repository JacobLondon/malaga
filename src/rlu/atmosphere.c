/**
 * TODO: Music manager in this
 */

#include <assert.h>
#include <rlu/rlu.h>

#define SETS_MAX 8
#define SCENES_MAX 32

struct atmosphere_tag {
	char *mp3_owned;
	texture_manager textureman;
	scene_manager sceneman;
	bool managers_loaded;

	scene_definition scenes[SCENES_MAX];
	scene_set sets[SETS_MAX];
	int scene_ndx;
	int set_ndx;
};

// we're weird and take ownership of these names, the managers assume const char, but we are owning them
static void free_definition_names(scene_definition *scenes);
static void free_set_definition_names(char **names);
static void free_set_names(scene_set *sets);

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

	return atm;
}

void atmosphere_del(atmosphere_t *atm)
{
	assert(atm != NULL);

	if (atm->managers_loaded)
	{
		free_definition_names(atm->scenes);
		free_set_names(atm->sets);
		scene_man_del(&atm->sceneman);
		texture_man_del(&atm->textureman);
	}

	if (atm->mp3_owned != NULL)
	{
		dealloc(atm->mp3_owned);
	}

	(void)memset(atm, 0, sizeof(*atm));
	dealloc(atm);
}

int atmosphere_insert_definition(atmosphere_t *atm, char *scene_name, size_t max_objects, scene_cb init, void *client)
{
	scene_definition *sd;

	assert(atm != NULL);
	assert(scene_name != NULL);
	assert(init != NULL);
	assert(max_objects != 0);
	msg_assert((size_t)atm->scene_ndx < ARRAY_SIZE(atm->scenes) - 1, "Too many scene definitions loaded at %s", scene_name);

	sd = &atm->scenes[atm->scene_ndx];
	sd->name = strdup(scene_name);
	sd->max_objects = max_objects;
	sd->init = init;
	sd->client = client;
	atm->scene_ndx++;

	return 0;
}

int atmosphere_insert_set(atmosphere_t *atm, char *set_name)
{
	scene_set *ss;

	assert(atm != NULL);
	assert(set_name != NULL);
	msg_assert((size_t)atm->set_ndx < ARRAY_SIZE(atm->sets) - 1);

	ss = &atm->sets[atm->set_ndx];
	ss->name = strdup(set_name);
	assert(ss->name);
	atm->set_ndx++;

	return 0;
}

int atmosphere_insert_set_scene(atmosphere_t *atm, char *set_name, char *scene_name)
{
	unsigned i;
	scene_set *ss = NULL;
	char *s = NULL;
	assert(atm != NULL);
	assert(set_name != NULL);
	assert(scene_name != NULL);

	for (i = 0; atm->sets[i].name != NULL; i++)
	{
		if (streq(atm->sets[i].name, set_name))
		{
			ss = &atm->sets[i];
			break;
		}
	}

	if (ss == NULL)
	{
		return 1;
	}

	for (i = 0; i < ARRAY_SIZE(ss->scene_names) - 1; i++)
	{
		if (ss->scene_names[i] == NULL)
		{
			s = strdup(scene_name);
			assert(s != NULL);
			ss->scene_names[i] = s;
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

	scene_man_new(&atm->sceneman, atm->scenes, atm->sets);
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
	size_t i;
	scene_set *ss;
	char *p = NULL;
	assert(atm != NULL);

	if (set_name == NULL)
	{
		return def;
	}

	ss = atm->sets;
	for (i = 0; ss[i].name != NULL; i++)
	{
		assert(i < ARRAY_SIZE(atm->sets) - 1);
		if (streq(set_name, ss[i].name))
		{
			p = set_name;
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

static void free_definition_names(scene_definition *scenes)
{
	int i;
	assert(scenes != NULL);

	for (i = 0; scenes[i].name != NULL; i++)
	{
		dealloc(scenes[i].name);
	}
}

static void free_set_definition_names(char **names)
{
	int i;
	assert(names != NULL);

	for (i = 0; names[i] != NULL; i++)
	{
		dealloc(names[i]);
	}
}

static void free_set_names(scene_set *sets)
{
	int i;
	assert(sets != NULL);

	for (i = 0; sets[i].name != NULL; i++)
	{
		dealloc((char *)sets[i].name);
		free_set_definition_names(sets[i].scene_names);
	}
}
