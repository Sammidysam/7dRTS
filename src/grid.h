#ifndef GRID_H
#define GRID_H

#include <tile.h>

typedef struct grid_t {
	double width;
	double height;

	tile_t *tiles[];
} grid_t;

void grid_draw();

#endif
