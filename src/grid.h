#ifndef GRID_H
#define GRID_H

#include <src/tile.h>

typedef struct grid_t {
	int width;
	int height;
} grid_t;

void grid_draw(grid_t *grid);

grid_t *grid_new();

void grid_tile_draw(grid_t *grid, tile_t *tile, double r);

#endif
