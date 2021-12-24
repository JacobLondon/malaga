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

static button *select_button;
static button *menu_button;
static button *endless_button;
static button *damage_button;
static button *invinc_button;
static button *nextship_button;
static button *prevship_button;

static char *mapdir = "";
static bool mapdirexists = false;
static bool endless_mode = false;
static bool trouble_mode = false;
static bool invinc_mode = false;

static texture_manager texman;
static Texture2D *shiptexas[SHIP_COUNT];
static int shipndx = 0;

static Itemlist *allmaps;

void levelselect_init(void)
{
	size_t i;

	select_button = button_new("SELECT", lselect, NULL);
	menu_button = button_new("MENU", menu, NULL);
	endless_button = button_new("ENDLESS", tendless, NULL);
	damage_button = button_new("2x TROUBLE", tdamage, NULL);
	invinc_button = button_new("INVINCIBLE", tinvinc, NULL);
	nextship_button = button_new("Next Ship", nextship, NULL);
	prevship_button = button_new("Previous Ship", prevship, NULL);

	shipndx = 0;
	texture_man_new(&texman);
	for (i = 0; i < ARRAY_SIZE(shiptexas); i++) {
		shiptexas[i] = texture_man_load_or_default(
			&texman,
			tempbuf("%s/%s%zu.png", context_get_skindir(), "default_player", i),
			50, 50, BLUE);
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

	atmos_init("Sunspot");

	ItemlistArgs args = {
		.fontsize = 40,
		.AB = AROUND_SELECTED,
		.offsetVert = (int)(GetScreenHeight() * 0.3f),
		.offsetHorz = GetScreenWidth() / 10,
		.offsetX = 0,
		.totalVert = GetScreenHeight(),
		.totalHorz = GetScreenWidth(),
		.bgSelected = WHITE,
		.fgSelected = BLACK,
		.fgNotSelected = WHITE,
		.search = DATA_MAPS_EXT,
	};
	allmaps = itemlist_new(&args);
	index_maps();
	mapdir = itemlist_get_selected(allmaps);
}

void levelselect_cleanup(void)
{
	if (allmaps) {
		itemlist_delete(allmaps);
		allmaps = NULL;
	}

	texture_man_del(&texman);
	memset(shiptexas, 0, sizeof(shiptexas));
	atmos_cleanup();
	component_del(select_button);
	component_del(menu_button);
	component_del(endless_button);
	component_del(damage_button);
	component_del(invinc_button);
	component_del(prevship_button);
	component_del(nextship_button);
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
			mapdir = itemlist_next(allmaps);
		}
		else if (dir > 0 || rlu_input_key(0, RLU_KEY_DPAD_UP, RLU_PRESS_RELEASED)) {
			mapdir = itemlist_prev(allmaps);
		}
	}
}

void levelselect_draw(void)
{
	atmos_draw();

	// draw around
	itemlist_draw(allmaps);

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
		memset(&msg, 0, sizeof(msg));

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
	mapdirexists = itemlist_set_directory(allmaps, DATA_MAPS_DIR);
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
