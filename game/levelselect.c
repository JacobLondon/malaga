#include <rlu/rlu.h>
#include "modules.h"
#include "atmos.h"
#include "data.h"

#define AROUND_SELECTED 11
#define SHIP_COUNT 6 /** TODO: This is terrible way of doing it */

static void lselect(void *client);
static void menu(void *client);
static void tendless(void *client);
static void tdamage(void *client);
static void tinvinc(void *client);
static void nextship(void *client);
static void prevship(void *client);

static void index_maps(void);
static void clear_maps(void);
static char *get_around(int center, int AB, int max, void **buf);
static char *find_next(void);
static char *find_prev(void);

static button *select_button;
static button *menu_button;
static button *endless_button;
static button *damage_button;
static button *invinc_button;
static button *nextship_button;
static button *prevship_button;

static char *mapdir = "";
static char **dirfiles = NULL;
static int dircount = -1;
static int dirndx = 0;
static struct parray *maplist; // point into dir files with the correct files
static bool mapdirexists = false;
static bool endless_mode = false;
static bool trouble_mode = false;
static bool invinc_mode = false;

static texture_manager texman;
static Texture2D *shiptexas[SHIP_COUNT];
static int shipndx = 0;

void levelselect_init(void)
{
	int i;

	select_button = button_new("SELECT", lselect, NULL);
	menu_button = button_new("MENU", menu, NULL);
	endless_button = button_new("ENDLESS", tendless, NULL);
	damage_button = button_new("2x TROUBLE", tdamage, NULL);
	invinc_button = button_new("INVINCIBLE", tinvinc, NULL);
	nextship_button = button_new("Next Ship", nextship, NULL);
	prevship_button = button_new("Previous Ship", prevship, NULL);
	maplist = parray_new(NULL);
	assert(maplist);

	shipndx = 0;
	texture_man_new(&texman);
	for (i = 0; i < ARRAY_SIZE(shiptexas); i++) {
		shiptexas[i] = texture_man_load_or_default(
			&texman,
			tempbuf("%s/%s%d.png", context_get_skindir(), "default_player", i),
			TEXTURE_GEN(50, 50, BLUE));
	}

	// menu-ing
	component_set_size(select_button, 24, .4, .1);
	component_set_size(menu_button, 24, .4, .1);
	// modes
	component_set_size(endless_button, 24, .5, .05);
	component_set_size(damage_button, 24, .5, .05);
	component_set_size(invinc_button, 24, .5, .05);
	component_set_size(prevship_button, 25, .5, .05);
	component_set_size(nextship_button, 24, .5, .05);

	component_set_pos(select_button, .75, .9);
	component_set_pos(menu_button, .25, .9);
	component_set_pos(endless_button, .75, .4);
	component_set_pos(damage_button, .75, .475);
	component_set_pos(invinc_button, .75, .55);
	component_set_pos(nextship_button, .75, .245);
	component_set_pos(prevship_button, .75, .05);

	component_set_color(select_button, WHITE, MYDARKGRAY);
	component_set_color(menu_button, WHITE, MYDARKGRAY);

	if (!endless_mode) {
		component_set_color(endless_button, WHITE, MYDARKGRAY);
	}
	else {
		component_set_color(endless_button, MYDARKGRAY, WHITE);
	}

	if (!trouble_mode) {
		component_set_color(damage_button, WHITE, MYDARKGRAY);
	}
	else {
		component_set_color(damage_button, MYDARKGRAY, WHITE);
	}

	if (!invinc_mode) {
		component_set_color(invinc_button, WHITE, MYDARKGRAY);
	}
	else {
		component_set_color(invinc_button, MYDARKGRAY, WHITE);
	}

	component_set_color(nextship_button, WHITE, MYDARKGRAY);
	component_set_color(prevship_button, WHITE, MYDARKGRAY);

	shipndx = 0;

	atmos_init("Dark");
	index_maps();
}

void levelselect_cleanup(void)
{
	texture_man_del(&texman);
	memset(shiptexas, 0, sizeof(shiptexas));
	clear_maps();
	atmos_cleanup();
	component_del(select_button);
	component_del(menu_button);
	component_del(endless_button);
	component_del(damage_button);
	component_del(invinc_button);
	component_del(prevship_button);
	component_del(nextship_button);
	parray_free(maplist);
}

void levelselect_update(void)
{
	atmos_update();
	component_update(select_button);
	component_update(menu_button);
	component_update(endless_button);
	component_update(damage_button);
	component_update(invinc_button);
	component_update(prevship_button);
	component_update(nextship_button);

	if (IsKeyDown(KEY_R)) {
		clear_maps();
		index_maps();
	}

	if (mapdirexists) {
		if (rlu_input_key(0, RLU_KEY_BUTTON_DOWN, RLU_PRESS_RELEASED)) {
			if (strlen(mapdir) > 0) {
				lselect(NULL);
			}
		}

		int dir = GetMouseWheelMove();
		if (dir < 0 || rlu_input_key(0, RLU_KEY_DPAD_DOWN, RLU_PRESS_RELEASED)) {
			mapdir = find_next();
		}
		else if (dir > 0 || rlu_input_key(0, RLU_KEY_DPAD_UP, RLU_PRESS_RELEASED)) {
			mapdir = find_prev();
		}
	}
}

