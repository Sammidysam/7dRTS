#ifndef ARMY_H
#define ARMY_H

#include <src/point.h>
#include <src/unit.h>

typedef struct army_t {
	point_t *location;

	int num_units;
	unit_t *units;
} army_t;

army_t *army_new(point_t *location);

void army_new_unit(army_t *army, unit_t *unit);

#endif
