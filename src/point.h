#ifndef POINT_H
#define POINT_H

typedef struct point_t {
	int x;
	int y;
} point_t;

point_t make_point(int x, int y);

point_t one_d_to_two_d(int one_d, int grid_width, int grid_height);
int two_d_to_one_d(point_t two_d, int grid_width, int grid_height);

int one_d_x(int one_d, int grid_width, int grid_height);
int one_d_y(int one_d, int grid_width, int grid_height);

#endif
