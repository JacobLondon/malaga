#include <rlu/rlu.h>
#include "scorefile.h"
#include "data.h"
#include "modules.h"

#define WIN_TEXT "- WINNER -"
#define LOSE_TEXT "- LOSER -"
#define NEWHI_TEXT "- NEW HIGHSCORE -"

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

void hiscore_init(void)
{
	Score *highest;

	new_highscore = false;

	memset(&myscore, 0, sizeof(myscore));
	myscore.score = score_get();
	myscore.name[0] = 'A';
	myscore.name[1] = 'A';
	myscore.name[2] = 'A';

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

	back_button = button_new("BACK", back_func, NULL);
	save_button = button_new("SAVE", save_func, NULL);
	component_set_size(back_button, 24, .4, .1);
	component_set_size(save_button, 24, .4, .1);
	component_set_pos(back_button, .75, .9);
	component_set_pos(save_button, .25, .9);
	component_set_color(back_button, WHITE, MYDARKGRAY);
	component_set_color(save_button, WHITE, MYDARKGRAY);
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
}

void hiscore_update(void)
{
	component_update(back_button);
	component_update(save_button);
}

void hiscore_draw(void)
{
	char *p;
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
