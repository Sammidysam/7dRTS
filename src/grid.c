#include "grid.h"

void draw_grid(grid_t grid)
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
