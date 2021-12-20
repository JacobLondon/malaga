/**
 * So here's the idea.
 * 
 * A context has a bunch of module definitions, where it
 * can look up modules by name. Modules can be loaded on
 * top of each other to allow for things such as overlays
 * or a menu with a different control set up or something.
 * 
 * ie.
 * Module 0 = Mouse invisible, game control
 * Esc pressed
 * Module 1 loaded
 * Module 1 = Mouse visible, pause menu shown over game
 * Esc pressed again
 * Module 1 unloaded, restore mouse invisibility
 */

#include "rlu.h"

#define PARAMETER_FILE "rlu.conf"

static void null_init(void);
static void null_cleanup(void);
static void null_update(void);
static void null_draw(void);

static module *lookup_module(const char *name);
static void module_operations(void);

static void init_module_func(void *mod);
static void cleanup_module_func(void *mod);
static void update_module_func(void *mod);
static void draw_module_func(void *mod);

static Image icon;
static char iconPath[128];
static bool initialized = false;
static bool done = false;
static module_definition *modules = NULL;

static module null_module = {
	.update = null_update,
	.draw = null_draw,
	.init = null_init,
	.cleanup = null_cleanup,
};
static module *loaded_modules[MODULES_MAX] = {NULL};
static module *on_deck = &null_module;
static int popcount = 0; // count the number of pops to perform if more than 1 POP op per frame
static svec module_vec;
static enum {
	XOP_NONE,
	XOP_PUSH,
	XOP_SWAP,
} operation = XOP_NONE;

static context_opt options;
static context_opt defaultOptions = {
	.background={0, 0, 0, 255},
	.screen_width=1600,
	.screen_height=900,
	.fps=60,
	.title="RLU",
	.use_audio=true,
	.volume=50,
};

static param paramList[] = {
	DEFINE_PARAM(PARAM_TYPE_COLOR, background, options, defaultOptions),
	DEFINE_PARAM(PARAM_TYPE_INT, screen_width, options, defaultOptions),
	DEFINE_PARAM(PARAM_TYPE_INT, screen_height, options, defaultOptions),
	DEFINE_PARAM(PARAM_TYPE_INT, fps, options, defaultOptions),
	DEFINE_PARAM(PARAM_TYPE_STRING, title, options, defaultOptions),
	DEFINE_PARAM(PARAM_TYPE_BOOL, use_audio, options, defaultOptions),
	DEFINE_PARAM(PARAM_TYPE_FLOAT, volume, options, defaultOptions),
	DEFINE_PARAM(PARAM_TYPE_STRING, icon, options, defaultOptions),
	DEFINE_PARAM(PARAM_TYPE_STRING, assetdir, options, defaultOptions),
	DEFINE_PARAM(PARAM_TYPE_STRING, skindir, options, defaultOptions),
	DEFINE_PARAM_END(),
};

static void null_init(void)
{

}

static void null_cleanup(void)
{

}

static void null_update(void)
{

}

static void null_draw(void)
{

}

static module *lookup_module(const char *name)
{
	assert(name);
	for (size_t i = 0; modules[i].name != NULL; i++) {
		if (streq(name, modules[i].name)) {
			return &modules[i].mod;
		}
	}
	return NULL;
}

/**
 * Perform module level operations given by 'operation'
 */
static void module_operations(void)
{
	module *mod;

	// pop is special operation, may always occur regardless of op
	if (popcount > 0) {
		for ( ; popcount > 0; popcount--) {
			mod = svec_tail(&module_vec);
			if (mod) {
				mod->cleanup();
			}
			svec_pop(&module_vec);
		}
	}

	if (operation == XOP_NONE) {
		return;
	}

	switch (operation) {
	case XOP_PUSH:
		svec_push(&module_vec, on_deck);
		on_deck->init();
		operation = XOP_NONE;
		break;
	case XOP_SWAP:
		mod = svec_tail(&module_vec);
		if (!mod) {
			break;
		}
		mod->cleanup();
		svec_pop(&module_vec);
		on_deck->init();
		svec_push(&module_vec, on_deck);
		operation = XOP_NONE;
		break;
	default:
		operation = XOP_NONE;
		break;
	}
}

static void init_module_func(void *mod)
{
	module *m = (module *)mod;
	m->init();
}

static void cleanup_module_func(void *mod)
{
	module *m = (module *)mod;
	m->cleanup();
}

static void update_module_func(void *mod)
{
	module *m = (module *)mod;
	m->update();
}

static void draw_module_func(void *mod)
{
	module *m = (module *)mod;
	m->draw();
}

void context_init(context_opt *opt, module_definition definitions[])
{
	assert(opt);
	assert(definitions);
	assert(initialized == false);

	// make the user specified the default options if no config file is there
	(void)memcpy(&defaultOptions, opt ? opt : &defaultOptions, sizeof(defaultOptions));

	// this reads config and/or copied defaultOptions to options
	param_read(PARAMETER_FILE, paramList);

	// load the default empty module so there is always 1 in there. The first module must be swapped in
	modules = definitions;
	svec_init(&module_vec, (void **)loaded_modules, ARRAY_SIZE(loaded_modules));
	svec_push(&module_vec, &null_module);
	initialized = true;
	done = false;

	// raylib init icon, audio, ...
	InitWindow(options.screen_width, options.screen_height, options.title);
	SetTargetFPS(options.fps);

	if (options.icon[0] != 0) {
		(void)snprintf(iconPath, sizeof(iconPath), "%s/%s", context_get_skindir(), options.icon);
		icon = LoadImage(iconPath);
		SetWindowIcon(icon);
	}

	if (options.use_audio) {
		InitAudioDevice();
		while (!IsAudioDeviceReady())
			;
		music_man_set_volume(options.volume);
	}

	//SetWindowPosition(options.screen_height / 3, options.screen_width / 3);
}

void context_cleanup(void)
{
	assert(initialized == true);
	initialized = false;
	done = false;

	if (options.use_audio) {
		CloseAudioDevice();
	}
	if (options.icon[0] != 0) {
		UnloadImage(icon);
	}
	CloseWindow();

	param_write(PARAMETER_FILE, paramList);
}

void context_loop(void)
{
	// initialize, run, then clean up
	svec_map(&module_vec, init_module_func);

	while (!WindowShouldClose() && !done) {
		module_operations();
		update_module_func(svec_tail(&module_vec));
		BeginDrawing();
		ClearBackground(options.background);
		svec_map(&module_vec, draw_module_func);
		EndDrawing();
	}

	svec_map(&module_vec, cleanup_module_func);
}

void context_loop_kill(void)
{
	assert(initialized);
	done = true;
}

void context_switch(const char *module_name)
{
	module *mod;
	assert(module_name);

	mod = lookup_module(module_name);
	msg_assert(mod, "Failed to find module '%s'", module_name);
	on_deck = mod;
	operation = XOP_SWAP;
}

void context_push(const char *module_name)
{
	module *mod;
	assert(module_name);

	mod = lookup_module(module_name);
	msg_assert(mod, "Failed to find module '%s'", module_name);
	on_deck = mod;
	operation = XOP_PUSH;
}

void context_pop(void)
{
	popcount++;
}

char *context_get_skindir(void)
{
	static char buf[96];
	assert(initialized);
	(void)snprintf(buf, sizeof(buf), "%s/%s", options.assetdir, options.skindir);
	return buf;
}
