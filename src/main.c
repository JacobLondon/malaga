#include <rlu/rlu.h>
#include "modules.h"

static context_opt options = {
	.background={0, 0, 0, 255},
	.screen_width=1600,
	.screen_height=900,
	.fps=60,
	.title="Matt's Game",
	.use_audio=true,
	.volume=50,
};

static module_definition definitions[] = {
	DEFINE_MODULE("MG", mg_init, mg_cleanup, mg_update, mg_draw),
	DEFINE_MODULE("GAME", game_init, game_cleanup, game_update, game_draw),
	{NULL}
};

int main(void)
{
	context_init(&options, definitions);
	context_switch("GAME");
	context_loop();
	context_cleanup();

	return 0;
}
