#include <assert.h>

#include <lua/lauxlib.h>
#include <lua/lualib.h>

#include <rlu/rlu.h>

#include "rlua.h"
#include "ltheater.h"

static bool initialized = false;
static atmosphere_t *atm = NULL;
static scene_manager *sman = NULL;
static texture_manager *tman = NULL;
static lua_State *lvm = NULL;

/**
 * C Interface
 */

int ltheater_init(const char *luafile, const char *mp3)
{
	assert(initialized == false);
	assert(luafile != NULL);

	atm = atmosphere_new(mp3);
	sman = atmosphere_get_sceneman(atm);
	tman = atmosphere_get_textureman(tman);
	lvm = luaL_newstate();
	luaL_openlibs(lvm);

	if (luaL_loadfile(lvm, luafile) || lua_pcall(lvm, 0, 0, 0)) {
		msg_warning("Could not open / run %s. Stopping...", luafile);
		return 1;
	}

	return 0;
}

void ltheater_cleanup(void)
{
	assert(initialized == true);
	lua_close(lvm);
	atmosphere_del(atm);
	atm = NULL;
	sman = NULL;
	tman = NULL;
	lvm = NULL;
}

void ltheater_update(void)
{
	assert(initialized == true);
	atmosphere_update(atm);
}

void ltheater_draw(void)
{
	assert(initialized == true);
	atmosphere_draw(atm);
}

/**
 * Lua Interface
 */

static int ltheater_width(lua_State *L);
static int ltheater_height(lua_State *L);
static int ltheater_uniform(lua_State *L);
static int ltheater_anim(lua_State *L);

/**
 * Helpers
 */
static lua_Integer ptr_to_Lint(void *in);

static const struct luaL_Reg theaterlib[] = {
	{"width",   ltheater_width},
	{"height",  ltheater_height},
	{"uniform", ltheater_uniform},
	{"anim_new", ltheater_anim_new},
	{NULL, NULL}
};

int luaopen_theater(lua_State *L)
{
	luaL_newlib(L, theaterlib);
	return 1;
}

static int ltheater_width(lua_State *L)
{
	lua_pushinteger(L, GetScreenWidth());
	return 1;
}

static int ltheater_height(lua_State *L)
{
	lua_pushinteger(L, GetScreenHeight());
	return 1;
}

static int ltheater_uniform(lua_State *L)
{
	lua_pushnumber(L, rand_uniform());
	return 1;
}

/**
 * Open/load an animation from a png in the context asset directory
 * 
 * filename, str
 * anim width, 1 = no animation, otherwise width
 * anim height, 1 = no animation, otherwise height
 * def color, str
 * def width, pix
 * def height, pix
 */
static int ltheater_anim_new(lua_State *L)
{
	char path[128];
	const char *filename = luaL_checkstring(L, 1);
	lua_Integer anim_width = luaL_checkinteger(L, 2);
	lua_Integer anim_height = luaL_checkinteger(L, 3);
	const char *color = luaL_checkstring(L, 4);
	lua_Integer def_width = luaL_checkinteger(L, 5);
	lua_Integer def_height = luaL_checkinteger(L, 6);

	/** TODO */
	/*Texture2D gen = TEXTURE_GEN(def_width, def_height, color_lookup(color));
	snprintf(path, sizeof(path), "%s/%s", context_get_skindir(), filename);
	Texture2D *t = texture_man_load_or_default(tman, path, gen);
	anim *a = anim_man_load(scene_man_get_anim_man(sman), t, anim_width, anim_height);
	lua_Integer n = ptr_to_Lint(a);
	lua_pushinteger(L, n);
	*/
	return 1;
}

/**
 * Helpers
 */

static lua_Integer ptr_to_Lint(void *in)
{
	lua_Integer rv;
	assert(sizeof(lua_Integer) == sizeof(void *));
	memcpy(&rv, &in, sizeof(lua_Integer));
	return rv;
}
