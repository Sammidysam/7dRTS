#ifndef PLAYER_H
#define PLAYER_H

typedef enum player_type_t {
	PLAYER_TYPE_HUMAN,
	PLAYER_TYPE_AI,
	
	PLAYER_TYPE_MAX
} player_type_t;

typedef struct player_t {
	player_type_t type;
	
	double approval_rating;
	long double food_units;
	long double army_units;
} player_t;

#endif
