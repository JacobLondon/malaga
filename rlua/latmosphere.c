#include <assert.h>

#include <lua/lauxlib.h>
#include <lua/lualib.h>

#include <rlu/rlu.h>

#include "rlua.h"

static int l_atmosphere_init(lua_State *L);
static int l_atmosphere_cleanup(lua_State *L);

static atmosphere_t *atmosphere = NULL;

static const struct luaL_Reg atmoslib[] = {
	{"init", l_atmosphere_init},
	{NULL, NULL}
};

int luaopen_atmosphere(lua_State *L)
{
	luaL_newlib(L, atmoslib);
	return 1;
}

static int l_atmosphere_init(lua_State *L)
{

}

static int l_atmosphere_cleanup(lua_State *L)
{

}
