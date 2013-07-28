#include <stdlib.h>

#include <src/player.h>

player_t *player_new(player_type_t type)
{
	player_t *player = (player_t *)calloc(1, sizeof(player_t));

	player->type = type;

	return player;
}
