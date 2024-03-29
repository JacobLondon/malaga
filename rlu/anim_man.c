#include "rlu.h"

#define IMGS_MAX 64

struct anim_man_tag {
	Texture2D *textures[IMGS_MAX];
	anim *animations[IMGS_MAX];
};

anim_man *anim_man_new(void)
{
	anim_man *self = allocate(sizeof(anim_man));
	assert(self);
	memset(self, 0, sizeof(anim_man));
	return self;
}

void anim_man_del(anim_man *self)
{
	int i;
	assert(self);
	for (i = 0; i < IMGS_MAX; i++) {
		if (self->textures[i]) {
			anim_del(self->animations[i]);
			self->textures[i] = NULL;
		}
	}
	dealloc(self);
}

void anim_man_update(anim_man *self)
{
	int i;
	assert(self);
	for (i = 0; i < IMGS_MAX; i++) {
		if (self->textures[i] != NULL) {
			anim_update(self->animations[i]);
		}
	}
}

anim *anim_man_get(anim_man *self, Texture2D *texture)
{
	int i;
	assert(self);
	assert(texture);
	for (i = 0; i < IMGS_MAX; i++) {
		if (self->textures[i] == texture) {
			return self->animations[i];
		}
	}
	msg_assert(i != IMGS_MAX, "Animation not found");
	return NULL;
}

anim *anim_man_load(anim_man *self, Texture2D *texture, int width, int height)
{
	int i;
	assert(self);
	assert(texture);
	for (i = 0; i < IMGS_MAX; i++) {
		if (self->textures[i] == NULL) {
			self->textures[i] = texture;
			self->animations[i] = anim_new(texture, width, height);
			return self->animations[i];
		}
		// texture already exists, don't need to load it again
		else if (self->textures[i] == texture) {
			return self->animations[i];
		}
	}
	msg_assert(i != IMGS_MAX, "Too many animations loaded: %d", IMGS_MAX);
	return NULL;
}
