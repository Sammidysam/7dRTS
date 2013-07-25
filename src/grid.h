#ifndef GRID_H
#define GRID_H

#include <GL/glut.h>

#include <tile.h>
#include <point.h>
#include <global.h>

typedef struct grid_t {
	double width;
	double height;

	tile_t *tiles[];
} grid_t;

void draw_grid();

#endif
