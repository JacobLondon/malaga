#ifndef L_THEATER_H_
#define L_THEATER_H_

/**
 * So the idea of this is to create a module
 * which has a C only interface for initializing
 * and updating, and a Lua only interface for
 * how to perform the scene/object/textureman/...
 * initialization.
 */

/**
 * C Interface
 */

int ltheater_init(const char *luafile, const char *mp3);
void ltheater_cleanup(void);
void ltheater_update(void);
void ltheater_draw(void);

#endif // L_THEATER_H_
