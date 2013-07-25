#include "grid.h"

point_t make_point(int x, int y)
{
	point_t point;
	point.x = x;
	point.y = y;
	return point;
}

point_t one_d_to_two_d_default(int one_d)
{
	point_t two_d;
	two_d.x = one_d % (int)grid.width;
	two_d.y = one_d / grid.height;
	return two_d;
}

int two_d_to_one_d_default(point_t two_d)
{
	return two_d.y * grid.height + two_d.x;
}

point_t one_d_to_two_d_custom(int one_d, grid_t custom_grid)
{
	point_t two_d;
	two_d.x = one_d % (int)custom_grid.width;
	two_d.y = one_d / custom_grid.height;
	return two_d;
}

int two_d_to_one_d_custom(point_t two_d, grid_t custom_grid)
{
	return two_d.y * custom_grid.height + two_d.x;
}

int one_d_x_default(int one_d)
{
	return one_d % (int)grid.width;
}

int one_d_y_default(int one_d)
{
	return one_d / grid.height;
}

int one_d_x_custom(int one_d, grid_t custom_grid)
{
	return one_d % (int)custom_grid.width;
}

int one_d_y_custom(int one_d, grid_t custom_grid)
{
	return one_d / custom_grid.height;
}

void draw_grid()
{
	/* draw vertical lines */
	for(double i = -(grid.width / 2.0) + offset_x; i <= (grid.width / 2.0) + offset_x; i += 1.0) {
		glBegin(GL_LINES);
		glVertex3d( i,	(grid.height / 2.0) + offset_y, -(render_distance));
		glVertex3d( i, -(grid.height / 2.0) + offset_y, -(render_distance));
		glEnd();
	}

	/* draw horizontal lines */
	for(double i = -(grid.height / 2.0) + offset_y; i <= (grid.height / 2.0) + offset_y; i += 1.0) {
		glBegin(GL_LINES);
		glVertex3d( (grid.width / 2.0) + offset_x,	i, -(render_distance));
		glVertex3d(-(grid.width / 2.0) + offset_x,	i, -(render_distance));
		glEnd();
	}
}
