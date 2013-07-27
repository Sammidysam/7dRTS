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
	tile_t *n = (tile_t *)malloc(sizeof(tile_t));

	n->location = (point_t *)malloc(sizeof(point_t));

	n->type = TILE_TYPE_UNINITIALIZED;

	_tile_populate_texture_from_type(n);

	point_t *location = point_one_d_to_two_d(point, grid_width, grid_height);

	n->location->x = location->x;
	n->location->y = location->y;

	return n;
}

tile_t *tile_new_empty_ints(int x, int y)
{
	tile_t *n = (tile_t *)malloc(sizeof(tile_t));

	n->location = (point_t *)malloc(sizeof(point_t));

	n->type = TILE_TYPE_UNINITIALIZED;

	_tile_populate_texture_from_type(n);

	n->location->x = x;
	n->location->y = y;

	return n;
}

tile_t *tile_new_empty_point(point_t *location)
{
	tile_t *n = (tile_t *)malloc(sizeof(tile_t));

	n->location = (point_t *)malloc(sizeof(point_t));

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
	surrounding = (tile_t *)malloc(sizeof(tile_t) * 8);

	for (int i = 0; i < 8; i++) {
		if (i == TILE_DIRECTION_MAX)
			break;
		
		point_t location = *point_add(tile_direction_add(i), tile->location);

		bool set_value = false;
		
		/* find if any tiles match location */
		for (int j = 0; j < grid_tiles_len; j++) {
			if (point_equals(grid_tiles[j].location, &location)) {
				surrounding[i] = grid_tiles[j];
				set_value = true;
			}
		}

		if (!set_value)
			surrounding[i] = *tile_new_from_type_point(TILE_TYPE_FAKE, &location);
	}
	
	return surrounding;
}

void initialize_board(int grid_width, int grid_height)
{
	for (int i = 0; i < grid_tiles_len; i++) { 
		tile_t *surrounding = tile_get_surrounding(&grid_tiles[i]);

		int real_initialized_tiles = 0;
		for (int j = 0; j < 8; j++) {
			if (surrounding[j].type != TILE_TYPE_FAKE && surrounding[j].type != TILE_TYPE_UNINITIALIZED)
				real_initialized_tiles++;
		}

		if (real_initialized_tiles == 0 && grid_tiles[i].type == TILE_TYPE_UNINITIALIZED) {
			tile_set_type(&grid_tiles[i], rand() % 4 + 1);
		} else {
			/* get surrounding types */
			bool grass_surrounds = false;
			bool forest_surrounds = false;
			bool water_surrounds = false;
			bool stone_surrounds = false;
			for (int j = 0; j < 8; j++) {
				if (surrounding[j].type != TILE_TYPE_FAKE && surrounding[j].type != TILE_TYPE_UNINITIALIZED) {
					switch (surrounding[j].type) {
					case TILE_TYPE_GRASS:
						grass_surrounds = true;
						break;
					case TILE_TYPE_FOREST:
						forest_surrounds = true;
						break;
					case TILE_TYPE_WATER:
						water_surrounds = true;
						break;
					case TILE_TYPE_STONE:
						stone_surrounds = true;
						break;
					default:
						printf("unexpected tile type %d at initialize_board\n", surrounding[j].type);
						break;
					}
				}
			}

			/* first, we want to continue lakes if there is one */
			if (water_surrounds) {
				for (int j = 0; j < 8; j++) {
					if (surrounding[j].type == TILE_TYPE_WATER) {
						for (int k = 0; k < 4; k++) {
							int index = point_two_d_to_one_d(point_add(surrounding[j].location, tile_direction_add(i)), grid_width, grid_height);

							/* we don't want a lake of 3 straight tiles, that's ugly and river-like */
							if (grid_tiles[index].type == TILE_TYPE_WATER && tile_direction_from_point(point_subtract(grid_tiles[index].location, surrounding[j].location)) != tile_direction_from_point(point_subtract(surrounding[j].location, grid_tiles[i].location)) && grid_tiles[i].type == TILE_TYPE_UNINITIALIZED)
								tile_set_type(&grid_tiles[i], TILE_TYPE_WATER);
						}

						if (grid_tiles[i].type == TILE_TYPE_UNINITIALIZED && tile_direction_from_point(point_subtract(surrounding[j].location, grid_tiles[i].location)) < TILE_DIRECTION_UP_RIGHT)
							tile_set_type(&grid_tiles[i], TILE_TYPE_WATER);
					}
				}
			} else if (forest_surrounds) {
				for (int j = 0; j < 8; j++) {
					if (surrounding[j].type == TILE_TYPE_FOREST && tile_direction_from_point(point_subtract(surrounding[j].location, grid_tiles[i].location)) < TILE_DIRECTION_UP_RIGHT && grid_tiles[i].type == TILE_TYPE_UNINITIALIZED)
						tile_set_type(&grid_tiles[i], TILE_TYPE_FOREST);
				}
			} else if (!stone_surrounds && grid_tiles[i].type == TILE_TYPE_UNINITIALIZED) {
				tile_set_type(&grid_tiles[i], TILE_TYPE_STONE);
			}
		}

		if (grid_tiles[i].type == TILE_TYPE_UNINITIALIZED)
			tile_set_type(&grid_tiles[i], TILE_TYPE_GRASS);
	}
}
