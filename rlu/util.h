#ifndef RLU_UTIL_H
#define RLU_UTIL_H

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include <rlu/raylib.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

#define msg_assert(Expr, ...) do { \
	if (!!(Expr)) {} \
	else { \
		fprintf(stderr, "ERROR: " __VA_ARGS__); \
		fprintf(stderr, "\n"); \
		fflush(stderr); \
		assert(Expr); \
		exit(1); \
	} \
} while (0)

#define msg_warning(...) \
	do { \
		(void)fprintf(stderr, "WARNING: " __VA_ARGS__); \
		(void)fprintf(stderr, "\n"); \
		(void)fflush(stderr); \
	} while (0)

#define msg_default(...) \
	do { \
		(void)fprintf(stderr, "INFO: " __VA_ARGS__); \
		(void)fprintf(stderr, "\n"); \
		(void)fflush(stderr); \
	} while (0)

/* definitely not thread safe lol */
void pool_init(size_t kilobytes);
void pool_cleanup(void);
void pool_usage(void);
void *poolloc(size_t size);
void poolfree(void *p);

/**
 * Use allocate and dealloc for dynamic
 * memory allocation. If pool isn't
 * initialized, it just used malloc and
 * free. But if pool is initialized, then
 * poolloc and poolfree is used instad.
 * On pool cleanup, malloc and free are
 * retied to these pointers.
 */
extern void *(*allocate)(size_t size);
extern void (*dealloc)(void *p);

#define RAND_RANGE(min, max) (rand() % (max - min) + min)
int rand_range(int min, int max);
float rand_uniform(void);

#define streq(s1, s2) (strcmp((s1), (s2)) == 0)

char *file_read(const char *fname, size_t *size);
int file_write(const char *fname, const char *str);
int file_append(const char *fname, const char *str);

int arg_check(int argc, char **argv, const char *arg);
char *arg_get(int argc, char **argv, const char *arg);

float fast_fsqrtf(float number);
float fast_fdistancef(float x0, float y0, float x1, float y1);
float fast_fsinf(float x);
float fast_fcosf(float x);
float fclamp(float x, float max, float min);

#ifndef min
int min(int, int);
#endif

#ifndef strdup
char *strdup(const char *str);
#endif

typedef enum param_type_tag {
	PARAM_TYPE_FLOAT,
	PARAM_TYPE_INT,
	PARAM_TYPE_BOOL,
	PARAM_TYPE_COLOR,
	PARAM_TYPE_STRING,
} param_type;

typedef struct param_tag {
	const char *name;
	void *value;
	void *def;
	int len;
	param_type type;
} param;

#define DEFINE_PARAM(TYPE, UNQUOTED_NAME, CONFIG, DEFAULT) \
{ \
	.name = #UNQUOTED_NAME, \
	.value = &CONFIG.UNQUOTED_NAME, \
	.def = &DEFAULT.UNQUOTED_NAME, \
	.len = sizeof(CONFIG.UNQUOTED_NAME), \
	.type = TYPE, \
}

#define DEFINE_PARAM_END() \
{ \
	.name=NULL, \
	.value=NULL, \
	.def=NULL, \
	.len=0, \
	.type=0, \
}

/** read the config file defined by paramList
 * Should have the format "varname=value" with 1 item per line
 */
int param_read(const char *filename, param paramList[]);
/** write the param list to the file */
int param_write(const char *filename, param paramList[]);

/**
 * Wrap an array to act as a growing/shrinkable vector.
 * Everything should be an array of pointers.
 * No point should be owned.
 */
typedef struct svec_tag {
	void **buf;
	size_t len;
	size_t end;
} svec;

void svec_init(svec *out, void **static_array, size_t length);
void svec_push(svec *self, void *data);
void svec_pop(svec *self);
void svec_map(svec *self, void (*func)(void *data));
void *svec_tail(svec *self); /** get last item */

Color color_lookup(const char *color);

#endif // RLU_UTIL_H
