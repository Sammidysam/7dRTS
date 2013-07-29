#include <stdlib.h>

#include <src/player.h>

player_t *player_new(player_type_t type)
{
	player_t *player = (player_t *)calloc(1, sizeof(player_t));

	player->resources = (player_resources_t *)calloc(1, sizeof(player_resources_t));

	player->type = type;

	player->disapproval_rating = 0.0;
	player->resources->stone_units = 7.0;
	player->resources->wood_units = 5.0;
	player->resources->food_units = 0.0;

	player->army_size = 0;

	return player;
}

void player_new_army(player_t *player, army_t *army)
{
	player->army_size++;
	
	if (player->army_size == 1) {
		player->armies = (army_t *)calloc(player->army_size, sizeof(army_t));

		player->armies[0] = *army;
	} else {
		army_t *copy;
		*copy = *player->armies;

		player->armies = (army_t *)calloc(player->army_size, sizeof(army_t));

		for (int i = 0; i < player->army_size - 1; i++)
			player->armies[i] = copy[i];

		player->armies[player->army_size - 1] = *army;

		free(copy);
	}
}
