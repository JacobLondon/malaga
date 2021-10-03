#include "rlu.h"

static void param_read_helper(param *item, char *value);

void *(*allocate)(size_t size) = malloc;
void (*dealloc)(void *p) = free;

static size_t pool_index = 0;
static size_t pool_size = 0;
static void *pool_begin = NULL;

void pool_init(size_t kilobytes)
{
	assert(pool_begin == NULL);
	pool_begin = malloc(kilobytes * 1024);
	assert(pool_begin);
	pool_size = kilobytes * 1024;
	memset(pool_begin, 0, pool_size);
	allocate = poolloc;
	dealloc = poolfree;
}

void pool_cleanup(void)
{
	assert(pool_begin);
	(void)memset(pool_begin, 0, pool_size);
	free(pool_begin);
	pool_index = 0;
	pool_size = 0;
	pool_begin = NULL;
	allocate = malloc;
	dealloc = free;
}

void pool_usage(void)
{
	(void)printf("%.3f KB / %.3f KB\n", pool_index / 1000.0f, pool_size / 1000.0f);
}

void *poolloc(size_t size)
{
	void *addr;
	msg_assert(pool_index + size < pool_size, "Not enough memory: %zu / %zu bytes", pool_index + size, pool_size);
	addr = (void *)&((unsigned char *)pool_begin)[pool_index];
	pool_index += size;

	return addr;
}

void poolfree(void *p)
{
	(void)p;
	// It's fine, just drain the pool.
	return;
}

int rand_range(int min, int max)
{
	return rand() % (max - min) + min;
}

float rand_uniform(void)
{
	return (float)rand() / (float)RAND_MAX;
}

char *file_read(const char *fname, size_t *size)
{
	if (!fname) {
		return NULL;
	}

	char *buf;
	size_t bytes;
	long length;

	FILE *f = fopen(fname, "rb");
	assert(f);

	fseek(f, 0, SEEK_END);
	length = ftell(f);
	fseek(f, 0, SEEK_SET);

	buf = malloc(length + 1);
	assert(buf);
	
	bytes = fread(buf, 1, length, f);
	buf[bytes] = '\0';
	if (size) {
		*size = bytes;
	}

	fclose(f);
	return buf;
}

int file_write(const char *fname, const char *str)
{
	FILE *f = fopen(fname, "wb");
	if (!f) {
		return 0;
	}

	fputs(str, f);
	fclose(f);
	return 1;
}

int file_append(const char *fname, const char *str)
{
	FILE *f = fopen(fname, "ab");
	if (!f) {
		return 0;
	}

	fputs(str, f);
	fclose(f);
	return 1;
}

int arg_check(int argc, char **argv, const char *arg)
{
	int i;
	for (i = 0; i < argc; i++) {
		if (strcmp(argv[i], arg) == 0) {
			return 1;
		}
	}
	return 0;
}

char *arg_get(int argc, char **argv, const char *arg)
{
	int i;
	for (i = 0; i < argc; i++) {
		if ((strcmp(argv[i], arg) == 0) && (i + 1 < argc)) {
			return argv[i + 1];
		}
	}
	return NULL;
}

float fast_fsqrtf(float number)
{
	const float x2 = number * 0.5F;
	const float threehalfs = 1.5F;

	union {
		float f;
		uint32_t i;
	} conv = { number }; /* member 'f' set to value of 'number'. */
	conv.i = 0x5f3759df - (conv.i >> 1);
	conv.f *= (threehalfs - (x2 * conv.f * conv.f));
	return 1.0f / conv.f;
}

float fast_distancef(float x0, float y0, float x1, float y1)
{
	return fast_fsqrtf((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0));
}

float fast_fsinf(float x)
{
	int k;
	double y;
	double z;
	union {
		double f;
		int i;
	} s;

	z = x;
	z *= 0.3183098861837907;
	z += 6755399441055744.0;
	s.f = z;
	k = s.i;
	z = k;
	z *= 3.1415926535897932;
	x -= z;
	y = x;
	y *= x;
	z = 0.0073524681968701;
	z *= y;
	z -= 0.1652891139701474;
	z *= y;
	z += 0.9996919862959676;
	x *= z;
	k &= 1;
	k += k;
	z = k;
	z *= x;
	x -= z;

	return (float)x;
}

float fast_fcosf(float x)
{
	return fast_fsinf(x + 3.14159265358979323846 / 2.0);
}

float fclamp(float x, float max, float min)
{
	if (x > max) {
		return max;
	}
	else if (x < min) {
		return min;
	}
	return x;
}

#ifndef min
int min(int a, int b)
{
	return (a > b) ? a : b;
}
#endif

#ifndef strdup
char *strdup(const char *str)
{
	char *rv;
	size_t len;
	assert(str);
	len = strlen(str);
	rv = malloc(len + 1);
	if (rv == NULL) {
		return NULL;
	}
	strncpy(rv, str, len);
	rv[len] = '\0';
	return rv;
}
#endif

int param_read(const char *filename, param paramList[])
{
	char buf[256];
	FILE *fp;
	char *p;
	size_t i;

	assert(filename);
	assert(paramList);

	// load defaults
	for (i = 0; paramList[i].name != NULL; i++) {
		param_read_helper(&paramList[i], NULL);
	}

	fp = fopen(filename, "r");
	if (!fp) {
		// already loaded defaults
		return 1;
	}

	// read file line by line
	while (fgets(buf, sizeof(buf), fp)) {
		if (buf[0] == '#') {
			continue;
		}

		p = strchr(buf, '=');
		if (!p) {
			continue;
		}

		// check if the varname is in the param list, 
		for (i = 0; paramList[i].name != NULL; i++) {
			if (strncmp(buf, paramList[i].name, (size_t)(p - buf)) == 0) {
				param_read_helper(&paramList[i], p + 1);
				break;
			}
		}
	}

	(void)fclose(fp);
	return 0;
}

