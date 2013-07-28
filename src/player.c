#include <stdlib.h>

#include <src/player.h>

player_t *player_new(player_type_t type)
{
	player_t *player = (player_t *)calloc(1, sizeof(player_t));

	player->resources = (player_resources_t *)calloc(1, sizeof(player_resources_t));

	player->type = type;

	/* add initial resources */

	player->army_size = 1;

	/* add first army */

	return player;
}

void player_new_army(player_t *player, army_t *army)
{
	
}
