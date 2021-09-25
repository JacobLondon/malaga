#ifndef RLU_ATMOSPHERE_H
#define RLU_ATMOSPHERE_H

#include <stddef.h>
#include "scene.h"
#include "scene_man.h"
#include "texture_man.h"

typedef struct atmosphere_tag atmosphere_t;

atmosphere_t *atmosphere_new(char *mp3_nullable);
void atmosphere_del(atmosphere_t *atm);

int atmosphere_insert_definition(atmosphere_t *atm, char *scene_name, size_t max_objects, scene_cb init, void *client);
int atmosphere_insert_set(atmosphere_t *atm, char *set_name);
int atmosphere_insert_set_scene(atmosphere_t *atm, char *set_name, char *scene_name);
void atmosphere_finish_inserting(atmosphere_t *atm);

void atmosphere_load(atmosphere_t *atm, const char *set_name);

void atmosphere_update(atmosphere_t *atm);
void atmosphere_draw(atmosphere_t *atm);

const char *atmosphere_get_setname_or_default(atmosphere_t *atm, char *set_name, char *def);
scene_manager *atmosphere_get_sceneman(atmosphere_t *atm);
texture_manager *atmosphere_get_textureman(atmosphere_t *atm);

#endif // RLU_ATMOSPHERE_H
