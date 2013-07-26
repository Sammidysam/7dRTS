#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include <png.h>
#include <GL/glut.h>
#include <libconfig.h>

#include <src/global.h>
#include <src/direction.h>
#include <src/menu.h>
#include <src/grid.h>
#include <src/draw_mode.h>
#include <src/texture.h>

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

int *mouse_zoom_in;
int mouse_zoom_in_len = 1;

int *mouse_zoom_out;
int mouse_zoom_out_len = 1;

int *quit_keys;
int quit_keys_len = 1;

int *move_up_keys;
int move_up_keys_len = 2;

int *move_down_keys;
int move_down_keys_len = 2;

int *move_left_keys;
int move_left_keys_len = 2;

int *move_right_keys;
int move_right_keys_len = 2;

int *zoom_in_keys;
int zoom_in_keys_len = 1;

int *zoom_out_keys;
int zoom_out_keys_len = 1;

int *reset_render_distance_keys;
int reset_render_distance_keys_len = 1;

int *confirm_selection_keys;
int confirm_selection_keys_len = 1;

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
			for (int i = 0; i < mouse_zoom_in_len; i++) {
				if (button == mouse_zoom_in[i])
					zoom_in();
			}
			for (int i = 0; i < mouse_zoom_out_len; i++) {
				if (button == mouse_zoom_out[i])
					zoom_out();
			}
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
	key_down[key] = false;
}

