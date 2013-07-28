#ifndef PLAYER_H
#define PLAYER_H

#include <src/army.h>

typedef enum player_type_t {
	PLAYER_TYPE_HUMAN,
	PLAYER_TYPE_AI,
	
	PLAYER_TYPE_MAX
} player_type_t;

typedef struct player_resources_t {
	long double stone_units;
	long double wood_units;
	long double food_units;
} player_resources_t;

typedef struct player_t {
	player_type_t type;
	
	double approval_rating;
	player_resources_t *resources;

	army_t *armies;
	int army_size;
} player_t;

player_t *player_new(player_type_t type);

void player_new_army(player_t *player, army_t *army);

#endif
