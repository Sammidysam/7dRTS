#ifndef GRID_H
#define GRID_H

#include <src/tile.h>

typedef struct grid_t {
	int width;
	int height;

	tile_t *tiles[];
} grid_t;

void grid_draw();

#endif
