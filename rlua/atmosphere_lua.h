#ifndef RLU_ATMOSPHERE_LUA_H_
#define RLU_ATMOSPHERE_LUA_H_

#include <lua/lua.h>

int luaopen_atmosphere(lua_State *L);

int luatmosphere_init(const char *lpath);
int luatmosphere_cleanup(void);

#endif // RLU_ATMOSPHERE_LUA_H_
