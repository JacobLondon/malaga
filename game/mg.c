#include <rlu/rlu.h>
#include "atmos.h"
#include "modules.h"

#define MENU_TEXT "MALAGA"

static button *play_button;

static void play_func(void *client);

void mg_init(void)
{
	play_button = button_new("PLAY", play_func, NULL);
	component_set_pos(play_button, .5 - .2, .75);
	component_set_size(play_button, 24, .4, .1);
	component_set_color(play_button, WHITE, MYDARKGRAY);
	atmos_init(NULL);
}

void mg_cleanup(void)
{
	atmos_cleanup();
	component_del(play_button);
}

void mg_update(void)
{
	atmos_update();
	component_update(play_button);
}

void mg_draw(void)
{
	atmos_draw();
	component_draw(play_button);
	DrawText(MENU_TEXT, GetScreenWidth() / 2 - MeasureText(MENU_TEXT, 40) / 2, GetScreenHeight() / 4, 40, WHITE);
}

////////////////////////////////////////////////////////////////////////////////

static void play_func(void *client)
{
	(void)client;
	context_switch("SELECT");
}
