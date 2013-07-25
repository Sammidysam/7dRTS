#include "point.h"

point_t point_new(int x, int y)
{
	point_t point;
	point.x = x;
	point.y = y;

	return point;
}

point_t point_one_d_to_two_d(int one_d, int grid_width, int grid_height)
{
	point_t two_d;
	two_d.x = one_d % grid_width;
	two_d.y = one_d / grid_height;
	return two_d;
}

int point_two_d_to_one_d(point_t two_d, int grid_width, int grid_height)
{
	return two_d.y * grid_height + two_d.x;
}

int point_one_d_x(int one_d, int grid_width, int grid_height)
{
	return one_d % grid_width;
}

int point_one_d_y(int one_d, int grid_width, int grid_height)
{
	return one_d / grid_height;
}
