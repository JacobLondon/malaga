#include <rlu/rlu.h>

#include "map.h"
#include "data.h"

#define DEFAULT_ENEMY_WIDTH 30
#define DEFAULT_ENEMY_HEIGHT 30
#define ENCOUNTER_ENEMIES_MAX 32

static char defenemyasset[256];

// DEFAULT ////////////////////////////////////////////////////////////////////
static enemy_definition default_enemy0 = {
	.shoot=bullet_enemy_flower,
	.move=enemy_move_downstop,
	.hp=7,
	.width=DEFAULT_ENEMY_WIDTH,
	.height=DEFAULT_ENEMY_HEIGHT,
	.speed=10,
	.level=0,
	.meta={.5f},
	.name="Default0",
	.pngname=defenemyasset,
};

static enemy_definition default_enemy1 = {
	.shoot=bullet_enemy_flower,
	.move=enemy_move_downstop,
	.hp=7,
	.width=DEFAULT_ENEMY_WIDTH,
	.height=DEFAULT_ENEMY_HEIGHT,
	.speed=10,
	.level=0,
	.meta={.9f},
	.name="Default1",
	.pngname=defenemyasset,
};

static encounter default_encounter0[] = {
	DEFINE_ENCOUNTER(&default_enemy1, 0, .5, 0),
	DEFINE_ENCOUNTER(&default_enemy0, .5, .2, 0),
	DEFINE_ENCOUNTER(&default_enemy0, .5, .7, 0),
	DEFINE_ENCOUNTER_END(),
};

static encounter default_encounter1[] = {
	DEFINE_ENCOUNTER(&default_enemy0, 0, .4, 0),
	DEFINE_ENCOUNTER(&default_enemy0, 0, .6, 0),
	DEFINE_ENCOUNTER(&default_enemy1, 1, .8, 0),
	DEFINE_ENCOUNTER(&default_enemy1, 1, .2, 0),
	DEFINE_ENCOUNTER_END(),
};

static encounter *default_encounters[] = {
	default_encounter0,
	default_encounter1,
	NULL
};

///////////////////////////////////////////////////////////////////////////////

#define ENEMIES_ID "$enemies"
#define ENEMIES_LEN (sizeof(ENEMIES_ID) - 1)
#define ENCOUNTERS_ID "$encounters"
#define ENCOUNTERS_LEN (sizeof(ENCOUNTERS_ID) - 1)
#define SETS_ID "$sets"
#define SETS_LEN (sizeof(SETS_ID) - 1)

typedef enum parse_state_tag {
	STATE_BEGIN,
	STATE_ENEMIES,
	STATE_ENCOUNTERS,
	STATE_SETS,

	STATE_IN_ENEMY,
	STATE_NEW_ENCOUNTER,
	STATE_IN_ENCOUNTER,
} parse_state;

typedef struct encounter_holder_tag {
	char *name;
	encounter spawn[ENCOUNTER_ENEMIES_MAX];
	encounter null;
} encounter_holder;

enemy_definition *make_enemy(enemy_definition *template);
void del_enemy(void *self);
encounter_holder *make_encounter_holder(encounter_holder *template);
void del_encounter_holder(void *self);

static struct parray *enemies = NULL;
static struct parray *encounters = NULL; // holds encounter_holders
static struct parray *sets = NULL;
static int initialized = 0;
static int used_static_def = 0;
static struct MapHeader headerinfo;

