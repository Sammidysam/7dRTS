#ifndef GRID_H
#define GRID_H

#include "tile.h"
#include "global.h"

typedef struct grid_t {
	double width;
	double height;

	tile_t *tiles[];
} grid_t;

typedef struct point_t {
	int x;
	int y;
} point_t;

// default uses global variables, custom uses argument
point_t make_point(int x, int y);

point_t one_d_to_two_d_default(int one_d);
int two_d_to_one_d_default(point_t two_d);
point_t one_d_to_two_d_custom(int one_d, grid_t custom_grid);
int two_d_to_one_d_custom(point_t two_d, grid_t custom_grid);
int one_d_x_default(int one_d);
int one_d_y_default(int one_d);
int one_d_x_custom(int one_d, grid_t custom_grid);
int one_d_y_custom(int one_d, grid_t custom_grid);

void draw_grid();

#endif
