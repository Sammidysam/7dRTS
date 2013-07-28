#include <stdbool.h>

#include <GL/glut.h>

#include <src/point.h>
#include <src/global.h>
#include <src/texture.h>
#include <src/grid.h>

void grid_draw(grid_t *grid, int selected_tile)
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
		grid_tile_draw(grid, &grid_tiles[i], (render_distance), selected_tile);
	
	glDisable(GL_TEXTURE_2D);
}

grid_t *grid_new()
{
	grid_t *n = (grid_t *)calloc(1, sizeof(grid_t));

	return n;
}

void grid_tile_draw(grid_t *grid, tile_t *tile, double r, int selected_tile)
{
	tile_t *grass = tile_new_from_type_ints(TILE_TYPE_GRASS, tile->location->x, tile->location->y);
	
	if (tile->type == TILE_TYPE_FOREST || tile->type == TILE_TYPE_CASTLE_WALL)
		grid_tile_draw(grid, grass, (render_distance + 0.0001), selected_tile);

	free(grass);

	double rotation_angle = 0.0;

	if (tile->type == TILE_TYPE_CASTLE_WALL) {
		/* get information about neighboring walls */
		bool in_direction [4];
		
		for (int i = 0; i < 4; i++) {
			point_t *add = point_add_safe(tile_direction_add(i), tile->location);
			
			int point = point_two_d_to_one_d(add, grid->width, grid->height);
			
			if (point > -1 && point < grid_tiles_len)
				in_direction[i] = grid_tiles[point].type == TILE_TYPE_CASTLE_WALL;

			free(add);
		}

		int directions = 0;
		for (int i = 0; i < 4; i++)
			if (in_direction[i])
				directions++;

		/* set correct texture and angle */
		switch (directions) {
		case 0: case 1:
			tile->texture = castle_wall_texture;
			
			if ((in_direction[TILE_DIRECTION_UP] || in_direction[TILE_DIRECTION_DOWN]) && !(in_direction[TILE_DIRECTION_LEFT] || in_direction[TILE_DIRECTION_RIGHT]))
				rotation_angle = 90.0;
			
			break;
		case 2:
			tile->texture = castle_corner_texture;
			
			if (in_direction[TILE_DIRECTION_LEFT] && in_direction[TILE_DIRECTION_UP])
				rotation_angle = 90.0;
			else if (in_direction[TILE_DIRECTION_LEFT] && in_direction[TILE_DIRECTION_DOWN])
				rotation_angle = 180.0;
			else if (in_direction[TILE_DIRECTION_RIGHT] && in_direction[TILE_DIRECTION_DOWN])
				rotation_angle = 270.0;
			else
				rotation_angle = 0.0;
			
			break;
		case 3:
			tile->texture = castle_three_texture;

			int not;
			for (int i = 0; i < 4; i++)
				if (!in_direction[i])
					not = i;

			switch (not) {
			case TILE_DIRECTION_UP:
				rotation_angle = 180.0;
				break;
			case TILE_DIRECTION_LEFT:
				rotation_angle = 270.0;
				break;
			case TILE_DIRECTION_RIGHT:
				rotation_angle = 90.0;
				break;
			case TILE_DIRECTION_DOWN:
			default:
				rotation_angle = 0.0;
				break;
			}
			
			break;
		case 4:
			tile->texture = castle_four_texture;
			break;
		default:
			tile->texture = castle_wall_texture;
			break;
		}
	}
	
	double osx = (offset_x + -(grid->width / 2.0));
	double osy = (offset_y + -(grid->height / 2.0));

	double minx = (osx + tile->location->x);
	double miny = (osy + tile->location->y);

	double maxx = (minx + 1.0);
	double maxy = (miny + 1.0);
	
	glBindTexture(GL_TEXTURE_2D, tile->texture);
	{
		if (point_two_d_to_one_d(tile->location, grid->width, grid->height) == selected_tile)
			glColor3d(1.0, 0.0, 0.0);
		
		if (tile->type == TILE_TYPE_CASTLE_WALL) {
			glPushMatrix();

			glTranslated(minx + 0.5, miny + 0.5, 0.0);
			
			glRotated(rotation_angle, 0.0, 0.0, 1.0);

			glTranslated(-(minx + 0.5), -(miny + 0.5), 0.0);
		}
		
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

		if (tile->type == TILE_TYPE_CASTLE_WALL)
			glPopMatrix();

		if (point_two_d_to_one_d(tile->location, grid->width, grid->height) == selected_tile)
			glColor3d(1.0, 1.0, 1.0);
	}
}
