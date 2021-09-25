#ifndef RLU_KEY_MAN_H
#define RLU_KEY_MAN_H

#define KEY_OBJECTS_MAX 32

typedef void (*load_cb)(ko *self);

/* definitions to look for when loading */
typedef struct key_definition_tag {
	char *name;
	load_cb load;
} key_definition;

typedef struct key_manager_tag {
	key_definition *defs;
	ko *key_objects[KEY_OBJECTS_MAX];
} key_manager;

/**
 * Manage groupings of key objects. Call
 * update on many key objects at once
 * for convenience.
 * Key objects can still be manually
 * managed however.
 * 
 * The key manager has its own internal
 * state as a file module. This should
 * be the highest level of key object
 * hierarchy.
 */

void key_man_new(key_manager *out, key_definition *definitions);
void key_man_del(key_manager *self);

/* load static key object initializations by name, end with NULL term */
void key_man_load(key_manager *self, char **names);

/* call update on all key objects */
void key_man_update(key_manager *self);

#endif // RLU_KEY_MAN_H
