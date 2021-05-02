#include <rlu/rlu.h>
#include "../modules.h"
#include "atmos.h"
#include "data.h"

#define AROUND_SELECTED 6

static void lselect(void *client);
static void menu(void *client);
static void index_maps(void);
static void clear_maps(void);
static char *get_around(int center, int AB, size_t max, void **buf);
static char *find_next(void);
static char *find_prev(void);

static button *select_button;
static button *menu_button;
static char *mapdir = "";
static char **dirfiles = NULL;
static int dircount = -1;
static int dirndx = 0;
static struct parray *maplist; // point into dir files with the correct files
static bool mapdirexists = false;

void levelselect_init(void)
{
	select_button = button_new("SELECT", lselect, NULL);
	menu_button = button_new("MENU", menu, NULL);
	maplist = parray_new(NULL);
	assert(maplist);

	component_set_pos(select_button, GetScreenWidth() * 3 / 4 - 150, GetScreenHeight() - 150);
	component_set_pos(menu_button, GetScreenWidth() / 4 - 150, GetScreenHeight() - 150);
	component_set_size(select_button, 24, 300, 100);
	component_set_size(menu_button, 24, 300, 100);
	component_set_color(select_button, BLACK, WHITE);
	component_set_color(menu_button, BLACK, WHITE);
	atmos_init();
	index_maps();
}

void levelselect_cleanup(void)
{
	clear_maps();
	atmos_cleanup();
	component_del(select_button);
	component_del(menu_button);
	parray_free(maplist);
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

	if (mapdirexists) {
		if (rlu_input_key(0, RLU_KEY_BUTTON_DOWN, RLU_PRESS_RELEASED)) {
			if (strlen(mapdir) > 0) {
				lselect(NULL);
			}
		}

		if (rlu_input_key(0, RLU_KEY_DPAD_DOWN, RLU_PRESS_RELEASED)) {
			mapdir = find_next();
		}
		else if (rlu_input_key(0, RLU_KEY_DPAD_UP, RLU_PRESS_RELEASED)) {
			mapdir = find_prev();
		}
	}
}

void levelselect_draw(void)
{
	const int AB = AROUND_SELECTED;
	const int height = AB * AB;
	const int fontsize = 20;
	const int offset = fontsize * AB;
	int i;

	atmos_draw();
	component_draw(select_button);
	component_draw(menu_button);

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
					.a=(int)(255.0 * 1.0 / (fabs(i) / (double)AB))
				};
				DrawText(
					get_around(dirndx, i, maplist->size, maplist->buf),
					GetScreenWidth() / 10,
					GetScreenHeight() / height * i + offset,
					fontsize,
					c);
			}
		}
	}
}

static void lselect(void *client)
{
	if (mapdirexists) {
		if (strlen(mapdir) > 0) {
			(void)snprintf(game_mapdir, sizeof(game_mapdir), "%s/%s", DATA_MAPS_DIR, mapdir);
		}
		context_switch("GAME");
	}
}

static void menu(void *client)
{
	context_switch("MG");
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

static char *get_around(int center, int AB, size_t max, void **buf)
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
	for (i = dirndx + 1; i < maplist->size; i++) {
		p = maplist->buf[i];
		if (strstr(p, DATA_MAPS_EXT)) {
			dirndx = i;
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
