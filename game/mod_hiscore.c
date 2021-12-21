#include <rlu/rlu.h>
#include "scorefile.h"
#include "data.h"
#include "modules.h"

#define WIN_TEXT "- WINNER -"
#define LOSE_TEXT "- LOSER -"
#define NEWHI_TEXT "- NEW HIGHSCORE -"
#define SCORES_SHOWN_MAX 8

static void back_func(void *client);
static void save_func(void *client);

static button *back_button;
static button *save_button;
static bool new_highscore;
static bool won;
static char scorefilename[128];
static Score myscore;
static Scorefile *scorefile;

// yeah, didn't think this through...
static char *letterchoice[] = {
	"A", "B", "C", "D", "E", "F", "G", "H", "I",
	"J", "K", "L", "M", "N", "O", "P", "Q", "R",
	"S", "T", "U", "V", "W", "X", "Y", "Z", "0",
	"1", "2", "3", "4", "5", "6", "7", "8", "9",
};

static Itemlist *letter0;
static Itemlist *letter1;
static Itemlist *letter2;

void hiscore_init(void)
{
	Score *highest;

	new_highscore = false;

	// only if the scorefilename is valid
	if (scorefilename[0]) {
		scorefile = scorefile_open(scorefilename);
		if (!scorefile) {
			return;
		}

		highest = scorefile_get_hiscore(scorefile);

		// brand new attempt, highest score!
		if (!highest) {
			new_highscore = true;
		}
		// bested the old highest score!
		else if (highest->score < myscore.score) {
			new_highscore = true;
		}
	}

	ItemlistArgs args = {
		.fontsize = 24,
		.AB = 3,
		.offsetVert = (int)(GetScreenHeight() * 0.4f),
		.offsetHorz = GetScreenWidth() / 12,
		.offsetX = 0,
		.totalVert = GetScreenHeight(),
		.totalHorz = GetScreenWidth() * 2 / 12,
		.bgSelected = WHITE,
		.fgSelected = BLACK,
		.fgNotSelected = WHITE,
		.search = NULL,
	};
	letter0 = itemlist_new(&args);

	args.offsetHorz = GetScreenWidth() * 3 / 12;
	args.offsetX = GetScreenWidth() * 2 / 12;
	letter1 = itemlist_new(&args);

	args.offsetHorz = GetScreenWidth() * 5 / 12;
	args.offsetX = GetScreenWidth() * 4 / 12;
	letter2 = itemlist_new(&args);

	itemlist_set(letter0, letterchoice, ARRAY_SIZE(letterchoice));
	itemlist_set(letter1, letterchoice, ARRAY_SIZE(letterchoice));
	itemlist_set(letter2, letterchoice, ARRAY_SIZE(letterchoice));

	memset(&myscore, 0, sizeof(myscore));
	myscore.score = score_get();
	myscore.name[0] = *itemlist_get_selected(letter0);
	myscore.name[1] = *itemlist_get_selected(letter1);
	myscore.name[2] = *itemlist_get_selected(letter2);

	save_button = button_new("SAVE", save_func, NULL);
	back_button = button_new("BACK", back_func, NULL);
	component_set_size(save_button, 24, .4, .1);
	component_set_size(back_button, 24, .4, .1);
	component_set_pos(save_button, .75, .9);
	component_set_pos(back_button, .25, .9);
	component_set_color(save_button, WHITE, MYDARKGRAY);
	component_set_color(back_button, WHITE, MYDARKGRAY);
}

void hiscore_cleanup(void)
{
	new_highscore = false;
	won = false;
	(void)memset(scorefilename, 0, sizeof(scorefilename));

	if (scorefile) {
		scorefile_close(scorefile);
		scorefile = NULL;
	}

	component_del(back_button);
	component_del(save_button);
	back_button = NULL;
	save_button = NULL;

	itemlist_delete(letter0);
	letter0 = NULL;
	itemlist_delete(letter1);
	letter1 = NULL;
	itemlist_delete(letter2);
	letter2 = NULL;
}

void hiscore_update(void)
{
	int x;
	int y;
	int dir;
	char *p;
	char *letter = NULL;
	Itemlist *selected = NULL;

	component_update(back_button);
	component_update(save_button);

	// select column based on mouse position and update the pointers for that column
	x = GetMouseX();
	y = GetMouseY();
	if (0 <= x && x < GetScreenWidth() / 6) {
		selected = letter0;
		letter = &myscore.name[0];
	}
	else if (GetScreenWidth() / 6 <= x && x < GetScreenWidth() * 2 / 6) {
		selected = letter1;
		letter = &myscore.name[1];
	}
	else if (GetScreenWidth() * 2 / 6 <= x && x < GetScreenWidth() * 3 / 6) {
		selected = letter2;
		letter = &myscore.name[2];
	}

	if (letter && selected) {
		dir = GetMouseWheelMove();
		if (dir < 0 || rlu_input_key(0, RLU_KEY_DPAD_DOWN, RLU_PRESS_RELEASED)) {
			p = itemlist_next(selected);
			*letter = *p;
		}
		else if (dir > 0 || rlu_input_key(0, RLU_KEY_DPAD_UP, RLU_PRESS_RELEASED)) {
			p = itemlist_prev(selected);
			*letter = *p;
		}
	}
}

void hiscore_draw(void)
{
	Score *scores;
	int count;
	int i;
	char *p;

	itemlist_draw(letter0);
	itemlist_draw(letter1);
	itemlist_draw(letter2);

	if (won) {
		p = new_highscore ? NEWHI_TEXT : WIN_TEXT;
		DrawRectangle(0, GetScreenHeight() * 0.2, GetScreenWidth(), 40, WHITE);
		DrawText(p, GetScreenWidth() / 2 - MeasureText(p, 40) / 2, GetScreenHeight() * 0.2, 40, BLACK);
	}
	else {
		p = new_highscore ? NEWHI_TEXT : LOSE_TEXT;
		DrawRectangle(0, GetScreenHeight() * 0.2, GetScreenWidth(), 40, WHITE);
		DrawText(p, GetScreenWidth() / 2 - MeasureText(p, 40) / 2, GetScreenHeight() * 0.2, 40, BLACK);
	}

	component_draw(back_button);
	component_draw(save_button);

	DrawRectangle(GetScreenWidth() / 2, GetScreenHeight() * 0.4, GetScreenWidth() / 2, 24, WHITE);
	p = score_get_string();
	DrawText(p, GetScreenWidth() * 3 / 4 - MeasureText(p, 24) / 2, GetScreenHeight() * 0.4, 24, BLACK);

	// this just skips if count is zero
	scores = scorefile_get_scores(scorefile, &count);
	for (i = 0; i < count && i < SCORES_SHOWN_MAX; i++) {
		score_draw(&scores[i], 0.8, 0.5 + (0.05 * i), 24, WHITE);
	}
}

void hiscore_conf(struct hiscore_message *msg)
{
	char *p;
	assert(msg);

	won = !!msg->won;
	(void)snprintf(scorefilename, sizeof(scorefilename), "%s", msg->scorefile);
}

static void back_func(void *client)
{
	(void)client;

	// meant to be on top of game, so pop/switch to menu
	context_pop();
	context_switch("MG");
}

static void save_func(void *client)
{
	(void)client;

	if (scorefile) {
		scorefile_add_score(scorefile, myscore.name, myscore.score);
		(void)scorefile_save(scorefile);
	}

	// go back anyways if you choose to save
	back_func(NULL);
}
