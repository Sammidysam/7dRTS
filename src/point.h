#ifndef POINT_H
#define POINT_H

#include <stdbool.h>

typedef struct point_t {
	int x;
	int y;
} point_t;

point_t *point_new(int x, int y);

point_t *point_one_d_to_two_d(int one_d, int grid_width, int grid_height);
int point_two_d_to_one_d(point_t *two_d, int grid_width, int grid_height);

int point_one_d_x(int one_d, int grid_width, int grid_height);
int point_one_d_y(int one_d, int grid_width, int grid_height);

bool point_equals(point_t *a_point, point_t *other_point);

point_t *point_add(point_t *add_to, point_t *from);
point_t *point_subtract(point_t *subtract_to, point_t *from);

point_t *point_distance(point_t *a_point, point_t *other_point);

#endif
