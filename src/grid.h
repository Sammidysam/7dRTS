#ifndef GRID_H
#define GRID_H

#include <src/tile.h>

#define TILE_BORDER_WIDTH 5.0f

typedef struct grid_t {
	int width;
	int height;
} grid_t;

void grid_draw(grid_t *grid, int selected_tile);

grid_t *grid_new();

void grid_tile_draw(grid_t *grid, tile_t *tile, double r, int selected_tile);

#endif
