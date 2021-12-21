#include <rlu/rlu.h>
#include "atmos.h"
#include "modules.h"

#define MENU_TEXT "MALAGA"

static button *play_button;
static button *skins_button;

static void play_func(void *client);
static void skins_func(void *client);

void mg_init(void)
{
	play_button = button_new("PLAY", play_func, NULL);
	skins_button = button_new("SKINS", skins_func, NULL);
	component_set_pos(play_button, .5 - .2, .65);
	component_set_pos(skins_button, .5 - .2, .8);

	component_set_size(play_button, 24, .4, .1);
	component_set_size(skins_button, 24, .4, .1);

	component_set_color(play_button, WHITE, MYDARKGRAY);
	component_set_color(skins_button, WHITE, MYDARKGRAY);
	atmos_init("Dark");
}

void mg_cleanup(void)
{
	atmos_cleanup();
	component_del(play_button);
	component_del(skins_button);
}

void mg_update(void)
{
	atmos_update();
	component_update(play_button);
	component_update(skins_button);
}

void mg_draw(void)
{
	atmos_draw();
	component_draw(play_button);
	component_draw(skins_button);
	DrawRectangle(0, GetScreenHeight() * 0.2, GetScreenWidth(), 40, WHITE);
	DrawText(MENU_TEXT, GetScreenWidth() / 2 - MeasureText(MENU_TEXT, 40) / 2, GetScreenHeight() * 0.2, 40, BLACK);
}

////////////////////////////////////////////////////////////////////////////////

static void play_func(void *client)
{
	(void)client;
	context_switch("SELECT");
}

static void skins_func(void *client)
{
	(void)client;
	context_switch("SKINS");
}
