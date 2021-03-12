#include <rlu/rlu.h>
#include "../modules.h"

struct thing {
	int x;
	int y;
	int size;
	void (*draw)(struct thing *);
	void (*update)(struct thing *);
};

static void player_draw(struct thing *self);
static void player_update(struct thing *self);
static void pbull_draw(struct thing *self);
static void pbull_update(struct thing *self);
static void ebull_draw(struct thing *self);
static void ebull_update(struct thing *self);

static struct thing player = {
	.size=50,
	.draw=player_draw,
	.update=player_update,
};
static struct thing pbulls[1000];
static int pbullndx = 0;

void mg_init(void)
{
	for (int i = 0; i < ARRAY_SIZE(pbulls); i++) {
		pbulls[i].size = 20;
		pbulls[i].draw = pbull_draw;
		pbulls[i].update = pbull_update;
		pbulls[i].x = -40;
	}
}

void mg_cleanup(void)
{

}

void mg_update(void)
{
	player_update(&player);
	for (int i = 0; i < ARRAY_SIZE(pbulls); i++) {
		pbull_update(&pbulls[i]);
	}
}

void mg_draw(void)
{
	DrawFPS(0, 0);

	player_draw(&player);
	for (int i = 0; i < ARRAY_SIZE(pbulls); i++) {
		pbull_draw(&pbulls[i]);
	}
}

static void player_draw(struct thing *self)
{
	DrawCircle(self->x + self->size / 2, self->y + self->size / 2, self->size, RED);
}

static void player_update(struct thing *self)
{
	self->x = GetMouseX();
	self->y = GetMouseY();

	if (IsKeyDown(KEY_SPACE)) {
		pbulls[pbullndx].x = player.x;
		pbulls[pbullndx].y = player.y;
		pbullndx = (pbullndx + 1) % ARRAY_SIZE(pbulls);
	}
}

static void pbull_draw(struct thing *self)
{
	if (self->x < -pbulls[0].size || self->x > GetScreenWidth() || self->y < -pbulls[0].size || self->y > GetScreenHeight()) {
		return;
	}

	DrawCircle(self->x, self->y, self->size, BLUE);
}

static void pbull_update(struct thing *self)
{
	if (self->x == 0 && self->y == 0) {
		return;
	}

	if (self->x < 0 || self->x > GetScreenWidth() || self->y < 0 || self->y > GetScreenHeight()) {
		return;
	}

	self->y += 30;
}

static void ebull_draw(struct thing *self)
{

}

static void ebull_update(struct thing *self)
{

}

