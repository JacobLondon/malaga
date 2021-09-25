#include <assert.h>

#include <lua/lauxlib.h>
#include <lua/lualib.h>

#include "atmosphere.h"
#include "atmosphere_lua.h"

static int mylua_openlib(lua_State *L, const char *name, const struct luaL_reg *reg);

static int l_init(lua_State *L);

static atmosphere_t *atmosphere = NULL;
static char *lpath = NULL;

static const struct luaL_reg atmospherelib[] = {
	{"init", l_init},
	{NULL, NULL}
};

int luaopen_atmosphere(lua_State *L)
{
	return mylua_openlib(L, "Atmosphere", atmospherelib);
}

int luatmosphere_init(const char *lpath)
{

}

int luatmosphere_cleanup(void)
{

}

static int mylua_openlib(lua_State *L, const char *name, const struct luaL_reg *reg)
{
	assert(name);
	assert(reg);

	lua_getglobal(L, name);
	if (lua_isnil(L, -1)) {
		lua_pop(L, 1);
		lua_newtable(L);
	}
	luaL_setfuncs(L, reg, 0);
	lua_setglobal(L, name);
	return 1;
}

static int l_init(lua_State *L)
{

}