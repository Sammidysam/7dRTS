#include <src/point.h>

point_t *point_new(int x, int y)
{
	point_t *point = (point_t *)malloc(sizeof(point_t));
	
	point->x = x;
	point->y = y;

	return point;
}

point_t *point_one_d_to_two_d(int one_d, int grid_width, int grid_height)
{
	point_t *two_d = (point_t *)malloc(sizeof(point_t));
	
	two_d->x = one_d % grid_width;
	two_d->y = one_d / grid_height;
	
	return two_d;
}

int point_two_d_to_one_d(point_t *two_d, int grid_width, int grid_height)
{
	return two_d->y * grid_height + two_d->x;
}

int point_one_d_x(int one_d, int grid_width, int grid_height)
{
	return one_d % grid_width;
}

int point_one_d_y(int one_d, int grid_width, int grid_height)
{
	return one_d / grid_height;
}

bool point_equals(point_t *a_point, point_t *other_point)
{
	return (a_point->x == other_point->x && a_point->y == other_point->y) ? true : false;
}

void point_add(point_t *add_to, point_t *from)
{
	add_to->x += from->x;
	add_to->y += from->y;
}

void point_subtract(point_t *subtract_to, point_t *from)
{
	subtract_to->x -= from->x;
	subtract_to->y -= from->y;
}
