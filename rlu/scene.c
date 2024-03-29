#include "rlu.h"

struct scene_tag {
	so **scene_objects;
	char *name;
	size_t max_objects;
};

void scene_definition_del(scene_definition *self)
{
	assert(self);
	assert(self->name);

	dealloc(self->name);
	(void)memset(self, 0, sizeof(scene_definition));
}

scene *scene_new(char *name, size_t max_objects, scene_cb init, void *client)
{
	scene *self = allocate(sizeof(scene));
	assert(self);
	self->scene_objects = allocate(sizeof(so *) * max_objects);
	memset(self->scene_objects, 0, sizeof(so *) * max_objects);
	self->name = name;
	self->max_objects = max_objects;

	if (init) {
		init(self, client);
	}
	return self;
}

scene *scene_new_def(scene_definition *def)
{
	return scene_new(def->name, def->max_objects, def->init, def->client);
}

void scene_del(scene *self)
{
	assert(self);
	scene_clear(self);
	dealloc(self);
}

void scene_load_object(scene *self, so *s)
{
	size_t i;
	assert(self);
	assert(s);
	for (i = 0; i < self->max_objects; i++) {
		if (self->scene_objects[i] == NULL) {
			self->scene_objects[i] = s;
			break;
		}
	}
	msg_assert(i != self->max_objects, "Too many scene objects: %zu", i);
}

void scene_clear(scene *self)
{
	size_t i;
	assert(self);
	for (i = 0; i < self->max_objects; i++) {
		if (self->scene_objects[i]) {
			so_del(self->scene_objects[i]);
			self->scene_objects[i] = NULL;
		}
	}
}

void scene_draw(scene *self)
{
	size_t i;
	assert(self);
	for (i = 0; i < self->max_objects; i++) {
		if (self->scene_objects[i]) {
			so_draw(self->scene_objects[i]);
		}
	}
}

void scene_update(scene *self)
{
	size_t i;
	assert(self);
	for (i = 0; i < self->max_objects; i++) {
		if (self->scene_objects[i]) {
			so_update(self->scene_objects[i]);
		}
	}
}

char *scene_get_name(scene *self)
{
	return self->name;
}
