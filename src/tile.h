#ifndef TILE_H
#define TILE_H

#include <player.h>
#include <texture.h>

typedef enum tile_type_t {
	TILE_TYPE_GRASS,
	TILE_TYPE_FOREST,
	TILE_TYPE_WATER,
	TILE_TYPE_STONE,
	TILE_TYPE_FARM,
	TILE_TYPE_CASTLE_WALL,
	TILE_TYPE_CASTLE_TOWER,
	TILE_TYPE_VOID,
	
	TILE_TYPE_MAX
} tile_type_t;

typedef struct tile_t {
	tile_type_t type;

	int data;
	
	texture_t *texture;

	player_t *owner;
	
	struct tile_t *next;
} tile_t;

tile_t *tile_new_empty();
tile_t *tile_new_from_type(tile_type_t type);

void tile_set_type(tile_t *tile, tile_type_t type);
tile_type_t tile_get_type(tile_t *tile);

void tile_set_owner(tile_t *tile, player_t *owner);
player_t *tile_get_owner(tile_t *tile);

void tile_set_next(tile_t *tile, tile_t *next);
tile_t *tile_get_next(tile_t *tile);

tile_t root_tile;

#endif
