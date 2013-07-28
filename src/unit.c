#include <stdlib.h>

#include <src/global.h>

#include <src/unit.h>

unit_t *unit_new(unit_type_t type)
{
	unit_t *unit = (unit_t *)calloc(1, sizeof(unit_t));

	unit->type = type;

	unit->attack = unit_attacks[type];
	unit->health = unit_healths[type];

	return unit;
}
