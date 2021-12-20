#include <memory.h>
#include <rlu/rlu.h>
#include "drop_manager.h"
#include "data.h"

#define DROPS_MAX 32
#define OUT_OF_BOUNDS 20000
#define FALL_VELOCITY 1
#define DROP_CHANCE 0.20 // percent
#define DROP_WIDTH 40
#define DROP_HEIGHT 40
#define DROP_DEFCOLOR PURPLE

typedef struct drop_object_tag {
	so *object;
	item_drop drop;
	float x;
	float y;
} drop_object;

typedef struct drop_context_tag {
	drop_object drops[DROPS_MAX];
	int dropndx;
} drop_context;

static void xinsert(item_drop drop, float x, float y);
static void xremove(int index);
static void setpos(drop_object *dro, float x, float y);

static drop_context ctx;
static anim_man *animan;
static texture_manager texman;
static int initialized = false;

void drop_manager_init(void)
{
	Texture2D *t;
	so *s;
	int i;
	char buf[256];

	assert(initialized == false);
	initialized = true;

	animan = anim_man_new();
	texture_man_new(&texman);

	memset(&ctx, 0, sizeof(ctx));
	snprintf(buf, sizeof(buf), "%s/%s", context_get_skindir(), DATA_ASSET_DROP);
	for (i = 0; i < DROPS_MAX; i++) {
		t = texture_man_load_or_default(&texman, buf, TEXTURE_GEN(DROP_WIDTH, DROP_HEIGHT, DROP_DEFCOLOR));
		s = so_new(anim_man_load(animan, t, 1, 1));
		ctx.drops[i].object = s;
		setpos(&ctx.drops[i], OUT_OF_BOUNDS, OUT_OF_BOUNDS);
	}
}

void drop_manager_dropit(enemy_data *ed)
{
	item_drop drop;
	float r = rand_uniform();

	if (r > DROP_CHANCE) {
		return;
	}

	drop = drop_random();
	xinsert(drop, ed->x, ed->y);
}

item_drop drop_manager_pickup(player_data *pd)
{
	item_drop rv;
	int collides;
	int i;
	Rectangle drop_rect;
	Rectangle player_rect = (Rectangle){
		pd->x - pd->width / 2,
		pd->y - pd->height / 2,
		pd->width,
		pd->height
	};

	memset(&rv, 0, sizeof(rv));
	for (i = 0; i < DROPS_MAX; i++) {
		drop_rect = (Rectangle){
			ctx.drops[i].x - DROP_WIDTH / 2,
			ctx.drops[i].y - DROP_HEIGHT / 2,
			DROP_WIDTH,
			DROP_HEIGHT
		};

		collides = CheckCollisionRecs(player_rect, drop_rect);
		if (collides) {
			rv = ctx.drops[i].drop;
			xremove(i);
			break;
		}
	}
	return rv;
}

void drop_manager_update(void)
{
	int i;
	for (i = 0; i < DROPS_MAX; i++) {
		if (ctx.drops[i].drop.type == DROP_NONE) {
			setpos(&ctx.drops[i], OUT_OF_BOUNDS, OUT_OF_BOUNDS);
		}
		else {
			setpos(&ctx.drops[i], ctx.drops[i].x, ctx.drops[i].y + FALL_VELOCITY);
			so_update(ctx.drops[i].object);
			if (ctx.drops[i].y > GetScreenHeight()) {
				ctx.drops[i].drop.type = DROP_NONE;
				setpos(&ctx.drops[i], OUT_OF_BOUNDS, OUT_OF_BOUNDS);
			}
		}
	}
}

void drop_manager_draw(void)
{
	int i;
	char *p;
	for (i = 0; i < DROPS_MAX; i++) {
		if (ctx.drops[i].drop.type != DROP_NONE) {
			so_draw(ctx.drops[i].object);

			p = drop_to_string(&ctx.drops[i].drop);
			if (ctx.drops[i].drop.type == DROP_BULLET) {
				p += + sizeof("player_bullet");
			}

			DrawText(
				p,
				ctx.drops[i].x + anim_get_width(so_get_anim(ctx.drops[i].object)),
				ctx.drops[i].y,
				12, WHITE);
		}
	}
}

void drop_manager_cleanup(void)
{
	assert(initialized == true);
	initialized = false;

	anim_man_del(animan);
	texture_man_del(&texman);
	memset(&ctx, 0, sizeof(ctx));
}

static void xinsert(item_drop drop, float x, float y)
{
	ctx.drops[ctx.dropndx].drop = drop;
	ctx.drops[ctx.dropndx].x = x;
	ctx.drops[ctx.dropndx].y = y;
	so_set_pos(ctx.drops[ctx.dropndx].object, (float)x, (float)y);
	ctx.dropndx = (ctx.dropndx + 1) % DROPS_MAX;
}

static void xremove(int index)
{
	assert(0 <= index);
	assert(index < DROPS_MAX);

	memset(&ctx.drops[index].drop, 0, sizeof(ctx.drops[index].drop));
	setpos(&ctx.drops[index], OUT_OF_BOUNDS, OUT_OF_BOUNDS);
}

static void setpos(drop_object *dro, float x, float y)
{
	assert(dro);
	dro->x = x;
	dro->y = y;
	so_set_pos(dro->object, x, y);
}
