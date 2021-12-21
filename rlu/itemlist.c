#include <rlu/rlu.h>

static char *get_around(int center, int AB, int max, char **buf);
static char *find_next(Itemlist *il);
static char *find_prev(Itemlist *il);

Itemlist *itemlist_new(ItemlistArgs *args)
{
	Itemlist *il;
	assert(args);
	il = allocate(sizeof(Itemlist));
	assert(il);
	memset(il, 0, sizeof(Itemlist));
	il->args = *args;
	il->filtered = parray_new(NULL);
	assert(il->filtered);

	return il;
}

void itemlist_delete(Itemlist *il)
{
	assert(il);
	assert(il->filtered);

	if (il->dirset) {
		ClearDirectoryFiles();
	}

	parray_free(il->filtered);
	dealloc(il);
}

void itemlist_set(Itemlist *il, char **buf, size_t len)
{
	size_t i;

	assert(il);
	assert(il->filtered);

	while (il->filtered->size > 0) {
		parray_pop(il->filtered);
	}

	for (i = 0; i < len; i++) {
		// skin current/parent dir if there
		if (streq(buf[i], ".") || streq(buf[i], "..")) {
			continue;
		}

		// only get what the doctor ordered
		if (il->args.search) {
			if (strstr(buf[i], il->args.search)) {
				parray_push(il->filtered, buf[i]);
			}
		}
		// everything
		else {
			parray_push(il->filtered, buf[i]);
		}
	}

	il->buf = il->filtered->buf;
	il->buflen = il->filtered->size;
	il->bufndx = 0;
}

bool itemlist_set_directory(Itemlist *il, const char *directory)
{
	char **files;
	int filecount;

	assert(il);
	assert(directory);
	
	if (il->dirset) {
		ClearDirectoryFiles();
		il->dirset = false;
	}

	if (!il->dirset) {
		if (DirectoryExists(directory)) {
			il->dirset = true;
			files = GetDirectoryFiles(directory, &filecount);
			itemlist_set(il, files, filecount);
		}
	}
}

bool itemlist_try_select(Itemlist *il, const char *name)
{
	int i;

	assert(il);
	assert(name);

	for (i = 0; i < il->filtered->size; i++) {
		if (streq(name, (char *)il->filtered->buf[i])) {
			il->bufndx = i;
			return true;
		}
	}

	return false;
}

char *itemlist_next(Itemlist *il)
{
	return find_next(il);
}

char *itemlist_prev(Itemlist *il)
{
	return find_prev(il);
}

void itemlist_draw(Itemlist *il)
{
	int i;

	assert(il);

	if (!il->buf) {
		return;
	}

	// draw around
	if (il->bufndx != -1) {
		for (i = -il->args.AB + 1; i < il->args.AB; i++) {
			// selected
			if (i == 0) {
				DrawRectangle(
					0,
					il->args.totalVert / il->args.fontsize * i + il->args.offsetVert,
					il->args.totalHorz,
					il->args.fontsize,
					il->args.bgSelected
				);
				DrawText(
					il->buf[il->bufndx],
					il->args.offsetHorz,
					il->args.totalVert / il->args.fontsize * i + il->args.offsetVert,
					il->args.fontsize,
					il->args.fgSelected);
			}
			// not selected
			else {
				const Color c = (Color){
					.r=il->args.fgNotSelected.r,
					.g=il->args.fgNotSelected.g,
					.b=il->args.fgNotSelected.b,
					.a=(int)(255.0 * (1 - ((double)abs(i) / (double)il->args.AB)))
				};
				DrawText(
					get_around(il->bufndx, i, (int)il->buflen, il->buf),
					il->args.offsetHorz,
					il->args.totalVert * 0.075 / 1.75 * i + il->args.offsetVert,
					il->args.fontsize,
					c);
			}
		}
	}
}

static char *get_around(int center, int AB, int max, char **buf)
{
	assert(buf);

	if (center + AB < 0 || center + AB >= max) {
		return NULL;
	}

	return buf[center + AB];
}

static char *find_next(Itemlist *il)
{
	int i;
	char *p;

	assert(il);

	if (il->bufndx + 1 < il->buflen) {
		il->bufndx += 1;
		return il->buf[il->bufndx];
	}

	il->bufndx = 0;
	return il->buf[il->bufndx];
}

static char *find_prev(Itemlist *il)
{
	int i;
	char *p;

	assert(il);

	if (il->bufndx - 1 >= 0) {
		il->bufndx -= 1;
		return il->buf[il->bufndx];
	}

	il->bufndx = il->buflen - 1;
	return il->buf[il->bufndx];
}

char *itemlist_get_selected(Itemlist *il)
{
	assert(il);

	return il->buf[il->bufndx];
}
