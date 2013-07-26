#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdbool.h>

#include <libconfig.h>

#include <src/tile.h>

#define LEN(x) (sizeof(x) / sizeof(x[0]))

extern int window_width;
extern int window_height;

extern int window_midw;
extern int window_midh;

extern double render_distance;

extern double offset_x;
extern double offset_y;

extern config_t config;
extern bool has_config;

extern tile_t *grid_tiles;

#endif
