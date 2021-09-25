#include "rlu.h"

void texture_man_new(texture_manager *out)
{
	assert(out);
	memset(out, 0, sizeof(*out));
}

void texture_man_del(texture_manager *self)
{
	int i;
	for (i = 0; i < TEXTURES_MAX; i++) {
		if (self->textures[i].png) {
			UnloadTexture(self->textures[i].texture);
			free(self->textures[i].png);
		}
	}
	memset(self->textures, 0, sizeof(self->textures));
}

Texture2D *texture_man_load(texture_manager *self, char *png)
{
	return texture_man_load_or_default(self, png, LoadTextureFromImage(GenImageChecked(100, 100, 2, 2, MAGENTA, BLACK)));
}

Texture2D *texture_man_load_or_default(texture_manager *self, char *png, Texture2D def)
{
	int i;
	Texture2D *ret = NULL;
	assert(png);

	for (i = 0; i < TEXTURES_MAX; i++) {
		if (self->textures[i].png == NULL) {
			self->textures[i].png = strdup(png);
			assert(self->textures[i].png);

			if (FileExists(png)) {
				self->textures[i].texture = LoadTexture(self->textures[i].png);
			}
			else {
				self->textures[i].texture = def;
				msg_warning("Could not load texture '%s'. Using default", png);
			}
			ret = &self->textures[i].texture;
			break;
		}
		else if (streq(self->textures[i].png, png)) {
			ret = &self->textures[i].texture;
			break;
		}
	}
	msg_assert(i < TEXTURES_MAX, "Too many textures loaded: %d / %d", i, TEXTURES_MAX);
	return ret;
}

Texture2D *texture_man_get(texture_manager *self, char *png)
{
	int i;
	assert(png);
	for (i = 0; i < TEXTURES_MAX; i++) {
		if (self->textures[i].png == NULL) {
			continue;
		}
		else if (streq(self->textures[i].png, png)) {
			return &self->textures[i].texture;
		}
	}
	msg_assert(i < TEXTURES_MAX, "Texture not found: %s", png);
	return NULL;
}

void texture_man_draw_tex(Texture2D *texture, int x, int y)
{
	assert(texture);
	DrawTexture(*texture, x, y, WHITE);
}

Image texture_man_img_load_or_default(char *png, int width, int height, Color color)
{
	assert(png);
	if (FileExists(png)) {
		return LoadImage(png);
	}
	return GenImageColor(width, height, color);
}
