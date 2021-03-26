#include <rlu/rlu.h>

void texture_man_new(texture_manager *out)
{
	assert(out);
	memset(out, 0, sizeof(*out));
}

void texture_man_cleanup(texture_manager *self)
{
	int i;
	for (i = 0; i < TEXTURES_MAX; i++) {
		if (self->textures[i].png) {
			UnloadTexture(self->textures[i].texture);
		}
	}
	memset(self->textures, 0, sizeof(self->textures));
}

Texture2D *texture_man_load(texture_manager *self, char *png)
{
	int i;
	Texture2D *ret = NULL;
	assert(png);

	for (i = 0; i < TEXTURES_MAX; i++) {
		if (self->textures[i].png == NULL) {
			self->textures[i].png = png;
			if (FileExists(png)) {
				self->textures[i].texture = LoadTexture(png);
			}
			else {
				self->textures[i].texture = LoadTextureFromImage(
					GenImageChecked(100, 100, 2, 2, MAGENTA, BLACK)
				);
				msg_warning("Could not load texture '%s'", png);
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

void texture_man_test_draw(Texture2D *texture, int x, int y)
{
	assert(texture);
	DrawTexture(*texture, x, y, WHITE);
}
