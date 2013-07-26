#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>

#include <png.h>
#include <GL/glut.h>
#include <libconfig.h>

#include <src/global.h>
#include <src/direction.h>
#include <src/menu.h>
#include <src/grid.h>
#include <src/draw_mode.h>
#include <src/config.h>
#include <src/texture.h>
#include <src/point.h>

#include <src/main.h>

// normally good is: false for development, true for release
bool fullscreen = false;

double zoom = 0.5;
double move_speed = 0.3;

bool key_down [256];

char *name;
char *description;
char *new_game;
char *load_game;
char *how_to_play;
char *settings;

button_t buttons [4];

int default_select_button = 0;

double default_render_distance = 40.0;

int mouse_zoom_in = 3;
int mouse_zoom_out = 4;
int quit_key = KEY_ESCAPE;
int move_up_key = 'w';
int move_down_key = 's';
int move_left_key = 'a';
int move_right_key = 'd';
int zoom_in_key = KEY_CTRL_W;
int zoom_out_key = KEY_CTRL_S;
int reset_key = KEY_CTRL_R;
int confirm_key = KEY_ENTER;

grid_t *grid;

player_t *players;

draw_mode_t draw_mode = DRAW_MODE_MENU;

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
	if (state != GLUT_UP) { 
		if (draw_mode != DRAW_MODE_MENU) {
			if (button == mouse_zoom_in)
				zoom_in();
			else if (button == mouse_zoom_out)
				zoom_out();
			else
				printf("Unsupported button %d pressed\n", button);
		} else {
			switch(button) {
			case 3: case 4:
				printf("handle_mouse: button=%d, state=%d, x=%d, y=%d, on menu, ignoring\n", button, state, x, y);
				break;
			default:
				printf("handle_mouse: unsupported button\n");
				break;
			}
		}
	}
}

void _set_window_mids()
{
	window_midw = window_width / 2;
	window_midh = window_height / 2;
}

void handle_key_up(unsigned char key, int x, int y)
{
	if (isupper(key))
		key = tolower(key);
	
	key_down[key] = false;
}

void handle_key_down(unsigned char key, int x, int y)
{
	if (isupper(key))
		key = tolower(key);
	
	key_down[key] = true;
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
	/* init grid */
	grid = grid_new();
	grid->width = 8;
	grid->height = 6;

	grid_tiles = (tile_t *)malloc((grid->width * grid->height) * sizeof(tile_t));

	/* set tiles as empty */
	for (int i = 0; i < grid->width * grid->height; i++)
		grid_tiles[i] = *tile_new_empty_int(i, grid->width, grid->height);

	/* initialize board */
	for (int i = 0; i < grid->width * grid->height; i++) {
		
	}
	
	/* this is to set the array of players in the game */
	// players = (player_t *)malloc(players * sizeof(player_t));
}

void init_buttons()
{
	buttons[0].text = new_game;
	buttons[1].text = load_game;
	buttons[2].text = how_to_play;
	buttons[3].text = settings;
	
	buttons[default_select_button < LEN(buttons) ? default_select_button : 0].selected = true;
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

void update(int value)
{
	/* handle key presses */
	for (int i = 0; i < 256; i++) {
		if (key_down[i]) {
			if (i == quit_key) {
				exit(0);
			} else if (i == move_up_key) {
				if (draw_mode != DRAW_MODE_MENU)
					offset_y -= move_speed;
				else
					menu_select(DIRECTION_UP, buttons, LEN(buttons));
			} else if (i == move_down_key) {
				if (draw_mode != DRAW_MODE_MENU)
					offset_y += move_speed;
				else
					menu_select(DIRECTION_DOWN, buttons, LEN(buttons));
			} else if (i == move_left_key) {
				if (draw_mode != DRAW_MODE_MENU)
					offset_x += move_speed;
				else
					menu_select(DIRECTION_LEFT, buttons, LEN(buttons));
			} else if (i == move_right_key) {
				if (draw_mode != DRAW_MODE_MENU)
					offset_x -= move_speed;
				else
					menu_select(DIRECTION_RIGHT, buttons, LEN(buttons));
			} else if (i == zoom_in_key) {
				if (draw_mode != DRAW_MODE_MENU)
					zoom_in();
			} else if (i == zoom_out_key) {
				if (draw_mode != DRAW_MODE_MENU)
					zoom_out();
			} else if (i == reset_key) {
				if (draw_mode != DRAW_MODE_MENU)
					render_distance = default_render_distance;
			} else if (i == confirm_key) {
				if (draw_mode == DRAW_MODE_MENU) {
					int selected_button;
					for (int i = 0; i < LEN(buttons); i++)
						if (buttons[i].selected)
							selected_button = i;

					switch (selected_button) {
					case 0:
						init_game();
						draw_mode = DRAW_MODE_IN_GAME;
						break;
					case 1:
						draw_mode = DRAW_MODE_LOAD_GAME;
						break;
					case 2:
						draw_mode = DRAW_MODE_HOW_TO_PLAY;
						break;
					case 3:
						draw_mode = DRAW_MODE_SETTINGS;
						break;
					default:
						draw_mode = DRAW_MODE_ERROR;
						break;
					}
				}
			} else {
				printf("Unsupported key %d pressed\n", i);
			}
		}
	}
	
	glutPostRedisplay();
	glutTimerFunc(16, update, 0);
}

void draw_screen()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor3d(1.0, 1.0, 1.0);

	switch (draw_mode) {
	case DRAW_MODE_MENU:
		menu_draw_text(name, description);
		menu_draw_buttons(buttons, LEN(buttons));
		break;
	case DRAW_MODE_IN_GAME:
		grid_draw(grid);
		break;
	case DRAW_MODE_LOAD_GAME:
		/* implement */
		break;
	case DRAW_MODE_HOW_TO_PLAY:
		/* implement */
		break;
	case DRAW_MODE_SETTINGS:
		/* implement */
		break;
	case DRAW_MODE_ERROR: default:
		/* draw error texture or something */
		break;
	}
	
	glutSwapBuffers();
}

