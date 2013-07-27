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

tile_t *tile_get_surrounding(tile_t *tile)
{
	tile_t *surrounding;
	surrounding = (tile_t *)malloc(sizeof(tile_t) * 8);

	for (int i = 0; i < 8; i++) { 
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

		if (surrounding[i].type != TILE_TYPE_FAKE)
			printf("\tx=%d y=%d\n", surrounding[i].location->x, surrounding[i].location->y);
	}
	
	return surrounding;
}

void initialize_board()
{
	
}
