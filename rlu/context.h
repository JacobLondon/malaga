#ifndef RLU_CONTEXT_H
#define RLU_CONTEXT_H

/**
 * Manage modules, loading and switching between them.
 */
#define MODULES_MAX 8 /* max number of modules to be active at once */

/** return 0 on success, anything else on error */
typedef void (* module_cb_init)(void);
typedef void (* module_cb_cleanup)(void);
typedef void (* module_cb_update)(void);
typedef void (* module_cb_draw)(void);

typedef struct module_tag {
	module_cb_update update;
	module_cb_draw draw;
	module_cb_init init;
	module_cb_cleanup cleanup;
} module;

typedef struct module_definition_tag {
	const char *name;
	module mod;
} module_definition;

#define DEFINE_MODULE(NAME, INIT, CLEANUP, UPDATE, DRAW) \
{ \
	.name=NAME, \
	.mod={ \
		.update=UPDATE, \
		.draw=DRAW, \
		.init=INIT, \
		.cleanup=CLEANUP \
	} \
}

#define DEFINE_MODULE_END() \
{ \
	.name=NULL, \
	.mod={ \
		.update=NULL, \
		.draw=NULL, \
		.init=NULL, \
		.cleanup=NULL, \
	} \
}

typedef struct context_opt_tag {
	Color background;
	int screen_width;
	int screen_height;
	int fps;
	char title[32];
	bool use_audio;
	float volume; // 0-100
	char icon[32]; // path to the icon
	char assetdir[32];
	char skindir[32]; // path to skin from assets, format: "dir/subdir" to leading or trailing items. Always relative
} context_opt;

/** ensure \a definitions is NULL terminated array. \a opt may be NULL to use defaults */
void context_init(context_opt *opt, module_definition definitions[]);
void context_cleanup(void);
void context_loop(void);
void context_loop_kill(void);

/** swap the top module in the buffer */
void context_switch(const char *module_name);
/** add a new top module to the buffer */
void context_push(const char *module_name);
/** remove the top module from the buffer */
void context_pop(void);

char *context_get_skindir(void);

#endif // RLU_CONTEXT_H
