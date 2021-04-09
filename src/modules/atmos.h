#ifndef MG_ATMOS_H
#define MG_ATMOS_H

/**
 * Util specifically for animated backdrops and music.
 * 
 * Can be used for any module, ie. menu, game, ... to add
 * atmosphere. Just change the assetdir as needed and cleanup
 * in between different atmosphere sessions.
 */

void atmos_init(char *assetdir);
void atmos_cleanup(void);
void atmos_update(void);
void atmos_draw(void);

#endif // MG_ATMOS_H