static void param_read_helper(param *item, char *value)
{
	assert(item);

	if (value == NULL) {
	default_param:
		(void)memcpy(item->value, item->def, item->len);
		return;
	}

	union {
		float f;
		int i;
		Color c;
		char boolean[8];
	} tmp;
	int rv;

	switch (item->type) {
	case PARAM_TYPE_FLOAT:
		rv = sscanf(value, "%f", &tmp.f);
		if (rv != 1) {
			goto default_param;
		}
		*(float *)item->value = tmp.f;
		break;
	case PARAM_TYPE_INT:
		rv = sscanf(value, "%d", &tmp.i);
		if (rv != 1) {
			goto default_param;
		}
		*(int *)item->value = tmp.i;
		break;
	case PARAM_TYPE_BOOL:
		rv = sscanf(value, "%s", tmp.boolean);
		if (rv != 1) {
			goto default_param;
		}
		if (strcmp(tmp.boolean, "true") == 0) {
			*(bool *)item->value = !0;
		}
		else {
			*(bool *)item->value = 0;
		}
		break;
	case PARAM_TYPE_COLOR:
		rv = sscanf(value, "%hhd %hhd %hhd %hhd", &tmp.c.r, &tmp.c.g, &tmp.c.b, &tmp.c.a);
		if (rv != 3 && rv != 4) {
			goto default_param;
		}
		if (rv == 3) {
			tmp.c.a = 255;
		}
		tmp.c.r = min(tmp.c.r, 255);
		tmp.c.g = min(tmp.c.g, 255);
		tmp.c.b = min(tmp.c.b, 255);
		tmp.c.a = min(tmp.c.a, 255);
		*(Color *)item->value = tmp.c;
		break;
	case PARAM_TYPE_STRING:
		// TODO: Is this fine? 'value' is in buf somewhere
		(void)snprintf((char *)item->value, (size_t)item->len, "%s", value);
		break;
	default:
		goto default_param;
	}
}

int param_write(const char *filename, param paramList[])
{
	int i;
	FILE *fp;

	assert(filename);
	assert(paramList);

	fp = fopen(filename, "w");
	if (!fp) {
		return 1;
	}

	for (i = 0; paramList[i].name != NULL; i++) {
		switch (paramList[i].type) {
		case PARAM_TYPE_FLOAT:
			(void)fprintf(fp, "%s=%f\n", paramList[i].name, *(float *)paramList[i].value);
			(void)fflush(fp);
			break;
		case PARAM_TYPE_INT:
			(void)fprintf(fp, "%s=%d\n", paramList[i].name, *(int *)paramList[i].value);
			(void)fflush(fp);
			break;
		case PARAM_TYPE_BOOL:
			(void)fprintf(fp, "%s=%hhd\n", paramList[i].name, *(bool *)paramList[i].value);
			(void)fflush(fp);
			break;
		case PARAM_TYPE_COLOR:
			(void)fprintf(fp, "%s=%d %d %d %d\n", paramList[i].name,
				((Color *)paramList[i].value)->r,
				((Color *)paramList[i].value)->g,
				((Color *)paramList[i].value)->b,
				((Color *)paramList[i].value)->a);
			(void)fflush(fp);
			break;
		case PARAM_TYPE_STRING:
			(void)fprintf(fp, "%s=%s\n", paramList[i].name, (char *)paramList[i].value);
			(void)fflush(fp);
			break;
		default:
			break;
		}
	}

	(void)fclose(fp);
	return 0;
}

void svec_init(svec *out, void **static_array, size_t length)
{
	assert(out);
	assert(static_array);

	(void)memset(out, 0, sizeof(*out));
	out->buf = static_array;
	out->len = length;
	out->end = 0;
}

void svec_push(svec *self, void *data)
{
	assert(self);
	assert(self->end < self->len);

	self->buf[self->end] = data;
	self->end++;
}

void svec_pop(svec *self)
{
	assert(self);
	
	if (self->end == 0) {
		self->buf[0] = NULL;
		return;
	}

	self->end--;
	self->buf[self->end] = NULL;
}

void svec_map(svec *self, void (*func)(void *data))
{
	assert(self);
	assert(func);

	for (size_t i = 0; i < self->end; i++) {
		func(self->buf[i]);
	}
}

void *svec_tail(svec *self)
{
	assert(self);
	if (self->end == 0) {
		return self->buf[0];
	}
	return self->buf[self->end - 1];
}

Color color_lookup(const char *color)
{
	int i;
	static struct {
		const char *name;
		Color color;
	} lookup[] = {
		{"lightgray", LIGHTGRAY},
		{"gray", GRAY},
		{"darkgray", DARKGRAY},
		{"yellow", YELLOW},
		{"gold", GOLD},
		{"orange", ORANGE},
		{"pink", PINK},
		{"red", RED},
		{"maroon", MAROON},
		{"green", GREEN},
		{"lime", LIME},
		{"darkgreen", DARKGREEN},
		{"skyblue", SKYBLUE},
		{"blue", BLUE},
		{"darkblue", DARKBLUE},
		{"purple", PURPLE},
		{"violet", VIOLET},
		{"beige", BEIGE},
		{"brown", BROWN},
		{"darkbrown", DARKBROWN},
		{"white", WHITE},
		{"black", BLACK},
		{"magenta", MAGENTA},
		{0}
	};

	if (!color) {
		goto miss;
	}

	for (i = 0; lookup[i].name != NULL; i++) {
		if (strcasecmp(color, lookup[i].name) == 0) {
			return lookup[i].color;
		}
	}

miss:
	return MAGENTA;
}
