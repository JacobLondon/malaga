#include <rlu/rlu.h>

#define STATES_MAX 6 /* max number of states for a ko to manage */

struct ko_tag {
	so *objects[STATES_MAX]; /* owns all of its screen objects */
	ko_cb state_cbs[STATES_MAX];

	/* So this is gonna be a weird one.
	 * These keys are the state variables for
	 * controlling objects. Now when
	 * ko add state is called, if the
	 * key variable passed is NULL,
	 * then the ko will automatically control
	 * that bool. If a key is passed, then the ko
	 * will have to wait on what is passed before
	 * progressing to the next state.
	 * 
	 * If I own, then NULL (void *)0 means false
	 * and (void *)1 means true
	 */
	bool *keys[STATES_MAX];  /* Own some, not all. If false, they state goes,
	                            if true, the state goes to the next*/
	bool mykeys[STATES_MAX]; /* true = I own, false = global bool */
	float rates[STATES_MAX]; /* Number of frames to animate per second per state */
	float oof; /* Time of the current animation. If this increases past the current
	              rate, then use the next frame of the animation */
	int frame; /* the frame number the animation is in */
	int state;
	int laststate; // index
};

ko *ko_new(void)
{
	ko *self = allocate(sizeof(ko));
	assert(self);
	memset(self, 0, sizeof(ko));
	return self;
}

void ko_del(ko *self)
{
	int i;
	assert(self);
	for (i = 0; i < self->laststate; i++) {
		if (self->objects[i]) {
			so_del(self->objects[i]);
		}
	}
	memset(self, 0, sizeof(ko));
	dealloc(self);
}

void ko_reset(ko *self)
{
	int i;
	assert(self);
	self->frame = 0;
	self->state = 0;
	for (i = 0; i < self->laststate; i++) {
		if (self->objects[i]) {
			anim_reset(so_get_anim(self->objects[i]));
		}

		// I own
		if (self->mykeys[i]) {
			self->keys[i] = NULL;
		}
		// I don't own, global
		else {
			// should already be valid, objects[i] exists
			*self->keys[i] = false;
		}
	}
}

ko *ko_copy(ko *self)
{
	assert(self);
	ko *other = allocate(sizeof(ko));
	assert(other);
	memcpy(other, self, sizeof(ko));

	// visual defaults, internals copied above
	ko_reset(other);
	return other;
}

void ko_add_rate(ko *self, so *object, ko_cb cb_state, bool *key, float animation_rate)
{
	assert(self);
	assert(cb_state);


	self->objects[self->laststate] = object;
	self->state_cbs[self->laststate] = cb_state;
	if (key) {
		self->keys[self->laststate] = key;
		self->mykeys[self->laststate] = false;
	}
	else {
		self->mykeys[self->laststate] = true;
	}
	self->rates[self->laststate] = animation_rate;

	self->laststate++;
	msg_assert(self->laststate < STATES_MAX, "Too many object keys: %d", self->laststate);
}

bool ko_update(ko *self)
{
	so *object;
	ko_cb cb;
	assert(self);

	object = self->objects[self->state];
	cb = self->state_cbs[self->state];
	cb(self, object);

	// done with state, go to next one
	if (ko_get_key(self)) {
		self->frame = 0;
		self->oof = 0.0f;
		self->state++;

		object = self->objects[self->state];
		if (self->state >= self->laststate) {
			ko_reset(self);
			return true;
		}
	}

	// update object if necessary
	if (object) {
		so_draw(object);
		so_update(object);
	}

	if (self->oof > 1.0f / self->rates[self->state]) {
		if (object) {
			anim_update(so_get_anim(object));
		}
		self->oof = 0.0f;
		self->frame++;
	}
	self->oof += GetFrameTime();
	return false;
}

bool ko_get_key(ko *self)
{
	assert(self);

	// I own
	if (self->mykeys[self->state]) {
		return (bool)self->keys[self->state]; // yeah it's weird but right
	}
	// I don't own, global
	else {
		assert(self->keys[self->state]); // careful
		return *self->keys[self->state];
	}
}

void ko_set_key(ko *self, bool key)
{
	assert(self);

	// I own
	if (self->mykeys[self->state]) {
		self->keys[self->state] = (void *)key;
	}
	// I don't own, global
	else {
		assert(self->keys[self->state]); // careful
		*self->keys[self->state] = key;
	}
}

float ko_get_frame(ko *self)
{
	assert(self);
	return self->frame;
}

int ko_get_max_frames(ko *self)
{
	assert(self);
	if (self->objects[self->state]) {
		return anim_get_frames(so_get_anim(self->objects[self->state]));
	}
	return 0;
}

void ko_set_pos(ko *self, int x, int y)
{
	int i;
	so *s;
	assert(self);
	for (i = 0; i < STATES_MAX; i++) {
		s = self->objects[i];
		if (s) {
			so_set_pos(
				s,
				x - anim_get_width(so_get_anim(s)) / 2,
				y - anim_get_height(so_get_anim(s)) / 2
			);
		}
	}
}

size_t ko_sizeof(void)
{
	return sizeof(ko);
}
