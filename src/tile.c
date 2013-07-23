#include <stdlib.h>
#include <string.h>

#include <texture.h>

#include <tile.h>

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
	case TILE_TYPE_VOID:
		n->texture = grass_texture;
		break;
	default:
		n->texture = error_texture;
		break;
	};
}

tile_t *tile_new_empty()
{
	tile_t *n = (tile_t *)malloc(sizeof(tile_t));
}

tile_t *tile_new_from_type(tile_type_t type)
{
	tile_t *n = tile_new_empty();

	n->type = type;

	_tile_populate_texture_from_type(n);
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

void tile_set_next(tile_t *tile, tile_t *next)
{
	tile->next = next;
}

tile_t *tile_get_next(tile_t *tile)
{
	return tile->next;
}
