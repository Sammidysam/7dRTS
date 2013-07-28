#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdbool.h>

#include <libconfig.h>

#include <src/grid.h>
#include <src/tile.h>
#include <src/castle.h>

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

extern grid_t *grid;
extern tile_t *grid_tiles;
extern int grid_tiles_len;

extern int num_players;

extern castle_t *castles;

extern player_t *players;

#endif
