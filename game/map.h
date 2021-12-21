#ifndef MG_MAP_H
#define MG_MAP_H

/**
 * Load maps / encounters
 */

#include "mod_game.h"

struct MapHeader {
	char atmosphere[128]; // name of atmosphere
	char music[128]; // name of music file
};

/**
 * Load a map by filename. May be NULL. Only 1 map active at a time
 */
encounter **map_init(const char *mapfilename, struct MapHeader *out);

/**
 * Clean up the mess made by the encounter
 */
void map_cleanup(void);

#endif // MG_MAP_H
