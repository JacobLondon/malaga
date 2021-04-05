#include <rlu/rlu.h>

#include "map.h"

// DEFAULT ////////////////////////////////////////////////////////////////////
static enemy_definition default_ldrifter = {
	.shoot=bullet_enemy_straight,
	.move=enemy_move_downleft,
	.hp=3,
	.speed=10,
	.level=0,
};

static encounter default_encounter0[] = {
	DEFINE_ENCOUNTER(&default_ldrifter, 0, 1, 0),
	DEFINE_ENCOUNTER(&default_ldrifter, 2, 1, 0),
	DEFINE_ENCOUNTER(&default_ldrifter, 4, 1, 0),
	{NULL}
};

static encounter default_encounter1[] = {
	DEFINE_ENCOUNTER(&default_ldrifter, 0, 1, 0),
	DEFINE_ENCOUNTER(&default_ldrifter, 2, 1, 0),
	DEFINE_ENCOUNTER(&default_ldrifter, 4, 1, 0),
	{NULL}
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

enemy_definition *make_enemy(enemy_definition *template);
void del_enemy(void *self);
encounter *make_encounter(encounter *template);
void del_encounter(void *self);

static struct parray *enemies = NULL;
static struct parray *encounters = NULL;
static struct parray *sets = NULL;
static int initialized = 0;
static int used_static_def = 0;

encounter **map_init(const char *mapfilename)
{
	assert(initialized == 0);
	initialized = 1;

	if (mapfilename == NULL) {
		used_static_def = 1;
		return default_encounters;
	}

	FILE *fp;
	char buf[256];
	char id[256];
	char lhs[256];
	char rhs[256];
	char *p;
	size_t tmp;
	size_t lineno;
	parse_state state;
	// scanf'd items...
	enemy_definition enemy_template;
	encounter encounter_template;
	float real;
	int whole;

	enemies = parray_new(del_enemy);
	assert(enemies);
	encounters = parray_new(del_encounter);
	assert(encounters);
	sets = parray_new(NULL); // points into encounters after it is built
	assert(sets);

	fp = fopen(mapfilename, "r");
	msg_assert(fp, "Could not open %s", mapfilename);

	state = STATE_BEGIN;
	for (lineno = 1; fgets(buf, sizeof(buf), fp); lineno++) {
		if (buf[0] == '#' || buf[0] == '\r' || buf[0] == '\n') {
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
				msg_assert(0, "%s:%zu Expected `(' found `%c'", mapfilename, lineno, buf[0]);
			}
			break;
		case STATE_IN_ENEMY:
			if (buf[0] == ')') {
				msg_assert(enemy_template.name, "%s:%zu Expected `name <enemy-name>' but found none", mapfilename, lineno);
				parray_push(enemies, make_enemy(&enemy_template));
				state = STATE_ENEMIES;
				memset(&enemy_template, 0, sizeof(enemy_template));
			}
			else if (sscanf(buf, "%s %s", lhs, rhs) == 2) {
				// name of enemy_definition to use
				if (strcmp(lhs, "name") == 0) {
					enemy_template.name = strdup(rhs);
					assert(enemy_template.name);
				}
				else if (strcmp(lhs, "shoot") == 0) {
					enemy_template.shoot = bullet_lookup_shoot(rhs);
					msg_assert(enemy_template.shoot, "%s:%zu Shoot func `%s' does not exist", mapfilename, lineno, rhs);
				}
				else if (strcmp(lhs, "move") == 0) {
					enemy_template.move = lookup_enemy_move(rhs);
					msg_assert(enemy_template.move, "%s:%zu Move func `%s' does not exist", mapfilename, lineno, rhs);
				}
				else if (strcmp(lhs, "hp") == 0) {
					if (sscanf(rhs, "%d", &whole) == 1) {
						enemy_template.hp = whole;
					}
					else {
						msg_assert(0, "%s:%zu Could not parse int: `%s'", mapfilename, lineno, rhs);
					}
				}
				else if (strcmp(lhs, "speed") == 0) {
					if (sscanf(rhs, "%f", &real) == 1) {
						enemy_template.speed = real;
					}
					else {
						msg_assert(0, "%s:%zu Could not parse float: `%s'", mapfilename, lineno, rhs);
					}
				}
				else if (strcmp(lhs, "level") == 0) {
					if (sscanf(rhs, "%d", &whole) == 1) {
						enemy_template.level = whole;
					}
					else {
						msg_assert(0, "%s:%zu Could not parse int: `%s'", mapfilename, lineno, rhs);
					}
				}
				else if (strcmp(lhs, "meta") == 0) {
					if (sscanf(rhs, "%f", &real) == 1) {
						enemy_template.meta.downstop = real;
					}
					else {
						msg_assert(0, "%s:%zu Could not parse float: `%s'", mapfilename, lineno, rhs);
					}
				}
			}
			else {
				msg_assert(0, "%s:%zu Expected 2 attributes: `LHS RHS' found `%s'", mapfilename, lineno, buf);
			}
			break;
		case STATE_ENCOUNTERS:
			if (buf[0] == '[') {
				p = strchr(buf, ']');
				msg_assert(p, "%s:%zu Expected `]'", mapfilename, lineno);
				(void)snprintf(id, sizeof(id), "%.*s", p - buf - 1, &buf[1]);
				encounter_template.name = strdup(id);
				assert(encounter_template.name);
				state = STATE_NEW_ENCOUNTER;
			}
			else if (buf[0] == '(') {
				(void)memset(&encounter_template, 0, sizeof(encounter_template));
				state = STATE_IN_ENCOUNTER;
			}
			else {
				msg_assert(0, "%s:%zu Expected `[<encounter-name>]' or `(' found `%c'", mapfilename, lineno, buf[0]);
			}
			break;
		case STATE_NEW_ENCOUNTER:
			if (buf[0] == '(') {
				(void)memset(&encounter_template, 0, sizeof(encounter_template));
				state = STATE_IN_ENCOUNTER;
			}
			else {
				msg_assert(0, "%s:%zu Expected `(' found `%c'", mapfilename, lineno, buf[0]);
			}
			break;
		case STATE_IN_ENCOUNTER:
			if (buf[0] == ')') {
				parray_push(encounters, make_encounter(&encounter_template));
				state = STATE_ENCOUNTERS;
				memset(&encounter_template, 0, sizeof(encounter_template));
			}
			else if (sscanf(buf, "%s %s", lhs, rhs) == 2) {
				// name of enemy_definition to use
				if (strcmp(lhs, "name") == 0) {
					for (tmp = 0; tmp < enemies->size; tmp++) {
						if (strcmp(rhs, ((enemy_definition *)(enemies->buf[tmp]))->name) == 0) {
							encounter_template.definition = enemies->buf[tmp];
							goto next;
						}
					}
					msg_assert(0, "%s:%zu Enemy name `%s' not found", mapfilename, lineno, rhs);
				}
				else if (strcmp(lhs, "time") == 0) {
					if (sscanf(rhs, "%f", &real) == 1) {
						encounter_template.spawntime = real;
					}
					else {
						msg_assert(0, "%s:%zu Could not parse float: `%s'", mapfilename, lineno, rhs);
					}
				}
				else if (strcmp(lhs, "x") == 0) {
					if (sscanf(rhs, "%f", &real) == 1) {
						encounter_template.x = real / 100.f;
					}
					else {
						msg_assert(0, "%s:%zu Could not parse float: `%s'", mapfilename, lineno, rhs);
					}
				}
				else if (strcmp(lhs, "y") == 0) {
					if (sscanf(rhs, "%f", &real) == 1) {
						encounter_template.y = real / 100.f;
					}
					else {
						msg_assert(0, "%s:%zu Could not parse float: `%s'", mapfilename, lineno, rhs);
					}
				}
				else {
					msg_assert(0, "%s:%zu Expected an enemy attribute, found `%s'", mapfilename, lineno, lhs);
				}
			}
			else {
				msg_assert(0, "%s:%zu Expected 2 attributes: `LHS RHS' found `%s'", mapfilename, lineno, buf);
			}
			break;
		case STATE_SETS:
			if (sscanf(buf, "%s", lhs) == 1) {
				for (tmp = 0; tmp < encounters->size; tmp++) {
					if (strcmp(lhs, ((encounter *)encounters->buf[tmp])->name) == 0) {
						parray_push(sets, encounters->buf[tmp]);
						goto next;
					}
				}
				msg_assert(0, "%s:%zu Encounter name `%s' not found", mapfilename, lineno, lhs);
			}
			else {
				msg_assert(0, "%s:%zu Could not read line `%s'", mapfilename, lineno, buf);
			}
			break;
		default:
			msg_assert(0, "%s:%zu Software failure, unknown state: %d", mapfilename, lineno, state);
			break;
		}

	next:
		;
	} // for fgets

	(void)fclose(fp);
	parray_push(sets, NULL);

	used_static_def = 0;
	return sets->buf;
}

void map_cleanup(void)
{
	assert(initialized);
	initialized = 0;

	// no clean up!
	if (used_static_def) {
		return;
	}

	// name is only used for memory allocated and not static items, so we need to free it
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
	free(self);
}

encounter *make_encounter(encounter *template)
{
	encounter *self;
	assert(template);
	self = malloc(sizeof(*self));
	assert(self);
	(void)memcpy(self, template, sizeof(*self));
	return self;
}

void del_encounter(void *self)
{
	encounter *en = self;
	if (!self) {
		return;
	}

	if (en->name) {
		free(en->name);
	}
	free(self);
}
