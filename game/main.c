#include <rlu/rlu.h>
#include "modules.h"

static context_opt options = {
	.background={0, 0, 0, 255},
	.screen_width=500,
	.screen_height=850,
	.fps=60,
	.title="Malaga",
	.use_audio=true,
	.volume=50,
	.icon="icon.png",
	.assetdir="assets",
};

static module_definition definitions[] = {
	DEFINE_MODULE("MG", mg_init, mg_cleanup, mg_update, mg_draw),
	DEFINE_MODULE("GAME", game_init, game_cleanup, game_update, game_draw),
	DEFINE_MODULE("PAUSE", pause_init, pause_cleanup, pause_update, pause_draw),
	DEFINE_MODULE("SELECT", levelselect_init, levelselect_cleanup, levelselect_update, levelselect_draw),
	DEFINE_MODULE_END(),
};

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	context_init(&options, definitions);
	context_switch("MG");
	context_loop();
	context_cleanup();

	return 0;
}
