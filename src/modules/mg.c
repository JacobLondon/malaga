#include <rlu/rlu.h>
#include "atmos.h"
#include "../modules.h"

#define MENU_TEXT "SPACE GAME"

static button *play_button;

static void play_func(void *client);
static Image icon;

void mg_init(void)
{
	play_button = button_new("PLAY", play_func, NULL);
	component_set_pos(play_button, GetScreenWidth() / 2 - 150, GetScreenHeight() / 2 - 50);
	component_set_size(play_button, 24, 300, 100);
	component_set_color(play_button, BLACK, WHITE);
	atmos_init("assets");
	icon = texture_man_img_load_or_default("assets/icon.png", 30, 30, BLUE);
	SetWindowIcon(icon);
}

void mg_cleanup(void)
{
	atmos_cleanup();
	component_del(play_button);
	UnloadImage(icon);
}

void mg_update(void)
{
	atmos_update();
	component_update(play_button);

	rlu_input_prescan(0);
	if (rlu_input_key(0, RLU_KEY_BUTTON_DOWN, RLU_PRESS_DOWN)) {
		context_switch("GAME");
	}
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
	context_switch("SELECT");
}
