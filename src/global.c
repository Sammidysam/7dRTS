#include <src/global.h>

int window_width = 1024;
int window_height = 768;

int window_midw;
int window_midh;

double render_distance = 40.0;

double offset_x = 0.0;
double offset_y = 0.0;

config_t config;
bool has_config = true;

grid_t *grid;
tile_t *grid_tiles;
int grid_tiles_len;

int num_players = 2;

castle_t *castles;

player_t *players;
