#include <rlu/rlu.h>
#include "../modules.h"
#include "atmos.h"

static void lselect(void *client);
static void menu(void *client);
static void index_maps(void);
static void clear_maps(void);
static char *find_next(void);
static char *find_prev(void);

static button *select_button;
static button *menu_button;
static Image icon;
static char *mapdir = NULL;
static char **dirfiles = NULL;
static int dircount = -1;
static int dirndx = 0;
static char map_display[128];

void levelselect_init(void)
{
	select_button = button_new("SELECT", lselect, NULL);
	menu_button = button_new("MENU", menu, NULL);

	component_set_pos(select_button, GetScreenWidth() * 3 / 4 - 150, GetScreenHeight() - 150);
	component_set_pos(menu_button, GetScreenWidth() / 4 - 150, GetScreenHeight() - 150);
	component_set_size(select_button, 24, 300, 100);
	component_set_size(menu_button, 24, 300, 100);
	component_set_color(select_button, BLACK, WHITE);
	component_set_color(menu_button, BLACK, WHITE);
	atmos_init("assets");
	index_maps();

	icon = texture_man_img_load_or_default("assets/icon.png", 30, 30, BLUE);
	SetWindowIcon(icon);
}

void levelselect_cleanup(void)
{
	clear_maps();
	atmos_cleanup();
	component_del(select_button);
	component_del(menu_button);
	UnloadImage(icon);
}

void levelselect_update(void)
{
	atmos_update();
	component_update(select_button);
	component_update(menu_button);

	if (IsKeyDown(KEY_R)) {
		clear_maps();
		index_maps();
	}
	if (rlu_input_key(0, RLU_KEY_BUTTON_DOWN, RLU_PRESS_RELEASED)) {
		lselect(NULL);
	}

	if (rlu_input_key(0, RLU_KEY_DPAD_DOWN, RLU_PRESS_RELEASED)) {
		mapdir = find_prev();
	}
	else if (rlu_input_key(0, RLU_KEY_DPAD_UP, RLU_PRESS_RELEASED)) {
		mapdir = find_next();
	}

	if (mapdir) {
		(void)snprintf(map_display, sizeof(map_display), "%s", mapdir);
	}
	else {
		(void)snprintf(map_display, sizeof(map_display), "NULL");
	}
}

void levelselect_draw(void)
{
	atmos_draw();
	component_draw(select_button);
	component_draw(menu_button);

	DrawText(
		map_display,
		GetScreenWidth() / 2 - MeasureText(map_display, 70) / 2,
		GetScreenHeight() / 3,
		70,
		WHITE);
}

static void lselect(void *client)
{
	if (mapdir) {
		(void)snprintf(game_mapdir, sizeof(game_mapdir), "maps/%s", mapdir);
	}
	context_switch("GAME");
}

static void menu(void *client)
{
	context_switch("MG");
}

static void index_maps(void)
{
	if (DirectoryExists("maps")) {
		dirfiles = GetDirectoryFiles("maps", &dircount);
		dirndx = 0;
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
}

static char *find_next(void)
{
	int i;
	for (i = dirndx + 1; i < dircount; i++) {
		if (strstr(dirfiles[i], ".mg")) {
			dirndx = i;
			return dirfiles[i];
		}
	}

	// didn't find it yet
	for (i = 0; i < dirndx; i++) {
		if (strstr(dirfiles[i], ".mg")) {
			dirndx = i;
			return dirfiles[i];
		}
	}

	return dirfiles[dirndx];
}

static char *find_prev(void)
{
	int i;
	for (i = dirndx - 1; i >= 0; i--) {
		if (strstr(dirfiles[i], ".mg")) {
			dirndx = i;
			return dirfiles[i];
		}
	}

	for (i = dirndx + 1; i < dircount; i++) {
		if (strstr(dirfiles[i], ".mg")) {
			dirndx = i;
			return dirfiles[i];
		}
	}

	return dirfiles[dirndx];
}
