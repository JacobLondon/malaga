#ifndef MG_DROP_MANAGER_H
#define MG_DROP_MANAGER_H

#include "game.h"
#include "drop.h"

void drop_manager_init(void);

// drop a random item on the screen
void drop_manager_dropit(enemy_data *ed);

// attempt to pick up an item, return NONE if nothing is there
item_drop drop_manager_pickup(player_data *pd);
void drop_manager_update(void);
void drop_manager_draw(void);
void drop_manager_cleanup(void);

#endif // MG_DROP_MANAGER_H
