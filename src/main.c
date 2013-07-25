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

#define KEY_CTRL_R 18
#define KEY_CTRL_S 19
#define KEY_CTRL_W 23
#define KEY_ESCAPE 27

#define DEFAULT_RENDER_DISTANCE 40.0

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

// normally good is: false for development, true for release
bool fullscreen = false;

int window_width = 1024;
int window_height = 768;

int window_midw;
int window_midh;

double render_distance = 40.0;

double grid_width = 8.0;
double grid_height = 6.0;

double zoom = 0.5;
double move_speed = 0.3;

double offset_x = 0.0;
double offset_y = 0.0;

bool key_down [256];

bool on_menu = true;

char *name = "7dRTS";
char *description = "A submission for Mini Ludum Dare #44 7dRTS by Kristofer Rye (four04) and Sam Craig (Sammidysam)";
char *new_game = "New Game";
char *load_game = "Load Game";
char *how_to_play = "How to Play";
char *settings = "Settings";

button_t buttons [4];

player_t *players;

void zoom_in()
{
	render_distance -= zoom;
}

void zoom_out()
{
	render_distance += zoom;
}

void handle_mouse(int button, int state, int x, int y)
{ 
	/* scroll wheel buttons */
	/* the numbers (3, 4 below) should be in the config to be safe */
	if (state != GLUT_UP) { 
		switch (button) {
		case 3:
			if (!on_menu)
				zoom_in();
			break;
		case 4:
			if (!on_menu)
				zoom_out();
			break;
		}
	}
}

void _set_window_mids(void)
{
	window_midw = window_width / 2;
	window_midh = window_height / 2;
}

void handle_key_up(unsigned char key, int x, int y)
{
	key_down[key] = false;
}

void handle_key_down(unsigned char key, int x, int y)
{
	key_down[key] = true;
}

void load_textures()
{
	grass_texture = texture_new_from_filename("img/grass.png");
	forest_texture = texture_new_from_filename("img/tree/oak.png");
	water_texture = texture_new_from_filename("img/water.png");
	stone_texture = texture_new_from_filename("img/stone.png");
	farm_texture = texture_new_from_filename("img/farm.png");
	castle_wall_texture = texture_new_from_filename("img/castle/wall/straight.png");
	castle_tower_texture = texture_new_from_filename("img/castle/tower.png");
}

void init_rendering()
{
	glShadeModel(GL_SMOOTH);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	load_textures();
}

void init_game()
{
	/* this is to set the array of players in the game */
	// players = (player_t*)malloc(players * sizeof(player_t));
}

void init_buttons()
{
	buttons[0].text = new_game;
	buttons[1].text = load_game;
	buttons[2].text = how_to_play;
	buttons[3].text = settings;

	buttons[0].selected = true;
}

void handle_resize(int w, int h)
{
	window_width = w;
	window_height = h;
	_set_window_mids();
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (double)w / (double)h, 1.0, 200.0);
}

void select(int direction)
{
	layout_t layout = get_button_layout(sizeof(buttons) / sizeof(buttons[0]));

	if (layout.type == LAYOUT_TYPE_ERROR)
		return;

	int selected_button;
	for (int i = 0; i < sizeof(buttons) / sizeof(buttons[0]); i++)
		if (buttons[i].selected)
			selected_button = i;

	int selected_button_x = selected_button % layout.rows;
	int selected_button_y = selected_button / layout.columns;

	switch (direction) {
	case UP:
		if (selected_button_y > 0) {
			buttons[selected_button].selected = false;
			buttons[(selected_button_y - 1) * layout.columns + selected_button_x].selected = true;
		}
		break;
	case DOWN:
		if (selected_button_y < layout.columns - 1) {
			buttons[selected_button].selected = false;
			buttons[(selected_button_y + 1) * layout.columns + selected_button_x].selected = true;
		}
		break;
	case LEFT:
		if (selected_button_x > 0) {
			buttons[selected_button].selected = false;
			buttons[selected_button_y * layout.columns + (selected_button_x - 1)].selected = true;
		}
		break;
	case RIGHT:
		if (selected_button_x < layout.rows - 1) {
			buttons[selected_button].selected = false;
			buttons[selected_button_y * layout.columns + (selected_button_x + 1)].selected = true;
		}
		break;
	}
}

