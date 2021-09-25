#include <assert.h>
#include <rlu/util.h>
#include "drop.h"

typedef item_drop (*drop_func)(int);

const char *drop_to_string(item_drop *drop)
{
	static char buf[128];
	assert(drop);
	switch (drop->type) {
	case DROP_LEVEL:
		snprintf(buf, sizeof(buf), "%+d Level", drop->spec.level_amount);
		break;
	case DROP_BULLET:
		snprintf(buf, sizeof(buf), "%s", bullet_lookup_id(drop->spec.bullet_id));
		break;
	case DROP_HEALTH:
		snprintf(buf, sizeof(buf), "%+d HP", drop->spec.health_amount);
		break;
	case DROP_SPEED:
		snprintf(buf, sizeof(buf), "%+d Speed", drop->spec.speed_amount);
		break;
	default:
		msg_assert(0, "Invalid path");
	}
	return buf;
}

item_drop drop_random(void)
{
	item_drop rv;
	rv.type = rand_range(0, DROP_COUNT);
	switch(rv.type) {
	case DROP_LEVEL:
		rv.spec.level_amount = 1;
		break;
	case DROP_BULLET:
		rv.spec.bullet_id = rand_range(0, BULLET_PLAYER_COUNT);
		break;
	case DROP_HEALTH:
		rv.spec.health_amount = 2;
		break;
	case DROP_SPEED:
		rv.spec.speed_amount = 10;
		break;
	default:
		break;
	}
	return rv;
}

item_drop drop_level(int amount)
{
	item_drop rv;
	rv.type = DROP_LEVEL;
	rv.spec.level_amount = amount;
	return rv;
}

item_drop drop_bullet(int id)
{
	item_drop rv;
	rv.type = DROP_BULLET;
	rv.spec.bullet_id = id;
	return rv;
}

item_drop drop_health(int amount)
{
	item_drop rv;
	rv.type = DROP_HEALTH;
	rv.spec.health_amount = amount;
	return rv;
}

item_drop drop_speed(int amount)
{
	item_drop rv;
	rv.type = DROP_SPEED;
	rv.spec.speed_amount = amount;
	return rv;
}
