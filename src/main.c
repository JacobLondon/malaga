#include <raylib.h>
#include <rlu/rlu.h>

struct thing {
	int x;
	int y;
	int size;
	void (*draw)(struct thing *);
	void (*update)(struct thing *);
};

static void context_init(void);
static void context_update(void);
static void context_draw(void);
static void context_cleanup(void);

static void player_draw(struct thing *self);
static void player_update(struct thing *self);
static void pbull_draw(struct thing *self);
static void pbull_update(struct thing *self);
static void ebull_draw(struct thing *self);
static void ebull_update(struct thing *self);

static float MG_SCREEN_WIDTH = 1600;
static float MG_SCREEN_HEIGHT = 900;
static int MG_FPS = 60;

static const char *MG_TITLE = "Matt's Game";

static struct thing player = {
	.size=50,
	.draw=player_draw,
	.update=player_update,
};
static struct thing pbulls[1000];
static int pbullndx = 0;

int main(void)
{
	context_init();

	while (!WindowShouldClose()) {
		context_update();
		BeginDrawing();
		context_draw();
		EndDrawing();
	}

	context_cleanup();

	return 0;
}

static void context_init(void)
{
	// graphics initialization
	InitWindow(MG_SCREEN_WIDTH, MG_SCREEN_HEIGHT, MG_TITLE);
	SetTargetFPS(MG_FPS);

	// library initialization
	texture_man_init();

	for (int i = 0; i < ARRAY_SIZE(pbulls); i++) {
		pbulls[i].size = 20;
		pbulls[i].draw = pbull_draw;
		pbulls[i].update = pbull_update;
	}
}

static void context_update(void)
{
	player_update(&player);
	for (int i = 0; i < ARRAY_SIZE(pbulls); i++) {
		pbull_update(&pbulls[i]);
	}
}

static void context_draw(void)
{
	DrawRectangle(0, 0, MG_SCREEN_WIDTH, MG_SCREEN_HEIGHT, BLACK);
	DrawFPS(0, 0);

	player_draw(&player);
	for (int i = 0; i < ARRAY_SIZE(pbulls); i++) {
		pbull_draw(&pbulls[i]);
	}
}

static void context_cleanup(void)
{
	CloseWindow();
}

static void player_draw(struct thing *self)
{
	DrawCircle(self->x + self->size / 2, self->y + self->size / 2, self->size, RED);

	if (IsKeyDown(KEY_SPACE)) {
		pbulls[pbullndx].x = player.x;
		pbulls[pbullndx].y = player.y;
		pbullndx++;
	}
}

static void player_update(struct thing *self)
{
	self->x = GetMouseX();
	self->y = GetMouseY();
}

static void pbull_draw(struct thing *self)
{
	if (self->x == 0 && self->y == 0) {
		return;
	}

	DrawCircle(self->x, self->y, self->size, BLUE);
}

static void pbull_update(struct thing *self)
{
	if (self->x == 0 && self->y == 0) {
		return;
	}

	if (self->x < 0 || self->x > MG_SCREEN_WIDTH || self->y < 0 || self->y > MG_SCREEN_HEIGHT) {
		self->x = 0;
		self->y = 0;
	}

	self->y += 30;
}

static void ebull_draw(struct thing *self)
{

}

static void ebull_update(struct thing *self)
{

}

