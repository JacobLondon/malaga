#ifndef RLUA_H_
#define RLUA_H_

#include <lua/lua.h>



#define RLUA_ATMOSLIBNAME "atmos"
int luaopen_atmosphere(lua_State *L);

#define RLUA_THEATER "theater"
int luaopen_theater(lua_State *L);

#endif // RLUA_H_
