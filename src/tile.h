#ifndef TILE_H
#define TILE_H

#include <src/texture.h>
#include <src/player.h>
#include <src/point.h>

typedef enum tile_direction_t {
	TILE_DIRECTION_UP,
	TILE_DIRECTION_DOWN,
	TILE_DIRECTION_RIGHT,
	TILE_DIRECTION_LEFT,
	
	TILE_DIRECTION_UP_RIGHT,
	TILE_DIRECTION_DOWN_RIGHT,
	TILE_DIRECTION_DOWN_LEFT,
	TILE_DIRECTION_UP_LEFT,

	TILE_DIRECTION_MAX
} tile_direction_t;

typedef enum tile_type_t {
	TILE_TYPE_UNINITIALIZED,
	
	TILE_TYPE_GRASS,
	TILE_TYPE_FOREST,
	TILE_TYPE_WATER,
	TILE_TYPE_STONE,
	TILE_TYPE_FARM,
	TILE_TYPE_CASTLE_WALL,
	TILE_TYPE_CASTLE_TOWER,

	TILE_TYPE_FAKE,
	
	TILE_TYPE_MAX
} tile_type_t;

typedef struct tile_t {
	tile_type_t type;

	int data;
	
	GLuint texture;

	player_t *owner;

	point_t *location;
} tile_t;

tile_t *tile_new_empty_int(int point, int grid_width, int grid_height);
tile_t *tile_new_empty_ints(int x, int y);
tile_t *tile_new_empty_point(point_t *location);
tile_t *tile_new_from_type_int(tile_type_t type, int point, int grid_width, int grid_height);
tile_t *tile_new_from_type_ints(tile_type_t type, int x, int y);
tile_t *tile_new_from_type_point(tile_type_t type, point_t *location);

void tile_set_type(tile_t *tile, tile_type_t type);
tile_type_t tile_get_type(tile_t *tile);

void tile_set_owner(tile_t *tile, player_t *owner);
player_t *tile_get_owner(tile_t *tile);

point_t *tile_direction_add(tile_direction_t direction);

tile_t *tile_get_surrounding(tile_t *tile);

void initialize_board();

#endif
