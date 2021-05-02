#include <rlu/rlu.h>
#include "../modules.h"

#define PAUSE_TEXT "- PAUSE -"

static void resume(void *client);
static void menu(void *client);

static button *resume_button;
static button *menu_button;

void pause_init(void)
{
	resume_button = button_new("RESUME", resume, NULL);
	menu_button = button_new("MENU", menu, NULL);

	component_set_size(resume_button, 24, .4, .1);
	component_set_size(menu_button, 24, .4, .1);
	component_set_pos(resume_button, .5, .4);
	component_set_pos(menu_button, .5, .6);
	component_set_color(resume_button, BLACK, WHITE);
	component_set_color(menu_button, BLACK, WHITE);
}

void pause_cleanup(void)
{
	component_del(resume_button);
	component_del(menu_button);
}

void pause_update(void)
{
	component_update(resume_button);
	component_update(menu_button);

	if (IsKeyPressed(KEY_ENTER)) {
		context_pop();
	}
}

void pause_draw(void)
{
	DrawRectangle(GetScreenWidth() / 2 - MeasureText(PAUSE_TEXT, 40) / 2, GetScreenHeight() / 4, MeasureText(PAUSE_TEXT, 40), 40, BLACK);
	DrawText(PAUSE_TEXT, GetScreenWidth() / 2 - MeasureText(PAUSE_TEXT, 40) / 2, GetScreenHeight() / 4, 40, WHITE);
	component_draw(resume_button);
	component_draw(menu_button);
}

static void resume(void *client)
{
	context_pop();
}

static void menu(void *client)
{
	context_pop();
	context_switch("MG");
}
