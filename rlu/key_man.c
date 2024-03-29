#include "rlu.h"

void key_man_new(key_manager *out, key_definition *definitions)
{
	assert(out);
	assert(definitions != NULL);

	memset(out, 0, sizeof(*out));
	out->defs = definitions;
}

void key_man_del(key_manager *self)
{
	int i;
	assert(self);

	for (i = 0; i < KEY_OBJECTS_MAX; i++) {
		if (self->key_objects[i]) {
			ko_del(self->key_objects[i]);
		}
	}
	memset(self->key_objects, 0, sizeof(self->key_objects));
}

void key_man_load(key_manager *self, char **names)
{
	key_definition *d;
	int i;
	ko *tmp;
	assert(self);
	assert(names);

	for (i = 0; names[i] != NULL; i++) {
		for (d = self->defs; d->name; d++) {
			if (streq(names[i], d->name)) {
				tmp = ko_new();
				d->load(tmp);
				self->key_objects[i] = tmp;
				break;
			}
		}

		/* should not get to the end of defs */
		msg_assert(d->name != NULL, "Key definition not found");
	}
}

void key_man_update(key_manager *self)
{
	int i;
	
	// update all key objects that exist
	for (i = 0; i < KEY_OBJECTS_MAX; i++) {
		if (self->key_objects[i]) {
			ko_update(self->key_objects[i]);
		}
	}
}

/*****************************************************************************
 * 
 * Key Object Callback Definitions
 * 
 * No animation manager. This is because key objects taken ownership
 * of their animations, so passing an owner pointer with anim_new is
 * correct, while anim_man_load or something is wrong, as animation managers
 * own, and do not transfer ownership.
 * 
 ****************************************************************************/

/* Example:
static key_definition defs[] = {
	{ "Missile", load_cb_laser },
	{ NULL, NULL }
};

ko_man_init(defs);

static void ko_cb_laser_0(ko *self, so *object)
{
	ko_set_pos(self, GetScreenWidth(), GetMouseY());
	// wait until the key press, then go!
	if (IsKeyPressed(KEY_J)) {
		ko_set_key(self, true);
	}
}

static void ko_cb_laser_1(ko *self, so *object)
{
	const Vector2 p = so_get_pos(object);
	const int x = GetMouseX();

	// once to the right of, close enough, next state
	if (p.x < x) {
		music_man_play_sound("Thump");
		ko_set_key(self, true);
	}
}

static void ko_cb_laser_2(ko *self, so *object)
{
	const int w = anim_get_width(so_get_anim(object));
	const int h = anim_get_height(so_get_anim(object));
	so_set_pos(object, GetMouseX() - w / 2, GetMouseY() - h / 2);
	// just animate and stop near the end
	if (ko_get_frame(self) >= ko_get_max_frames(self)) {
		ko_set_key(self, true);
	}
}

static void load_cb_laser(ko *self)
{
	so *tmp;
	Texture2D *missile = texture_man_load(ASSET_DIRECTORY "/missile1.png");
	Texture2D *explosion = texture_man_load(ASSET_DIRECTORY "/explosion3.png");
	music_man_load_sound("Thump");
	
	tmp = so_new_own(anim_new(missile, 1, 1));
	so_set_pos(tmp, GetScreenWidth(), GetScreenHeight() / 2);
	so_newmov(tmp, so_cb_left, 30, NULL);
	so_newmov(tmp, so_cb_trk_vrt, 2, NULL);
	ko_add(self, tmp, ko_cb_laser_0, NULL); // spawn on press
	ko_add_rate(self, tmp, ko_cb_laser_1, NULL, 8); // move to mouse
	tmp = so_new_own(anim_new(explosion, 4, 4));
	ko_add_rate(self, tmp, ko_cb_laser_2, NULL, 20); // explode
}
*/
