#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <src/texture.h>
#include <src/global.h>

#include <src/tile.h>

void _tile_populate_texture_from_type(tile_t *n)
{
	switch(n->type) {
	case TILE_TYPE_GRASS:
		n->texture = grass_texture;
		break;
	case TILE_TYPE_FOREST:
		n->texture = forest_texture;
		break;
	case TILE_TYPE_WATER:
		n->texture = water_texture;
		break;
	case TILE_TYPE_STONE:
		n->texture = stone_texture;
		break;
	case TILE_TYPE_FARM:
		n->texture = farm_texture;
		break;
	case TILE_TYPE_CASTLE_WALL:
		n->texture = castle_wall_texture;
		break;
	case TILE_TYPE_CASTLE_TOWER:
		n->texture = castle_tower_texture;
		break;
	case TILE_TYPE_UNINITIALIZED:
		n->texture = uninitialized_texture;
		break;
	default:
		n->texture = error_texture;
		break;
	};
}

tile_t *tile_new_empty_int(int point, int grid_width, int grid_height)
{
	tile_t *n = (tile_t *)calloc(1, sizeof(tile_t));

	n->location = (point_t *)calloc(1, sizeof(point_t));

	n->type = TILE_TYPE_UNINITIALIZED;

	_tile_populate_texture_from_type(n);

	n->location->x = point_one_d_x(point, grid_width, grid_height);
	n->location->y = point_one_d_y(point, grid_width, grid_height);

	return n;
}

tile_t *tile_new_empty_ints(int x, int y)
{
	tile_t *n = (tile_t *)calloc(1, sizeof(tile_t));

	n->location = (point_t *)calloc(1, sizeof(point_t));

	n->type = TILE_TYPE_UNINITIALIZED;

	_tile_populate_texture_from_type(n);

	n->location->x = x;
	n->location->y = y;

	return n;
}

tile_t *tile_new_empty_point(point_t *location)
{
	tile_t *n = (tile_t *)calloc(1, sizeof(tile_t));

	n->location = (point_t *)calloc(1, sizeof(point_t));

	n->type = TILE_TYPE_UNINITIALIZED;

	_tile_populate_texture_from_type(n);

	n->location->x = location->x;
	n->location->y = location->y;

	return n;
}

tile_t *tile_new_from_type_int(tile_type_t type, int point, int grid_width, int grid_height)
{
	tile_t *n = tile_new_empty_int(point, grid_width, grid_height);

	n->type = type;

	_tile_populate_texture_from_type(n);

	return n;
}

tile_t *tile_new_from_type_ints(tile_type_t type, int x, int y)
{
	tile_t *n = tile_new_empty_ints(x, y);

	n->type = type;

	_tile_populate_texture_from_type(n);

	return n;
}

tile_t *tile_new_from_type_point(tile_type_t type, point_t *location)
{
	tile_t *n = tile_new_empty_point(location);

	n->type = type;

	_tile_populate_texture_from_type(n);

	return n;
}

void tile_set_type(tile_t *tile, tile_type_t type)
{
	tile->type = type;
	_tile_populate_texture_from_type(tile);
}

tile_type_t tile_get_type(tile_t *tile)
{
	return tile->type;
}

void tile_set_owner(tile_t *tile, player_t *owner)
{
	tile->owner = owner;
}

player_t *tile_get_owner(tile_t *tile)
{
	return tile->owner;
}

point_t *tile_direction_add(tile_direction_t direction)
{ 
	switch (direction) {
	case TILE_DIRECTION_UP:
		return point_new(0, 1);
	case TILE_DIRECTION_DOWN:
		return point_new(0, -1);
	case TILE_DIRECTION_RIGHT:
		return point_new(1, 0);
	case TILE_DIRECTION_LEFT:
		return point_new(-1, 0);
	case TILE_DIRECTION_UP_RIGHT:
		return point_new(1, 1);
	case TILE_DIRECTION_DOWN_RIGHT:
		return point_new(1, -1);
	case TILE_DIRECTION_DOWN_LEFT:
		return point_new(-1, -1);
	case TILE_DIRECTION_UP_LEFT:
		return point_new(-1, 1);
	default:
		printf("Invalid direction %d at function tile_direction_add\n", direction);
		return point_new(0, 0);
	}
}

tile_direction_t tile_direction_from_point(point_t *point)
{
	if (point->x == 0 && point->y == 1)
		return TILE_DIRECTION_UP;
	else if (point->x == 0 && point->y == -1)
		return TILE_DIRECTION_DOWN;
	else if (point->x == 1 && point->y == 0)
		return TILE_DIRECTION_RIGHT;
	else if (point->x == -1 && point->y == 0)
		return TILE_DIRECTION_LEFT;
	else if (point->x == 1 && point->y == 1)
		return TILE_DIRECTION_UP_RIGHT;
	else if (point->x == 1 && point->y == -1)
		return TILE_DIRECTION_DOWN_RIGHT;
	else if (point->x == -1 && point->y == -1)
		return TILE_DIRECTION_DOWN_LEFT;
	else if (point->x == -1 && point->y == 1)
		return TILE_DIRECTION_UP_LEFT;
	else
		return TILE_DIRECTION_MAX;
}

tile_t *tile_get_surrounding(tile_t *tile)
{
	tile_t *surrounding;
	surrounding = (tile_t *)calloc(8, sizeof(tile_t));

	for (int i = 0; i < 8; i++) {
		if (i == TILE_DIRECTION_MAX)
			break;
		
		point_t *location = point_add(tile_direction_add(i), tile->location);

		bool set_value = false;
		
		/* find if any tiles match location */
		for (int j = 0; j < grid_tiles_len; j++) {
			if (point_equals(grid_tiles[j].location, location)) {
				surrounding[i] = grid_tiles[j];
				set_value = true;
				break;
			}
		}

		if (!set_value)
			surrounding[i] = *tile_new_from_type_point(TILE_TYPE_FAKE, location);

		free(location);
	}
	
	return surrounding;
}

void initialize_board(int grid_width, int grid_height)
{
	/* create tile types */
	for (int i = 0; i < grid_tiles_len; i++) {
		tile_t *surrounding = tile_get_surrounding(&grid_tiles[i]);

		int num_real_initialized_tiles = 0;
		for (int j = 0; j < 8; j++)
			if (surrounding[j].type != TILE_TYPE_FAKE && surrounding[j].type != TILE_TYPE_UNINITIALIZED)
				num_real_initialized_tiles++;

		if (num_real_initialized_tiles != 8) {
			int result = rand() % 50;

			if (result < 30)
				tile_set_type(&grid_tiles[i], TILE_TYPE_GRASS);
			else if (result < 45)
				tile_set_type(&grid_tiles[i], TILE_TYPE_FOREST);
			else if (result < 49)
				tile_set_type(&grid_tiles[i], TILE_TYPE_STONE);
			else
				tile_set_type(&grid_tiles[i], TILE_TYPE_WATER);
		} else {
			tile_set_type(&grid_tiles[i], surrounding[rand() % 4].type);
		}

		free(surrounding);
	}

	/* create castles */
	for (int i = 0; i < num_players; i++) {
		point_t *location = point_new(rand() % grid_width, rand() % grid_height);
		for (int j = 0; j < i; j++) {
			point_t *distance = point_distance(location, )
		}
	}
}
