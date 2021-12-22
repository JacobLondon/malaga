#include <rlu/rlu.h>
#include "data.h"
#include "modules.h"
#include "scorefile.h"

#define SCORES_TEXT "SCORES"
#define SCORES_SHOWN_MAX 11

static void menu_func(void *client);

static button *menu_button;
static Itemlist *scorefiles;
static char *selected_scorefilename;
static char *prev_scorefilename;
static Scorefile *selected_scorefile = NULL;

void scorelist_init(void)
{
	menu_button = button_new("MENU", menu_func, NULL);
	component_set_pos(menu_button, 0.3, .85);
	component_set_size(menu_button, 24, .4, .1);
	component_set_color(menu_button, WHITE, MYDARKGRAY);

	ItemlistArgs args = {
		.fontsize = 24,
		.AB = 6,
		.offsetVert = (int)(GetScreenHeight() * 0.5f),
		.offsetHorz = GetScreenWidth() / 8,
		.offsetX = 0,
		.totalVert = GetScreenHeight(),
		.totalHorz = GetScreenWidth() / 2,
		.bgSelected = WHITE,
		.fgSelected = BLACK,
		.fgNotSelected = WHITE,
		.search = DATA_SCORE_EXT,
	};
	scorefiles = itemlist_new(&args);
	itemlist_set_directory(scorefiles, DATA_MAPS_DIR);
	selected_scorefilename = itemlist_get_selected(scorefiles);
	prev_scorefilename = NULL;

	atmos_init("PlanetField");
}

void scorelist_cleanup(void)
{
	atmos_cleanup();
	component_del(menu_button);
	itemlist_delete(scorefiles);
	if (selected_scorefile) {
		scorefile_close(selected_scorefile);
		selected_scorefile = NULL;
	}
}

void scorelist_update(void)
{
	int dir;

	atmos_update();
	component_update(menu_button);

	dir = GetMouseWheelMove();
	if (dir < 0 || rlu_input_key(0, RLU_KEY_DPAD_DOWN, RLU_PRESS_RELEASED)) {
		selected_scorefilename = itemlist_next(scorefiles);
	}
	else if (dir > 0 || rlu_input_key(0, RLU_KEY_DPAD_UP, RLU_PRESS_RELEASED)) {
		selected_scorefilename = itemlist_prev(scorefiles);
	}

	// the pointer changed
	if ((void *)prev_scorefilename != (void *)selected_scorefilename) {
		if (selected_scorefile) {
			scorefile_close(selected_scorefile);
		}
		selected_scorefile = scorefile_open((const char *)tempbuf("%s/%s", DATA_MAPS_DIR, selected_scorefilename));
		prev_scorefilename = selected_scorefilename;
	}
}

void scorelist_draw(void)
{
	size_t i;
	size_t count;
	Score *s;

	atmos_draw();
	itemlist_draw(scorefiles);
	component_draw(menu_button);

	DrawRectangle(0, GetScreenHeight() * 0.2, GetScreenWidth(), 40, WHITE);
	DrawText(SCORES_TEXT, GetScreenWidth() / 2 - MeasureText(SCORES_TEXT, 40) / 2, GetScreenHeight() * 0.2, 40, BLACK);

	if (selected_scorefile) {
		s = scorefile_get_scores(selected_scorefile, &count);
		for (i = 0; i < count && i < SCORES_SHOWN_MAX; i++) {
			score_draw(&s[i], 0.75, 0.3 + (0.05 * i), 24, WHITE);
		}
	}
}

static void menu_func(void *client)
{
	(void)client;
	context_switch("MG");
}
