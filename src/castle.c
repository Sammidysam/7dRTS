#include <stdlib.h>

#include <src/castle.h>

castle_t *castle_new(player_t *owner, int num_walls, int num_towers)
{
	castle_t *castle = (castle_t *)calloc(1, sizeof(castle_t));

	castle->walls = (tile_t **)calloc(num_walls, sizeof(tile_t *));
	castle->towers = (tile_t **)calloc(num_towers, sizeof(tile_t *));

	castle->num_walls = num_walls;
	castle->num_towers = num_towers;

	castle->owner = owner;

	/* adjust later */
	castle->wall_strength = 5;

	return castle;
}

void castle_new_wall(castle_t *castle, tile_t *tile)
{
	
}

void castle_new_tower(castle_t *castle, tile_t *tile)
{
	
}
