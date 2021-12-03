#ifndef MG_DROP_H
#define MG_DROP_H

#include "bullet.h"

typedef struct item_drop {
	enum {
		DROP_NONE,
		DROP_LEVEL,
		DROP_BULLET,
		DROP_HEALTH,
		DROP_SPEED,
		DROP_COUNT
	} type;
	union {
		int none;
		int level_amount;
		int bullet_id;
		int health_amount;
		int speed_amount;
	} spec;
} item_drop;

const char *drop_to_string(item_drop *drop);
item_drop drop_random(void);
item_drop drop_level(int amount);
item_drop drop_bullet(int id);
item_drop drop_health(int amount);
item_drop drop_speed(int amount);

#endif // MG_DROP_H
