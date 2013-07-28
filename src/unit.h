#ifndef UNIT_H
#define UNIT_H

typedef enum unit_type_t {
	UNIT_TYPE_SWORDSMAN,
	UNIT_TYPE_HORSEMAN,
	UNIT_TYPE_KNIGHT,
	UNIT_TYPE_TREBUCHET,
	UNIT_TYPE_ARCHER,

	UNIT_TYPE_MAX
} unit_type_t;

typedef struct unit_t {
	unit_type_t type;

	int attack;
	int health;
} unit_t;

unit_t *unit_new(unit_type_t type);

#endif
