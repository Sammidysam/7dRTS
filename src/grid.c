
#include <GL/glut.h>

#include <src/point.h>
#include <src/global.h>
#include <src/texture.h>
#include <src/grid.h>

void grid_draw(grid_t *grid)
{
	/* ensure correct line width */
	glLineWidth(1.0f);
	
	/* draw vertical lines */
	for(double i = -((double)grid->width / 2.0) + offset_x; i <= ((double)grid->width / 2.0) + offset_x; i += 1.0) {
		glBegin(GL_LINES);
		glVertex3d( i,	(grid->height / 2.0) + offset_y, -(render_distance));
		glVertex3d( i, -(grid->height / 2.0) + offset_y, -(render_distance));
		glEnd();
	}

	/* draw horizontal lines */
	for(double i = -((double)grid->height / 2.0) + offset_y; i <= ((double)grid->height / 2.0) + offset_y; i += 1.0) {
		glBegin(GL_LINES);
		glVertex3d( (grid->width / 2.0) + offset_x,	i, -(render_distance));
		glVertex3d(-(grid->width / 2.0) + offset_x,	i, -(render_distance));
		glEnd();
	}

	printf("error_texture = 0x%x\n", error_texture);
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, error_texture);
	{
		glBegin(GL_QUADS);
		
		glTexCoord2d( 0.0,  0.0);
		glVertex3d(-(grid->width / 2.0), -(grid->height / 2.0), -(render_distance));

		glTexCoord2d( 1.0,  0.0);
		glVertex3d( (grid->width / 2.0), -(grid->height / 2.0), -(render_distance));

		glTexCoord2d( 1.0,  1.0);
		glVertex3d( (grid->width / 2.0),  (grid->height / 2.0), -(render_distance));

		glTexCoord2d( 0.0,  1.0);
		glVertex3d(-(grid->width / 2.0),  (grid->height / 2.0), -(render_distance));
		
		glEnd();
	}
	glDisable(GL_TEXTURE_2D);
}

grid_t *grid_new()
{
	grid_t *n = (grid_t *)malloc(sizeof(grid_t));

	return n;
}