void clean_up()
{
	free(players);

	free(name);
	free(description);
	free(new_game);
	free(load_game);
	free(how_to_play);
	free(settings);

	free(grid_tiles);
	free(grid);

	config_destroy(&config);
}

int main(int argc, char *argv[])
{
	// srand((unsigned int)time(NULL));

	/* initialize strings */
	/* allocate memory */
	name = (char *)malloc(1024 * sizeof(char));
	description = (char *)malloc(1024 * sizeof(char));
	new_game = (char *)malloc(1024 * sizeof(char));
	load_game = (char *)malloc(1024 * sizeof(char));
	how_to_play = (char *)malloc(1024 * sizeof(char));
	settings = (char *)malloc(1024 * sizeof(char));

	/* assign values */
	name = "7dRTS";
	description = "A submission for Mini Ludum Dare #44 7dRTS by Kristofer Rye (four04) and Sam Craig (Sammidysam)";
	new_game = "New Game";
	load_game = "Load Game";
	how_to_play = "How to Play";
	settings = "Settings";

	/* set up config */
	config_init(&config);

	if (config_read_file(&config, "game.cfg") != CONFIG_TRUE) {
		printf("Error loading file game.cfg!\n");

		printf("%s\n", config_error_text(&config));
		printf("%s\n", config_error_file(&config));
		printf("%i\n", config_error_line(&config));

		has_config = false;

		/* if extra time, add generation of file */
	} else {
		/* load all settings */
		config_get_item_string(&name, "window.title");
		printf("value parsed: %s\n", name);
		config_get_item_int(&window_width, "window.size.width");
		printf("value parsed: %d\n", window_width);
		config_get_item_int(&window_height, "window.size.height");
		printf("value parsed: %d\n", window_height);
		config_get_item_bool(&fullscreen, "window.fullscreen");
		printf("value parsed: %s\n", (fullscreen ? "fullscreen" : "!fullscreen"));
		
		config_get_item_string(&description, "menu.game_description");
		printf("value parsed: %s\n", description);
		config_get_item_string(&new_game, "menu.new_game_text");
		printf("value parsed: %s\n", new_game);
		config_get_item_string(&load_game, "menu.load_game_text");
		printf("value parsed: %s\n", load_game);
		config_get_item_string(&how_to_play, "menu.how_to_play_text");
		printf("value parsed: %s\n", how_to_play);
		config_get_item_string(&settings, "menu.settings_text");
		printf("value parsed: %s\n", settings);
		config_get_item_int(&default_select_button, "menu.default_selected_button");
		printf("value parsed: %d\n", default_select_button);
				
		config_get_item_double(&zoom, "control_handling.zoom_modifier");
		printf("value parsed: %lf\n", zoom);
		config_get_item_double(&move_speed, "control_handling.move_speed");
		printf("value parsed: %lf\n", move_speed);
		config_get_item_double(&default_render_distance, "control_handling.default_render_distance");
		printf("value parsed: %lf\n", default_render_distance);
		render_distance = default_render_distance;
		config_get_item_int(&mouse_zoom_in, "control_handling.zoom_in_mouse");
		config_get_item_int(&mouse_zoom_out, "control_handling.zoom_out_mouse");
		config_get_item_int(&quit_key, "control_handling.quit_keys");
		config_get_item_int(&move_up_key, "control_handling.move_up_keys");
		config_get_item_int(&move_down_key, "control_handling.move_down_keys");
		config_get_item_int(&move_left_key, "control_handling.move_left_keys");
		config_get_item_int(&move_right_key, "control_handling.move_right_keys");
		config_get_item_int(&zoom_in_key, "control_handling.zoom_in_keys");
		config_get_item_int(&zoom_out_key, "control_handling.zoom_out_keys");
		config_get_item_int(&reset_key,"control_handling.reset_render_distance_keys");
		config_get_item_int(&confirm_key, "control_handling.confirm_selection_keys");
	}
	
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
	if (draw_mode != DRAW_MODE_MENU)
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