void handle_key_down(unsigned char key, int x, int y)
{
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
	
	/* this is to set the array of players in the game */
	// players = (player_t*)malloc(players * sizeof(player_t));
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
			for (int j = 0; j < quit_keys_len; j++) {
				if (i == quit_keys[j])
					exit(0);
			}

			for (int j = 0; j < move_up_keys_len; j++) {
				if (i == move_up_keys[j]) {
					if (draw_mode != DRAW_MODE_MENU)
						offset_y -= move_speed;
					else
						menu_select(DIRECTION_UP, &buttons, LEN(buttons));
				}
			}

			for (int j = 0; j < move_down_keys_len; j++) {
				if (i == move_down_keys[j]) {
					if (draw_mode != DRAW_MODE_MENU)
						offset_y += move_speed;
					else
						menu_select(DIRECTION_DOWN, &buttons, LEN(buttons));
				}
			}

			for (int j = 0; j < move_left_keys_len; j++) {
				if (i == move_left_keys[j]) {
					if (draw_mode != DRAW_MODE_MENU)
						offset_x += move_speed;
					else
						menu_select(DIRECTION_LEFT, &buttons, LEN(buttons));
				}
			}

			for (int j = 0; j < move_right_keys_len; j++) {
				if (i == move_right_keys[j]) {
					if (draw_mode != DRAW_MODE_MENU)
						offset_x -= move_speed;
					else
						menu_select(DIRECTION_RIGHT, &buttons, LEN(buttons));
				}
			}

			for (int j = 0; j < zoom_in_keys_len; j++) {
				if (i == zoom_in_keys[j] && draw_mode != DRAW_MODE_MENU)
					zoom_in();
			}

			for (int j = 0; j < zoom_out_keys_len; j++) {
				if (i == zoom_out_keys[j] && draw_mode != DRAW_MODE_MENU)
					zoom_out();
			}

			for (int j = 0; j < reset_render_distance_keys_len; j++) {
				if (i == reset_render_distance_keys[j] && draw_mode != DRAW_MODE_MENU)
					render_distance = default_render_distance;
			}

			for (int j = 0; j < confirm_selection_keys_len; j++) {
				if (i == confirm_selection_keys[j] && draw_mode == DRAW_MODE_MENU) {
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

	config_destroy(&config);
}

int main(int argc, char *argv[])
{
	// srand((unsigned int)time(NULL));

	/* initialize strings */
	/* allocate memory */
	name = (char*)malloc(1024 * sizeof(char));
	description = (char*)malloc(1024 * sizeof(char));
	new_game = (char*)malloc(1024 * sizeof(char));
	load_game = (char*)malloc(1024 * sizeof(char));
	how_to_play = (char*)malloc(1024 * sizeof(char));
	settings = (char*)malloc(1024 * sizeof(char));

	/* assign values */
	name = "7dRTS";
	description = "A submission for Mini Ludum Dare #44 7dRTS by Kristofer Rye (four04) and Sam Craig (Sammidysam)";
	new_game = "New Game";
	load_game = "Load Game";
	how_to_play = "How to Play";
	settings = "Settings";

	/* initialize other dynamic arrays */
	mouse_zoom_in = (int*)malloc(mouse_zoom_in_len * sizeof(int));
	mouse_zoom_in[0] = 3;

	mouse_zoom_out = (int*)malloc(mouse_zoom_in_len * sizeof(int));
	mouse_zoom_out[0] = 4;

	quit_keys = (int*)malloc(quit_keys_len * sizeof(int));
	quit_keys[0] = KEY_ESCAPE;

	move_up_keys = (int*)malloc(move_up_keys_len * sizeof(int));
	move_up_keys[0] = 'W';
	move_up_keys[1] = 'w';

	move_down_keys = (int*)malloc(move_down_keys_len * sizeof(int));
	move_down_keys[0] = 'S';
	move_down_keys[1] = 's';

	move_left_keys = (int*)malloc(move_left_keys_len * sizeof(int));
	move_left_keys[0] = 'A';
	move_left_keys[1] = 'a';

	move_right_keys = (int*)malloc(move_right_keys_len * sizeof(int));
	move_right_keys[0] = 'D';
	move_right_keys[1] = 'd';

	zoom_in_keys = (int*)malloc(zoom_in_keys_len * sizeof(int));
	zoom_in_keys[0] = KEY_CTRL_W;

	zoom_out_keys = (int*)malloc(zoom_out_keys_len * sizeof(int));
	zoom_out_keys[0] = KEY_CTRL_S;

	reset_render_distance_keys = (int*)malloc(reset_render_distance_keys_len * sizeof(int));
	reset_render_distance_keys[0] = KEY_CTRL_R;

	confirm_selection_keys = (int*)malloc(confirm_selection_keys_len * sizeof(int));
	confirm_selection_keys[0] = KEY_ENTER;

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
		config_setting_t *current_setting = 0;

		current_setting = config_lookup(&config, "window.title");

		if (config_setting_type(current_setting) == CONFIG_TYPE_STRING)
			name = config_setting_get_string(current_setting);

		current_setting = config_lookup(&config, "window.size.width");

		if (config_setting_type(current_setting) == CONFIG_TYPE_INT)
			window_width = config_setting_get_int(current_setting);

		current_setting = config_lookup(&config, "window.size.height");

		if (config_setting_type(current_setting) == CONFIG_TYPE_INT)
			window_height = config_setting_get_int(current_setting);

		current_setting = config_lookup(&config, "window.fullscreen");

		if (config_setting_type(current_setting) == CONFIG_TYPE_BOOL)
			fullscreen = config_setting_get_bool(current_setting);

		current_setting = config_lookup(&config, "menu.game_description");

		if (config_setting_type(current_setting) == CONFIG_TYPE_STRING)
			description = config_setting_get_string(current_setting);

		current_setting = config_lookup(&config, "menu.new_game_text");

		if (config_setting_type(current_setting) == CONFIG_TYPE_STRING)
			new_game = config_setting_get_string(current_setting);

		current_setting = config_lookup(&config, "menu.load_game_text");

		if (config_setting_type(current_setting) == CONFIG_TYPE_STRING)
			load_game = config_setting_get_string(current_setting);

		current_setting = config_lookup(&config, "menu.how_to_play_text");

		if (config_setting_type(current_setting) == CONFIG_TYPE_STRING)
			how_to_play = config_setting_get_string(current_setting);

		current_setting = config_lookup(&config, "menu.settings_text");

		if (config_setting_type(current_setting) == CONFIG_TYPE_STRING)
			settings = config_setting_get_string(current_setting);

		current_setting = config_lookup(&config, "menu.default_selected_button");

		if (config_setting_type(current_setting) == CONFIG_TYPE_INT)
			default_select_button = config_setting_get_int(current_setting);

		current_setting = config_lookup(&config, "control_handling.zoom_modifier");

		if (config_setting_type(current_setting) == CONFIG_TYPE_FLOAT)
			zoom = config_setting_get_float(current_setting);

		current_setting = config_lookup(&config, "control_handling.move_speed");

		if (config_setting_type(current_setting) == CONFIG_TYPE_FLOAT)
			move_speed = config_setting_get_float(current_setting);

		current_setting = config_lookup(&config, "control_handling.default_render_distance");

		if (config_setting_type(current_setting) == CONFIG_TYPE_FLOAT) {
			default_render_distance = config_setting_get_float(current_setting);
			render_distance = config_setting_get_float(current_setting);
		}

		current_setting = config_lookup(&config, "control_handling.zoom_in_mouse");

		if (config_setting_type(current_setting) == CONFIG_TYPE_LIST) {
			free(mouse_zoom_in);
			
			mouse_zoom_in_len = config_setting_length(current_setting);

			mouse_zoom_in = (int*)malloc(mouse_zoom_in_len * sizeof(int));
			for (int i = 0; i < mouse_zoom_in_len; i++) {
				char lookup_name [1024];

				sprintf(lookup_name, "control_handling.zoom_in_mouse.[%i]", i);
				
				current_setting = config_lookup(&config, lookup_name);

				if (config_setting_type(current_setting) == CONFIG_TYPE_INT)
					mouse_zoom_in[i] = config_setting_get_int(current_setting);
			}
		}

		current_setting = config_lookup(&config, "control_handling.zoom_out_mouse");

		if (config_setting_type(current_setting) == CONFIG_TYPE_LIST) {
			free(mouse_zoom_out);
			
			mouse_zoom_out_len = config_setting_length(current_setting);

			mouse_zoom_out = (int*)malloc(mouse_zoom_out_len * sizeof(int));
			for (int i = 0; i < mouse_zoom_out_len; i++) {
				char lookup_name [1024];

				sprintf(lookup_name, "control_handling.zoom_out_mouse.[%i]", i);
				
				current_setting = config_lookup(&config, lookup_name);

				if (config_setting_type(current_setting) == CONFIG_TYPE_INT)
					mouse_zoom_out[i] = config_setting_get_int(current_setting);
			}
		}

		current_setting = config_lookup(&config, "control_handling.quit_keys");

		if (config_setting_type(current_setting) == CONFIG_TYPE_LIST) {
			free(quit_keys);
			
			quit_keys_len = config_setting_length(current_setting);

			quit_keys = (int*)malloc(quit_keys_len * sizeof(int));
			for (int i = 0; i < quit_keys_len; i++) {
				char lookup_name [1024];

				sprintf(lookup_name, "control_handling.quit_keys.[%i]", i);
				
				current_setting = config_lookup(&config, lookup_name);

				if (config_setting_type(current_setting) == CONFIG_TYPE_STRING)
					quit_keys[i] = config_setting_get_string(current_setting)[0];
				else if (config_setting_type(current_setting) == CONFIG_TYPE_INT)
					quit_keys[i] = config_setting_get_int(current_setting);
			}
		}

		current_setting = config_lookup(&config, "control_handling.move_up_keys");

		if (config_setting_type(current_setting) == CONFIG_TYPE_LIST) {
			free(move_up_keys);
			
			move_up_keys_len = config_setting_length(current_setting);

			move_up_keys = (int*)malloc(move_up_keys_len * sizeof(int));
			for (int i = 0; i < move_up_keys_len; i++) {
				char lookup_name [1024];

				sprintf(lookup_name, "control_handling.move_up_keys.[%i]", i);
				
				current_setting = config_lookup(&config, lookup_name);

				if (config_setting_type(current_setting) == CONFIG_TYPE_STRING)
					move_up_keys[i] = config_setting_get_string(current_setting)[0];
				else if (config_setting_type(current_setting) == CONFIG_TYPE_INT)
					move_up_keys[i] = config_setting_get_int(current_setting);
			}
		}

		current_setting = config_lookup(&config, "control_handling.move_down_keys");

		if (config_setting_type(current_setting) == CONFIG_TYPE_LIST) {
			free(move_down_keys);
			
			move_down_keys_len = config_setting_length(current_setting);

			move_down_keys = (int*)malloc(move_down_keys_len * sizeof(int));
			for (int i = 0; i < move_down_keys_len; i++) {
				char lookup_name [1024];

				sprintf(lookup_name, "control_handling.move_down_keys.[%i]", i);
				
				current_setting = config_lookup(&config, lookup_name);

				if (config_setting_type(current_setting) == CONFIG_TYPE_STRING)
					move_down_keys[i] = config_setting_get_string(current_setting)[0];
				else if (config_setting_type(current_setting) == CONFIG_TYPE_INT)
					move_down_keys[i] = config_setting_get_int(current_setting);
			}
		}

		current_setting = config_lookup(&config, "control_handling.move_left_keys");

		if (config_setting_type(current_setting) == CONFIG_TYPE_LIST) {
			free(move_left_keys);
			
			move_left_keys_len = config_setting_length(current_setting);

			move_left_keys = (int*)malloc(move_left_keys_len * sizeof(int));
			for (int i = 0; i < move_left_keys_len; i++) {
				char lookup_name [1024];

				sprintf(lookup_name, "control_handling.move_left_keys.[%i]", i);
				
				current_setting = config_lookup(&config, lookup_name);

				if (config_setting_type(current_setting) == CONFIG_TYPE_STRING)
					move_left_keys[i] = config_setting_get_string(current_setting)[0];
				else if (config_setting_type(current_setting) == CONFIG_TYPE_INT)
					move_left_keys[i] = config_setting_get_int(current_setting);
			}
		}

		current_setting = config_lookup(&config, "control_handling.move_right_keys");

		if (config_setting_type(current_setting) == CONFIG_TYPE_LIST) {
			free(move_right_keys);
			
			move_right_keys_len = config_setting_length(current_setting);

			move_right_keys = (int*)malloc(move_right_keys_len * sizeof(int));
			for (int i = 0; i < move_right_keys_len; i++) {
				char lookup_name [1024];

				sprintf(lookup_name, "control_handling.move_right_keys.[%i]", i);
				
				current_setting = config_lookup(&config, lookup_name);

				if (config_setting_type(current_setting) == CONFIG_TYPE_STRING)
					move_right_keys[i] = config_setting_get_string(current_setting)[0];
				else if (config_setting_type(current_setting) == CONFIG_TYPE_INT)
					move_right_keys[i] = config_setting_get_int(current_setting);
			}
		}

		current_setting = config_lookup(&config, "control_handling.zoom_in_keys");

		if (config_setting_type(current_setting) == CONFIG_TYPE_LIST) {
			free(zoom_in_keys);
			
			zoom_in_keys_len = config_setting_length(current_setting);

			zoom_in_keys = (int*)malloc(zoom_in_keys_len * sizeof(int));
			for (int i = 0; i < zoom_in_keys_len; i++) {
				char lookup_name [1024];

				sprintf(lookup_name, "control_handling.zoom_in_keys.[%i]", i);
				
				current_setting = config_lookup(&config, lookup_name);

				if (config_setting_type(current_setting) == CONFIG_TYPE_STRING)
					zoom_in_keys[i] = config_setting_get_string(current_setting)[0];
				else if (config_setting_type(current_setting) == CONFIG_TYPE_INT)
					zoom_in_keys[i] = config_setting_get_int(current_setting);
			}
		}

		current_setting = config_lookup(&config, "control_handling.zoom_out_keys");

		if (config_setting_type(current_setting) == CONFIG_TYPE_LIST) {
			free(zoom_out_keys);
			
			zoom_out_keys_len = config_setting_length(current_setting);

			zoom_out_keys = (int*)malloc(zoom_out_keys_len * sizeof(int));
			for (int i = 0; i < zoom_out_keys_len; i++) {
				char lookup_name [1024];

				sprintf(lookup_name, "control_handling.zoom_out_keys.[%i]", i);
				
				current_setting = config_lookup(&config, lookup_name);

				if (config_setting_type(current_setting) == CONFIG_TYPE_STRING)
					zoom_out_keys[i] = config_setting_get_string(current_setting)[0];
				else if (config_setting_type(current_setting) == CONFIG_TYPE_INT)
					zoom_out_keys[i] = config_setting_get_int(current_setting);
			}
		}

		current_setting = config_lookup(&config, "control_handling.reset_render_distance_keys");

		if (config_setting_type(current_setting) == CONFIG_TYPE_LIST) {
			free(reset_render_distance_keys);
			
			reset_render_distance_keys_len = config_setting_length(current_setting);

			reset_render_distance_keys = (int*)malloc(reset_render_distance_keys_len * sizeof(int));
			for (int i = 0; i < reset_render_distance_keys_len; i++) {
				char lookup_name [1024];

				sprintf(lookup_name, "control_handling.reset_render_distance_keys.[%i]", i);
				
				current_setting = config_lookup(&config, lookup_name);

				if (config_setting_type(current_setting) == CONFIG_TYPE_STRING)
					reset_render_distance_keys[i] = config_setting_get_string(current_setting)[0];
				else if (config_setting_type(current_setting) == CONFIG_TYPE_INT)
					reset_render_distance_keys[i] = config_setting_get_int(current_setting);
			}
		}

		current_setting = config_lookup(&config, "control_handling.confirm_selection_keys");

		if (config_setting_type(current_setting) == CONFIG_TYPE_LIST) {
			free(confirm_selection_keys);
			
			confirm_selection_keys_len = config_setting_length(current_setting);

			confirm_selection_keys = (int*)malloc(confirm_selection_keys_len * sizeof(int));
			for (int i = 0; i < confirm_selection_keys_len; i++) {
				char lookup_name [1024];

				sprintf(lookup_name, "control_handling.confirm_selection_keys.[%i]", i);
				
				current_setting = config_lookup(&config, lookup_name);

				if (config_setting_type(current_setting) == CONFIG_TYPE_STRING)
					confirm_selection_keys[i] = config_setting_get_string(current_setting)[0];
				else if (config_setting_type(current_setting) == CONFIG_TYPE_INT)
					confirm_selection_keys[i] = config_setting_get_int(current_setting);
			}
		}
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