void update(int value)
{
	/* handle key presses */
	for (int i = 0; i < 256; i++) {
		if (key_down[i]) {
			switch (i) {
			case KEY_ESCAPE:
				exit(0);
				break;
			/* keyboard movement keys should be customizable */
			case 'W': case 'w':
				/* move up */
				if (!on_menu)
					offset_y -= move_speed;
				else
					select(UP);
				break;
		   case 'S': case 's':
				/* move down */
			   if (!on_menu)
				   offset_y += move_speed;
			   else
				   select(DOWN);
			   break;
			case 'A': case 'a':
				/* move left */
				if (!on_menu)
					offset_x += move_speed;
				else
					select(LEFT);
				break;
			case 'D': case 'd':
				/* move right */
				if (!on_menu)
					offset_x -= move_speed;
				else
					select(RIGHT);
				break;
			case KEY_CTRL_W:
				if (!on_menu)
					zoom_in();
				break;
			case KEY_CTRL_S:
				if (!on_menu)
					zoom_out();
				break;
			case KEY_CTRL_R:
				if (!on_menu)
					render_distance = DEFAULT_RENDER_DISTANCE;
				break;
			}
		}
	}
	
	glutPostRedisplay();
	glutTimerFunc(16, update, 0);
}

void draw_grid()
{
	/* draw vertical lines */
	for(double i = -(grid_width / 2.0) + offset_x; i <= (grid_width / 2.0) + offset_x; i += 1.0) {
		glBegin(GL_LINES);
		glVertex3d( i,	(grid_height / 2.0) + offset_y, -(render_distance));
		glVertex3d( i, -(grid_height / 2.0) + offset_y, -(render_distance));
		glEnd();
	}

	/* draw horizontal lines */
	for(double i = -(grid_height / 2.0) + offset_y; i <= (grid_height / 2.0) + offset_y; i += 1.0) {
		glBegin(GL_LINES);
		glVertex3d( (grid_width / 2.0) + offset_x,	i, -(render_distance));
		glVertex3d(-(grid_width / 2.0) + offset_x,	i, -(render_distance));
		glEnd();
	}
}

void draw_screen()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor3d(1.0, 1.0, 1.0);

	if (!on_menu) {
		draw_grid();
	} else {
		draw_menu_text(name, description);
		draw_menu_buttons(buttons, sizeof(buttons) / sizeof(buttons[0]));
	}
	
	glutSwapBuffers();
}

void clean_up()
{
	free(players);
}

int main(int argc, char *argv[])
{
	// srand((unsigned int)time(NULL));
	
	glutInit(&argc, argv);
	
	if (fullscreen) {
		window_width = glutGet(GLUT_SCREEN_WIDTH) != 0 ? glutGet(GLUT_SCREEN_WIDTH) : window_width;
		window_height = glutGet(GLUT_SCREEN_HEIGHT) != 0 ? glutGet(GLUT_SCREEN_HEIGHT) : window_height;
	}
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(window_width, window_height);
	_set_window_mids();
	glutCreateWindow(name);

	if (fullscreen)
		glutFullScreen();

	init_rendering();
	if (!on_menu)
		init_game();
	else
		init_buttons();

	glutMouseFunc(handle_mouse);
	glutDisplayFunc(draw_screen);
	glutKeyboardFunc(handle_key_down);
	glutKeyboardUpFunc(handle_key_up);
	glutReshapeFunc(handle_resize);
	glutTimerFunc(16, update, 0);
	glutMainLoop();

	clean_up();

	return 0;
}