void levelselect_draw(void)
{
	const int AB = AROUND_SELECTED;
	const int fontsize = 40;
	const int height = fontsize;
	const int offset = (int)(GetScreenHeight() * .3f);
	int i;

	atmos_draw();

	// draw around
	if (dirndx != -1 && mapdirexists) {
		for (i = -AB + 1; i < AB; i++) {
			if (i == 0) {
				DrawRectangle(
					0,
					GetScreenHeight() / height * i + offset,
					GetScreenWidth(),
					fontsize,
					WHITE
				);
				DrawText(
					mapdir,
					GetScreenWidth() / 10,
					GetScreenHeight() / height * i + offset,
					fontsize,
					BLACK);
			}
			else {
				const Color c = (Color){
					.r=255, .g=255, .b=255,
					.a=(int)(255.0 * (1 - ((double)abs(i) / (double)AB)))
				};
				DrawText(
					get_around(dirndx, i, (int)maplist->size, maplist->buf),
					GetScreenWidth() / 10,
					GetScreenHeight() * .075 / 1.75 * i + offset,
					fontsize,
					c);
			}
		}
	}

	component_draw(select_button);
	component_draw(menu_button);
	component_draw(endless_button);
	component_draw(damage_button);
	component_draw(invinc_button);
	component_draw(prevship_button);
	component_draw(nextship_button);
	texture_man_draw_tex(shiptexas[shipndx], GetScreenWidth() * 0.70, GetScreenHeight() * 0.1);
}

static void lselect(void *client)
{
	struct game_message msg;
	(void)client;

	if (mapdirexists) {
		// load configuration and launch
		msg.endless_mode = endless_mode;
		msg.trouble_mode = trouble_mode;
		msg.god_mode = invinc_mode;
		if (strlen(mapdir) > 0) {
			(void)snprintf(msg.mapdir, sizeof(msg.mapdir), "%s/%s", DATA_MAPS_DIR, mapdir);
		}
		msg.playership = shipndx;

		game_conf(&msg);
		context_switch("GAME");
	}
}

static void menu(void *client)
{
	(void)client;
	context_switch("MG");
}

static void tendless(void *client)
{
	(void)client;

	// from non-endless to endless
	if (!endless_mode) {
		endless_mode = true;
		component_set_color(endless_button, MYDARKGRAY, WHITE);
	}
	// from endless to non-endless
	else {
		endless_mode = false;
		component_set_color(endless_button, WHITE, MYDARKGRAY);
	}
}

static void tdamage(void *client)
{
	(void)client;

	if (!trouble_mode) {
		trouble_mode = true;
		component_set_color(damage_button, MYDARKGRAY, WHITE);
	}
	else {
		trouble_mode = false;
		component_set_color(damage_button, WHITE, MYDARKGRAY);
	}
}

static void tinvinc(void *client)
{
	(void)client;

	if (!invinc_mode) {
		invinc_mode = true;
		component_set_color(invinc_button, MYDARKGRAY, WHITE);
	}
	else {
		invinc_mode = false;
		component_set_color(invinc_button, WHITE, MYDARKGRAY);
	}
}

static void index_maps(void)
{
	int i;

	mapdirexists = DirectoryExists(DATA_MAPS_DIR);
	if (mapdirexists) {
		dirfiles = GetDirectoryFiles(DATA_MAPS_DIR, &dircount);

		for (i = 0; i < dircount; i++) {
			if (strstr(dirfiles[i], DATA_MAPS_EXT)) {
				parray_push(maplist, dirfiles[i]);
			}
		}
		qsort(maplist->buf, maplist->size, sizeof(char *), (int (*)(const void *, const void *))strcmp);
		mapdir = find_next();
	}
}

static void clear_maps(void)
{
	if (dirfiles) {
		ClearDirectoryFiles();
		dirfiles = NULL;
		dircount = -1;
		mapdir = NULL;
	}

	while (maplist->size > 0) {
		parray_pop(maplist);
	}
}

static char *get_around(int center, int AB, int max, void **buf)
{
	assert(buf);

	if (center + AB < 0 || center + AB >= max) {
		return NULL;
	}

	return buf[center + AB];
}

static char *find_next(void)
{
	int i;
	char *p;
	for (i = dirndx + 1; (size_t)i < maplist->size; i++) {
		p = maplist->buf[i];
		if (strstr(p, DATA_MAPS_EXT)) {
			dirndx = (int)i;
			return p;
		}
	}

	// didn't find it yet
	for (i = 0; i < dirndx; i++) {
		p = maplist->buf[i];
		if (strstr(p, DATA_MAPS_EXT)) {
			dirndx = i;
			return p;
		}
	}

	if (dirndx == -1) {
		return "";
	}
	return maplist->buf[dirndx];
}

static char *find_prev(void)
{
	int i;
	char *p;
	for (i = dirndx - 1; i >= 0; i--) {
		p = maplist->buf[i];
		if (strstr(p, DATA_MAPS_EXT)) {
			dirndx = i;
			return p;
		}
	}

	for (i = maplist->size - 1; i > dirndx; i--) {
		p = maplist->buf[i];
		if (strstr(p, DATA_MAPS_EXT)) {
			dirndx = i;
			return p;
		}
	}

	if (dirndx == -1) {
		return "";
	}
	return maplist->buf[dirndx];
}

static void nextship(void *client)
{
	(void)client;
	if (shipndx + 1 >= SHIP_COUNT) {
		return;
	}

	shipndx += 1;
}

static void prevship(void *client)
{
	(void)client;
	if (shipndx - 1 < 0) {
		return;
	}

	shipndx -= 1;
}
