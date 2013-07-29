#include <stdlib.h>

#include <src/army.h>

army_t *army_new(point_t *location)
{
	army_t *army = (army_t *)calloc(1, sizeof(army_t));

	army->location = location;

	return army;
}

void army_new_unit(army_t *army, unit_t *unit)
{
	army->num_units++;
	
	if (army->num_units == 1) {
		army->num_units = (army_t *)calloc(army->num_units, sizeof(unit_t));

		army->units[0] = *unit;
	} else {
		unit_t *copy;
		*copy = *army->units;

		army->units = (unit_t *)calloc(army->num_units, sizeof(unit_t));

		for (int i = 0; i < army->num_units - 1; i++)
			army->units[i] = copy[i];

		army->units[army->num_units - 1] = *unit;

		free(copy);
	}
}