encounter **map_init(const char *mapfilename, struct MapHeader *out)
{
	assert(initialized == 0);
	assert(out);

	initialized = 1;
	memset(&headerinfo, 0, sizeof(headerinfo));

	if (mapfilename == NULL) {
	static_def:
		memcpy(out, &headerinfo, sizeof(*out));
		used_static_def = 1;
		return default_encounters;
	}

	FILE *fp;
	char buf[256];
	char id[256];
	char lhs[256];
	char rhs[256];
	char *p;
	size_t i;
	size_t lineno;
	parse_state state;
	// scanf'd items...
	enemy_definition enemy_template;
	encounter encounter_template;
	encounter_holder holder;
	int holderndx = 0;
	float real;
	int whole;
	bool in_header = false;

	// these must all have been satisfied, or the file is invalid
	bool found_enemy = false;
	bool found_encounter = false;
	bool found_set = false;

	snprintf(defenemyasset, sizeof(defenemyasset), "%s/%s", context_get_assetdir(), DATA_ASSET_ENEMY);

	enemies = parray_new(del_enemy);
	encounters = parray_new(del_encounter_holder);
	sets = parray_new(NULL); // points into encounters after it is built
	assert(enemies);
	assert(encounters);
	assert(sets);

	fp = fopen(mapfilename, "r");
	if (!fp) {
		msg_warning("Could not load %s. Defaulting to static encounters.", mapfilename);
		goto static_def;
	}

	state = STATE_BEGIN;
	for (lineno = 1; fgets(buf, sizeof(buf), fp); lineno++) {
		if (buf[0] == '#' || buf[0] == '\r' || buf[0] == '\n') {
			continue;
		}

		if (buf[0] == '*' && buf[1] == '*') {
			in_header = !in_header;
			continue;
		}

		if (in_header) {
			if (sscanf(buf, "%s %s", lhs, rhs) == 2) {
				if (strcmp(lhs, "atmosphere") == 0) {
					snprintf(headerinfo.atmosphere, sizeof(headerinfo.atmosphere), "%s", rhs);
				}
				else if (strcmp(lhs, "music") == 0) {
					snprintf(headerinfo.music, sizeof(headerinfo.music), "%s", rhs);
				}
				else {
					msg_warning("%s:%zu Unexpected header identifier found `%s'", mapfilename, lineno, buf);
					goto fail;
				}
			}
			else {
				msg_warning("%s:%zu Header expected 2 attributes: `LHS RHS' found `%s'", mapfilename, lineno, buf);
				goto fail;
			}
			continue;
		}

		if (state == STATE_BEGIN ||
		    state == STATE_ENEMIES ||
		    state == STATE_ENCOUNTERS ||
		    state == STATE_SETS)
		{
			if (strncmp(buf, ENEMIES_ID, ENEMIES_LEN) == 0) {
				state = STATE_ENEMIES;
				continue;
			}
			else if (strncmp(buf, ENCOUNTERS_ID, ENCOUNTERS_LEN) == 0) {
				state = STATE_ENCOUNTERS;
				continue;
			}
			else if (strncmp(buf, SETS_ID, SETS_LEN) == 0) {
				state = STATE_SETS;
				continue;
			}
		}

		switch (state) {
		case STATE_BEGIN:
			break;
		case STATE_ENEMIES:
			if (buf[0] == '(') {
				(void)memset(&enemy_template, 0, sizeof(enemy_template));
				state = STATE_IN_ENEMY;
			}
			else {
				msg_warning("%s:%zu Expected `(' found `%c'", mapfilename, lineno, buf[0]);
				goto fail;
			}
			break;
		case STATE_IN_ENEMY:
			if (buf[0] == ')') {
				if (!enemy_template.name) {
					msg_warning("%s:%zu Expected `name <enemy-name>' but found none", mapfilename, lineno);
					goto fail;
				}
				if (enemy_template.width == 0) {
					enemy_template.width = DEFAULT_ENEMY_WIDTH;
				}
				if (enemy_template.height == 0) {
					enemy_template.height = DEFAULT_ENEMY_HEIGHT;
				}
				if (enemy_template.pngname == NULL) {
					enemy_template.pngname = strdup(defenemyasset);
					assert(enemy_template.pngname);
				}
				parray_push(enemies, make_enemy(&enemy_template));

				// this section has at least been parsed successfully at least once
				found_enemy = true;
				state = STATE_ENEMIES;
				memset(&enemy_template, 0, sizeof(enemy_template));
			}
			else if (sscanf(buf, "%s %s", lhs, rhs) == 2) {
				// name of enemy_definition to use
				if (strcmp(lhs, "name") == 0) {
					enemy_template.name = strdup(rhs);
					assert(enemy_template.name);
				}
				else if (strcmp(lhs, "png") == 0) {
					enemy_template.pngname = strdup(rhs);
					assert(enemy_template.pngname);
				}
				else if (strcmp(lhs, "shoot") == 0) {
					enemy_template.shoot = bullet_lookup_shoot(rhs);
					if (!enemy_template.shoot) {
						msg_warning("%s:%zu Shoot func `%s' does not exist", mapfilename, lineno, rhs);
						goto fail;
					}
				}
				else if (strcmp(lhs, "move") == 0) {
					enemy_template.move = lookup_enemy_move(rhs);
					if (!enemy_template.move) {
						msg_warning("%s:%zu Move func `%s' does not exist", mapfilename, lineno, rhs);
						goto fail;
					}
				}
				else if (strcmp(lhs, "hp") == 0) {
					if (sscanf(rhs, "%d", &whole) == 1) {
						enemy_template.hp = whole;
					}
					else {
						msg_warning("%s:%zu Could not parse int: `%s'", mapfilename, lineno, rhs);
						goto fail;
					}
				}
				else if (strcmp(lhs, "speed") == 0) {
					if (sscanf(rhs, "%f", &real) == 1) {
						enemy_template.speed = real;
					}
					else {
						msg_warning("%s:%zu Could not parse float: `%s'", mapfilename, lineno, rhs);
						goto fail;
					}
				}
				else if (strcmp(lhs, "level") == 0) {
					if (sscanf(rhs, "%d", &whole) == 1) {
						enemy_template.level = whole;
					}
					else {
						msg_warning("%s:%zu Could not parse int: `%s'", mapfilename, lineno, rhs);
						goto fail;
					}
				}
				else if (strcmp(lhs, "meta") == 0) {
					if (sscanf(rhs, "%f", &real) == 1) {
						enemy_template.meta.downstop = real;
					}
					else {
						msg_warning("%s:%zu Could not parse float: `%s'", mapfilename, lineno, rhs);
						goto fail;
					}
				}
				else if (strcmp(lhs, "width") == 0) {
					if (sscanf(rhs, "%d", &whole) == 1) {
						enemy_template.width = whole;
					}
					else {
						msg_warning("%s:%zu Could not parse int: `%s'", mapfilename, lineno, rhs);
						goto fail;
					}
				}
				else if (strcmp(lhs, "height") == 0) {
					if (sscanf(rhs, "%d", &whole) == 1) {
						enemy_template.height = whole;
					}
					else {
						msg_warning("%s:%zu Could not parse int: `%s'", mapfilename, lineno, rhs);
						goto fail;
					}
				}
			}
			else {
				msg_warning("%s:%zu Expected 2 attributes: `LHS RHS' found `%s'", mapfilename, lineno, buf);
				goto fail;
			}
			break;
		case STATE_ENCOUNTERS:
			if (buf[0] == '[') {
				p = strchr(buf, ']');
				if (!p) {
					msg_warning("%s:%zu Expected `]'", mapfilename, lineno);
					goto fail;
				}
				(void)snprintf(id, sizeof(id), "%.*s", (int)(p - buf - 1), &buf[1]);

				// save it
				if (holderndx != 0) {
					parray_push(encounters, make_encounter_holder(&holder));
				}
				(void)memset(&holder, 0, sizeof(holder));
				holder.name = strdup(id);
				assert(holder.name);
				holderndx = 0;
				state = STATE_NEW_ENCOUNTER;
			}
			else if (buf[0] == '(') {
				(void)memset(&encounter_template, 0, sizeof(encounter_template));
				state = STATE_IN_ENCOUNTER;
			}
			else {
				msg_warning("%s:%zu Expected `[<encounter-name>]' or `(' found `%c'", mapfilename, lineno, buf[0]);
				goto fail;
			}
			break;
		case STATE_NEW_ENCOUNTER:
			if (buf[0] == '(') {
				(void)memset(&encounter_template, 0, sizeof(encounter_template));
				state = STATE_IN_ENCOUNTER;
			}
			else {
				msg_warning("%s:%zu Expected `(' found `%c'", mapfilename, lineno, buf[0]);
				goto fail;
			}
			break;
		case STATE_IN_ENCOUNTER:
			if (buf[0] == ')') {
				if (holderndx < ENCOUNTER_ENEMIES_MAX) {
					// this section has at least been parsed successfully at least once
					found_encounter = true;
					holder.spawn[holderndx] = encounter_template;
					holderndx++;
				}
				else {
					msg_warning("%s:%zu Too many enemies in encounter (%d/%d)", mapfilename, lineno, holderndx + 1, ENCOUNTER_ENEMIES_MAX);
					goto fail;
				}
				state = STATE_ENCOUNTERS;
				memset(&encounter_template, 0, sizeof(encounter_template));
			}
			else if (sscanf(buf, "%s %s", lhs, rhs) == 2) {
				// name of enemy_definition to use
				if (strcmp(lhs, "name") == 0) {
					for (i = 0; i < enemies->size; i++) {
						if (strcmp(rhs, ((enemy_definition *)(enemies->buf[i]))->name) == 0) {
							encounter_template.definition = enemies->buf[i];
							goto next;
						}
					}
					msg_warning("%s:%zu Enemy name `%s' not found", mapfilename, lineno, rhs);
					goto fail;
				}
				else if (strcmp(lhs, "time") == 0) {
					if (sscanf(rhs, "%f", &real) == 1) {
						encounter_template.spawntime = real;
					}
					else {
						msg_warning("%s:%zu Could not parse float: `%s'", mapfilename, lineno, rhs);
						goto fail;
					}
				}
				else if (strcmp(lhs, "x") == 0) {
					if (sscanf(rhs, "%f", &real) == 1) {
						encounter_template.x = real / 100.f;
					}
					else {
						msg_warning("%s:%zu Could not parse float: `%s'", mapfilename, lineno, rhs);
						goto fail;
					}
				}
				else if (strcmp(lhs, "y") == 0) {
					if (sscanf(rhs, "%f", &real) == 1) {
						encounter_template.y = real / 100.f;
					}
					else {
						msg_warning("%s:%zu Could not parse float: `%s'", mapfilename, lineno, rhs);
						goto fail;
					}
				}
				else {
					msg_warning("%s:%zu Expected an enemy attribute, found `%s'", mapfilename, lineno, lhs);
					goto fail;
				}
			}
			else {
				msg_warning("%s:%zu Expected 2 attributes: `LHS RHS' found `%s'", mapfilename, lineno, buf);
				goto fail;
			}
			break;
		case STATE_SETS:
			// should only ever be in this state once...
			// so make sure to append the last encounter
			if (holderndx > 0) {
				parray_push(encounters, make_encounter_holder(&holder));
				holderndx = 0;
				(void)memset(&holder, 0, sizeof(holder));
			}

			if (sscanf(buf, "%s", lhs) == 1) {
				for (i = 0; i < encounters->size; i++) {
					encounter_holder *eh = encounters->buf[i];
					if (strcmp(lhs, eh->name) == 0) {
						// this section has at least been parsed successfully at least once
						found_set = true;

						parray_push(sets, eh->spawn);
						goto next;
					}
				}
				msg_warning("%s:%zu Encounter name `%s' not found", mapfilename, lineno, lhs);
			}
			else {
				msg_warning("%s:%zu Could not read line `%s'", mapfilename, lineno, buf);
			}
			break;
		default:
			msg_warning("%s:%zu Software failure, unknown state: %d", mapfilename, lineno, state);
			break;
		}

	next:
		;
	} // for fgets

	// must have found at least one of each
	if (!found_enemy || !found_encounter || !found_set) {
		goto fail;
	}

	(void)fclose(fp);
	parray_push(encounters, NULL);
	parray_push(sets, NULL);

	used_static_def = 0;
	memcpy(out, &headerinfo, sizeof(*out));
	msg_default("Map %s loaded", mapfilename);
	return (encounter **)sets->buf;

fail:
	(void)fclose(fp);
	parray_free(enemies);
	parray_free(encounters);
	parray_free(sets);
	goto static_def;
}

