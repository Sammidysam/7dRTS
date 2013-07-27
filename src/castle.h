#ifndef CASTLE_H
#define CASTLE_H

#include <src/tile.h>

typedef struct castle_t {
	tile_t **walls;
	int num_walls;
	tile_t **towers;
	int num_towers;

	player_t *owner;

	int wall_strength;
} castle_t;

castle_t *castle_new(player_t *owner, int num_walls, int num_towers);

void castle_new_wall(castle_t *castle, tile_t *tile);
void castle_new_tower(castle_t *castle, tile_t *tile);

#endif
