
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
	for (double i = -((double)grid->width / 2.0) + offset_x; i <= ((double)grid->width / 2.0) + offset_x; i += 1.0) {
		glBegin(GL_LINES);
		glVertex3d( i,	(grid->height / 2.0) + offset_y, -(render_distance));
		glVertex3d( i, -(grid->height / 2.0) + offset_y, -(render_distance));
		glEnd();
	}

	/* draw horizontal lines */
	for (double i = -((double)grid->height / 2.0) + offset_y; i <= ((double)grid->height / 2.0) + offset_y; i += 1.0) {
		glBegin(GL_LINES);
		glVertex3d( (grid->width / 2.0) + offset_x,	i, -(render_distance));
		glVertex3d(-(grid->width / 2.0) + offset_x,	i, -(render_distance));
		glEnd();
	}

	glEnable(GL_TEXTURE_2D);
	
	for(int i = 0; i < grid_tiles_len; i++)
		grid_tile_draw(grid, &grid_tiles[i], (render_distance));
	
	glDisable(GL_TEXTURE_2D);
}

grid_t *grid_new()
{
	grid_t *n = (grid_t *)calloc(1, sizeof(grid_t));

	return n;
}

void grid_tile_draw(grid_t *grid, tile_t *tile, double r)
{
	tile_t *grass = tile_new_from_type_ints(TILE_TYPE_GRASS, tile->location->x, tile->location->y);
	
	if (tile->type == TILE_TYPE_FOREST)
		grid_tile_draw(grid, grass, (render_distance + 0.0001));

	free(grass);
	
	double osx = (offset_x + -(grid->width / 2.0));
	double osy = (offset_y + -(grid->height / 2.0));

	double minx = (osx + tile->location->x);
	double miny = (osy + tile->location->y);

	double maxx = (minx + 1.0);
	double maxy = (miny + 1.0);
	
	glBindTexture(GL_TEXTURE_2D, tile->texture);
	{
		glBegin(GL_QUADS);

		glTexCoord2d( 0.0,  0.0);
		glVertex3d( minx,  miny, -r);

		glTexCoord2d( 1.0,  0.0);
		glVertex3d( maxx,  miny, -r);

		glTexCoord2d( 1.0,  1.0);
		glVertex3d( maxx,  maxy, -r);

		glTexCoord2d( 0.0,  1.0);
		glVertex3d( minx,  maxy, -r);
		
		glEnd();
	}
}
