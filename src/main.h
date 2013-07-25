#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include <png.h>
#include <GL/glut.h>

#include "texture.h"
#include "player.h"
#include "menu.h"
#include "grid.h"
#include "global.h"

#define KEY_ENTER 13
#define KEY_CTRL_R 18
#define KEY_CTRL_S 19
#define KEY_CTRL_W 23
#define KEY_ESCAPE 27

#define DEFAULT_RENDER_DISTANCE 40.0

#define LEN(x) (sizeof(x) / sizeof(x[0]))

typedef enum direction_t {
	DIRECTION_UP,
	DIRECTION_DOWN,
	DIRECTION_LEFT,
	DIRECTION_RIGHT,

	DIRECTION_MAX
} direction_t;

void zoom_in();
void zoom_out();
void handle_mouse(int button, int state, int x, int y);
void _set_window_mids();
void handle_key_up(unsigned char key, int x, int y);
void handle_key_down(unsigned char key, int x, int y);
void load_textures();
void init_rendering();
void init_game();
void init_buttons();
void handle_resize(int w, int h);
void select(direction_t direction);
void update(int value);
void draw_screen();
void clean_up();
int main(int argc, char *argv[]);

#endif