void map_cleanup(void)
{
	assert(initialized);
	initialized = 0;

	// no clean up!
	if (used_static_def) {
		return;
	}

	if (enemies) {
		parray_free(enemies);
		enemies = NULL;
	}
	if (encounters) {
		parray_free(encounters);
		encounters = NULL;
	}
	if (sets) {
		parray_free(sets);
		sets = NULL;
	}
}

enemy_definition *make_enemy(enemy_definition *template)
{
	enemy_definition *self;
	assert(template);
	self = malloc(sizeof(*self));
	assert(self);
	(void)memcpy(self, template, sizeof(*self));
	return self;
}

void del_enemy(void *self)
{
	enemy_definition *en = self;
	if (!self) {
		return;
	}

	if (en->name) {
		free(en->name);
	}
	if (en->pngname) {
		free(en->pngname);
	}
	free(self);
}

encounter_holder *make_encounter_holder(encounter_holder *template)
{
	encounter_holder *self;
	assert(template);
	self = malloc(sizeof(*self));
	assert(self);
	(void)memcpy(self, template, sizeof(*self));
	return self;
}

void del_encounter_holder(void *self)
{
	encounter_holder *holder = self;
	if (!self) {
		return;
	}

	if (holder->name) {
		free(holder->name);
	}

	free(self);
}
