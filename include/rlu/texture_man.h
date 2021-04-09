#ifndef RLU_TEXTURE_MAN_H
#define RLU_TEXTURE_MAN_H

#include "util.h"

#define TEXTURES_MAX 32

#define TEXTURE_GEN(WIDTH, HEIGHT, COLOR) \
	LoadTextureFromImage(GenImageColor((WIDTH), (HEIGHT), (COLOR)))

typedef struct texture_lookup_tag {
	char *png;
	Texture2D texture;
} texture_lookup;

typedef struct texture_manager_tag {
	texture_lookup textures[TEXTURES_MAX];
} texture_manager;

void texture_man_new(texture_manager *out);
void texture_man_del(texture_manager *self);

/**
 * Load a texture and return a shared pointer to it,
 * ignore the loading if the texture already exists
 */
Texture2D *texture_man_load(texture_manager *self, char *png);
Texture2D *texture_man_load_or_default(texture_manager *self, char *png, Texture2D def);

/* Extract a texture. If it's not there, assertion */
Texture2D *texture_man_get(texture_manager *self, char *png);

/**
 * Test drawing routine
 * Only use for test drawing or if animation will not
 * be necessary.
*/
void texture_man_test_draw(Texture2D *texture, int x, int y);
Image texture_man_img_load_or_default(char *png, int width, int height, Color color);

#endif // RLU_TEXTURE_MAN_H
