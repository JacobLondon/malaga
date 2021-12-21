/**
 * Have a collection of items which can be scrolled through
 * and selected.
 */
#ifndef RLU_ITEMLIST_H
#define RLU_ITEMLIST_H

#include <rlu/raylib.h>
#include <stdlib.h>

typedef struct ItemlistArgsStruct
{
	int fontsize; // size of the font
	int AB; // number above/below to show of the list
	int offsetVert; // location to put highlighted from top of screen in pixels
	int offsetHorz; // location to put text from left of screen in pixels
	int offsetX; // location to put highlighted from left of screen in pixels
	int totalVert; // total height of the area
	int totalHorz; // total width of the area
	Color bgSelected;
	Color fgSelected;
	Color fgNotSelected;
	const char *search;
} ItemlistArgs;

// THIS IS ALL PRIVATE DON'T TOUCH
typedef struct ItemlistStruct
{
	ItemlistArgs args;
	char **buf;
	size_t buflen;
	int bufndx;
	struct parray *filtered; // buf points to in here
	bool dirset;
} Itemlist;

Itemlist *itemlist_new(ItemlistArgs *args);
void itemlist_delete(Itemlist *il);

void itemlist_set(Itemlist *il, char **buf, size_t len);
bool itemlist_set_directory(Itemlist *il, const char *directory); // true if dir exists, false otherwise
bool itemlist_try_select(Itemlist *il, const char *name); // try to select this item
char *itemlist_next(Itemlist *il);
char *itemlist_prev(Itemlist *il);
void itemlist_draw(Itemlist *il);
char *itemlist_get_selected(Itemlist *il);

#endif // RLU_ITEMLIST_H
