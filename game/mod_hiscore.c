#include <rlu/rlu.h>
#include "scorefile.h"
#include "data.h"
#include "modules.h"

#define WIN_TEXT "- WINNER -"
#define LOSE_TEXT "- LOSER -"
#define NEWHI_TEXT "--! NEW HIGHSCORE !--"

static bool new_highscore;
static bool won;
static char scorefilename[128];
static score_t points;
static Scorefile *scorefile;

void hiscore_init(void)
{
	Score *highest;

	new_highscore = false;
	points = score_get();

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
		else if (highest->score < points) {
			new_highscore = true;
		}
	}
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
	points = 0;
}

void hiscore_update(void)
{
	
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
}

void hiscore_conf(struct hiscore_message *msg)
{
	char *p;
	assert(msg);

	won = !!msg->won;
	(void)snprintf(scorefilename, sizeof(scorefilename), "%s", msg->scorefile);
}
