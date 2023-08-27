#include <rlu/rlu.h>
#include "atmos.h"
#include "modules.h"
#include "data.h"

static void reload_func(void *client);
static void menu_func(void *client);
static void save_skin(void);

static button *reload_button;
static button *menu_button;
static char *skin = "";
static bool skinsexist = false;
static Itemlist *allskins;

void skins_init(void)
{
	char *p;

	reload_button = button_new("LOAD", reload_func, NULL);
	menu_button = button_new("MENU", menu_func, NULL);

	component_set_size(reload_button, 24, .4, .1);
	component_set_size(menu_button, 24, .4, .1);

	component_set_pos(reload_button, .75, .9);
	component_set_pos(menu_button, .25, .9);

	component_set_color(reload_button, WHITE, MYDARKGRAY);
	component_set_color(menu_button, WHITE, MYDARKGRAY);

	atmos_init("Flyby");

	ItemlistArgs args = {
		.fontsize = 40,
		.AB = 5,
		.offsetVert = (int)(GetScreenHeight() * 0.3f),
		.offsetHorz = GetScreenWidth() / 10,
		.offsetX = 0,
		.totalVert = GetScreenHeight(),
		.totalHorz = GetScreenWidth(),
		.bgSelected = WHITE,
		.fgSelected = BLACK,
		.fgNotSelected = WHITE,
		.search = NULL,
	};
	allskins = itemlist_new(&args);
	skinsexist = itemlist_set_directory(allskins, DATA_SKINS_DIR);

	p = (char *)mybasename((const char *)context_get_skindir());
	(void)itemlist_try_select(allskins, p);
}

void skins_cleanup(void)
{
	itemlist_delete(allskins);
	atmos_cleanup();
	component_del(reload_button);
	component_del(menu_button);
}

void skins_update(void)
{
	atmos_update();
	component_update(reload_button);
	component_update(menu_button);

	if (skinsexist) {
		int dir = GetMouseWheelMove();
		if (dir < 0 || rlu_input_key(0, RLU_KEY_DPAD_DOWN, RLU_PRESS_RELEASED)) {
			skin = itemlist_next(allskins);
			save_skin();
		}
		else if (dir > 0 || rlu_input_key(0, RLU_KEY_DPAD_UP, RLU_PRESS_RELEASED)) {
			skin = itemlist_prev(allskins);
			save_skin();
		}
	}
}

void skins_draw(void)
{
	atmos_draw();
	itemlist_draw(allskins);
	component_draw(reload_button);
	component_draw(menu_button);
}

static void reload_func(void *client)
{
	(void)client;
	context_switch("SKINS");
}

static void menu_func(void *client)
{
	(void)client;
	context_switch("MG");
}

static void save_skin(void)
{
	// itemlist isn't empty and skin is valid
	if (skin && strlen(skin) > 0) {
		context_set_param("skindir", (void *)mybasename(skin));
	}
}
