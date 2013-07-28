#include <stdlib.h>

#include <src/global.h>

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

castle_t *castle_new_int(player_t *owner, int num_walls, int num_towers, int point)
{
	castle_t *castle = castle_new(owner, num_walls, num_towers);

	castle->primary_location = point_one_d_to_two_d(point, grid->width, grid->height);

	return castle;
}

castle_t *castle_new_ints(player_t *owner, int num_walls, int num_towers, int x, int y)
{
	castle_t *castle = castle_new(owner, num_walls, num_towers);

	castle->primary_location = point_new(x, y);

	return castle;
}

castle_t *castle_new_point(player_t *owner, int num_walls, int num_towers, point_t *point)
{
	castle_t *castle = castle_new(owner, num_walls, num_towers);

	castle->primary_location = point;

	return castle;
}

void castle_new_wall(castle_t *castle, tile_t *tile)
{
	tile_t **copy;
	*copy = *castle->walls;

	castle->num_walls++;
	castle->walls = (tile_t **)calloc(castle->num_walls, sizeof(tile_t *));

	for (int i = 0; i < castle->num_walls - 1; i++)
		castle->walls[i] = copy[i];

	castle->walls[castle->num_walls - 1] = tile;

	free(copy);
}

void castle_new_tower(castle_t *castle, tile_t *tile)
{
	tile_t **copy;
	*copy = *castle->towers;

	castle->num_towers++;
	castle->walls = (tile_t **)calloc(castle->num_towers, sizeof(tile_t *));

	for (int i = 0; i < castle->num_walls - 1; i++)
		castle->walls[i] = copy[i];

	castle->walls[castle->num_walls - 1] = tile;

	free(copy);
}

void castle_set_tiles(castle_t *castle)
{
	for (int i = 0; i < castle->num_walls; i++)
		tile_set_type(castle->walls[i], TILE_TYPE_CASTLE_WALL);

	for (int i = 0; i < castle->num_towers; i++)
		tile_set_type(castle->towers[i], TILE_TYPE_CASTLE_TOWER);
}
