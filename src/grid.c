#include <GL/glut.h>

#include <src/point.h>
#include <src/global.h>
#include <src/grid.h>

void grid_draw(grid_t grid)
{
	/* ensure correct line width */
	glLineWidth(1.0f);
	
	/* draw vertical lines */
	for(double i = -((double)grid.width / 2.0) + offset_x; i <= ((double)grid.width / 2.0) + offset_x; i += 1.0) {
		glBegin(GL_LINES);
		glVertex3d( i,	(grid.height / 2.0) + offset_y, -(render_distance));
		glVertex3d( i, -(grid.height / 2.0) + offset_y, -(render_distance));
		glEnd();
	}

	/* draw horizontal lines */
	for(double i = -((double)grid.height / 2.0) + offset_y; i <= ((double)grid.height / 2.0) + offset_y; i += 1.0) {
		glBegin(GL_LINES);
		glVertex3d( (grid.width / 2.0) + offset_x,	i, -(render_distance));
		glVertex3d(-(grid.width / 2.0) + offset_x,	i, -(render_distance));
		glEnd();
	}
}
