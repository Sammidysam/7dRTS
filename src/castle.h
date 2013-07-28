#ifndef CASTLE_H
#define CASTLE_H

#include <src/tile.h>
#include <src/point.h>

typedef struct castle_t {
	tile_t **walls;
	int num_walls;
	tile_t **towers;
	int num_towers;

	player_t *owner;

	int wall_strength;

	point_t *primary_location;
} castle_t;

castle_t *castle_new(player_t *owner, int num_walls, int num_towers);

castle_t *castle_new_int(player_t *owner, int num_walls, int num_towers, int point);
castle_t *castle_new_ints(player_t *owner, int num_walls, int num_towers, int x, int y);
castle_t *castle_new_point(player_t *owner, int num_walls, int num_towers, point_t *point);

void castle_new_wall(castle_t *castle, tile_t *tile);
void castle_new_tower(castle_t *castle, tile_t *tile);

void castle_set_tiles(castle_t *castle);

#endif
