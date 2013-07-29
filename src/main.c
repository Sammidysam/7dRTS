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
#include <src/castle.h>
#include <src/player.h>
#include <src/save_manager.h>

#include <src/main.h>

// normally good is: false for development, true for release
bool fullscreen = false;

double zoom = 0.5;
double move_speed = 0.3;

bool key_down [256];

char *name;
char *description;
char *art_credits;
char *new_game;
char *load_game;
char *how_to_play;
char *settings;

char *select_message;
char *no_saves;

char **save_list;

int selected_save = 0;

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
int tile_up_key = 'i';
int tile_down_key = 'k';
int tile_left_key = 'j';
int tile_right_key = 'l';

draw_mode_t draw_mode = DRAW_MODE_MENU;

int selected_grid_tile;

bool game_init = false;

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
		if (draw_mode == DRAW_MODE_IN_GAME) {
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
	players = (player_t *)calloc(num_players, sizeof(player_t));
	
	for (int i = 0; i < num_players; i++)
		players[i] = *player_new(i == 0 ? PLAYER_TYPE_HUMAN : PLAYER_TYPE_AI);

	castles = (castle_t *)calloc(num_players, sizeof(castle_t));
	
	/* init grid */
	grid = grid_new();
	grid->width = 80;
	grid->height = 60;

	selected_grid_tile = point_two_d_to_one_d(point_new(0, grid->height - 1), grid->width, grid->height);

	grid_tiles_len = grid->width * grid->height;
	grid_tiles = (tile_t *)calloc(grid_tiles_len, sizeof(tile_t));

	/* set tiles as empty */
	for (int i = 0; i < grid->width * grid->height; i++)
		grid_tiles[i] = *tile_new_empty_int(i, grid->width, grid->height);

	/* initialize board */
	initialize_board(grid->width, grid->height);

	/* create armies for each player */
	for (int i = 0; i < num_players; i++) {
		army_t *army = army_new(castles[i].primary_location);

		player_new_army(&players[i], army);
	}

	printf("drawing grid...\n");
	printf("\t,=grass\n");
	printf("\tT=forest\n");
	printf("\t~=water\n");
	printf("\t@=stone\n");
	printf("\t#=farm\n");
	printf("\t|=castle wall\n");
	printf("\t^=castle tower\n");
	printf("\n");
	for(int i = 0; i < grid->height; i++) {
		for(int j = 0; j < grid->width; j++) {
			char d;
			switch(grid_tiles[(i * grid->width) + j].type) {
			case TILE_TYPE_UNINITIALIZED: d = ' '; break;
			case TILE_TYPE_GRASS: d = ','; break;
			case TILE_TYPE_FOREST: d = 'T'; break;
			case TILE_TYPE_WATER: d = '~'; break;
			case TILE_TYPE_STONE: d = '@'; break;
			case TILE_TYPE_FARM: d = '#'; break;
			case TILE_TYPE_CASTLE_WALL: d = '|'; break;
			case TILE_TYPE_CASTLE_TOWER: d = '^'; break;
			case TILE_TYPE_FAKE: d = '_'; break;
			default: d = ' '; break;
			};
			putchar(d);
		}
		putchar('\n');
	}
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
				if (draw_mode != DRAW_MODE_MENU) {
					draw_mode = DRAW_MODE_MENU;
					key_down[i] = false;
				} else {
					exit(0);
				}
			} else if (i == move_up_key) {
				if (draw_mode == DRAW_MODE_IN_GAME)
					offset_y -= move_speed;
				else if (draw_mode == DRAW_MODE_MENU)
					menu_select(DIRECTION_UP, buttons, LEN(buttons));
				else if (draw_mode == DRAW_MODE_LOAD_GAME)
					save_manager_move_select(&selected_save, false);
			} else if (i == move_down_key) {
				if (draw_mode == DRAW_MODE_IN_GAME)
					offset_y += move_speed;
				else if (draw_mode == DRAW_MODE_MENU)
					menu_select(DIRECTION_DOWN, buttons, LEN(buttons));
				else if (draw_mode == DRAW_MODE_LOAD_GAME)
					save_manager_move_select(&selected_save, true);
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
						if (!game_init) {
							init_game();
							game_init = true;
						}
						draw_mode = DRAW_MODE_IN_GAME;
						break;
					case 1:
						draw_mode = DRAW_MODE_LOAD_GAME;
						save_manager_get_saves(save_list);
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
			} else if (i == tile_up_key) {
				if (point_add_safe(tile_direction_add(TILE_DIRECTION_UP), grid_tiles[selected_grid_tile].location)->y < grid->height)
					selected_grid_tile += grid->width;
				
				key_down[i] = false;
			} else if (i == tile_down_key) {
				if (point_add_safe(tile_direction_add(TILE_DIRECTION_DOWN), grid_tiles[selected_grid_tile].location)->y > -1)
					selected_grid_tile -= grid->width;
				
				key_down[i] = false;
			} else if (i == tile_left_key) {
				if (point_add_safe(tile_direction_add(TILE_DIRECTION_LEFT), grid_tiles[selected_grid_tile].location)->x > -1)
					selected_grid_tile--;
				
				key_down[i] = false;
			} else if (i == tile_right_key) {
				if (point_add_safe(tile_direction_add(TILE_DIRECTION_RIGHT), grid_tiles[selected_grid_tile].location)->x < grid->width)
					selected_grid_tile++;
				
				key_down[i] = false;
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
		menu_draw_text(name, description, art_credits);
		menu_draw_buttons(buttons, LEN(buttons));
		break;
	case DRAW_MODE_IN_GAME:
		grid_draw(grid, selected_grid_tile);
		for (int i = 0; i < num_players; i++) {
			for (int j = 0; j < players[i].army_size; j++) {
				/* draw army indicators */
			}
		} 
		break;
	case DRAW_MODE_LOAD_GAME:
		save_manager_draw_saves(save_list, select_message, no_saves, selected_save);
		break;
	case DRAW_MODE_HOW_TO_PLAY:
		/* implement */
	case DRAW_MODE_SETTINGS:
		/* implement */
	case DRAW_MODE_ERROR:
	default:
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, error_texture);
		{
			glBegin(GL_QUADS);

			glTexCoord2d(0.0, 0.0);
			glVertex3d(-20.0, -20.0, -(render_distance));

			glTexCoord2d(1.0, 0.0);
			glVertex3d(20.0, -20.0, -(render_distance));

			glTexCoord2d(1.0, 1.0);
			glVertex3d(20.0, 20.0, -(render_distance));

			glTexCoord2d(0.0, 1.0);
			glVertex3d(-20.0, 20.0, -(render_distance));
			
			glEnd();
		}
		glDisable(GL_TEXTURE_2D);
		break;
	}
	
	glutSwapBuffers();
}

void clean_up()
{
	free(players);

	free(name);
	free(description);
	free(art_credits);
	free(new_game);
	free(load_game);
	free(how_to_play);
	free(settings);
	
	free(select_message);
	free(no_saves);

	for (int i = 0; i < save_manager_get_save_number(); i++)
		free(save_list[i]);
	free(save_list);

	free(grid_tiles);
	free(grid);

	config_destroy(&config);
}

int main(int argc, char *argv[])
{
	srand((unsigned int)time(NULL));

	/* initialize strings */
	/* allocate memory */
	name = (char *)calloc(1024, sizeof(char));
	description = (char *)calloc(1024, sizeof(char));
	art_credits = (char *)calloc(1024, sizeof(char));
	new_game = (char *)calloc(1024, sizeof(char));
	load_game = (char *)calloc(1024, sizeof(char));
	how_to_play = (char *)calloc(1024, sizeof(char));
	settings = (char *)calloc(1024, sizeof(char));
	
	select_message = (char *)calloc(1024, sizeof(char));
	no_saves = (char *)calloc(1024, sizeof(char));

	/* assign values */
	name = "7dRTS";
	description = "A submission for Mini Ludum Dare #44 7dRTS by Kristofer Rye (four04) and Sam Craig (Sammidysam)";
	art_credits = "Art by Kristofer Rye (four04) and Michael Will";
	new_game = "New Game";
	load_game = "Load Game";
	how_to_play = "How to Play";
	settings = "Settings";
	
	select_message = "Please select a save file to play.";
	no_saves = "No saves detected!  Saves are found from the pattern \"saves/*.save\".";

	save_list = (char **)calloc(save_manager_get_save_number(), sizeof(char *));
	for (int i = 0; i < save_manager_get_save_number(); i++)
		save_list[i] = (char *)calloc(1024, sizeof(char));

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
		printf("parsing game.cfg...\n");
		
		/* load all settings */
		config_get_item_string(&name, "window.title");
		config_print_debug_string("name (window.title)", name);
		config_get_item_int(&window_width, "window.size.width");
		config_print_debug_int("window_width (window.size.width)", window_width);	
		config_get_item_int(&window_height, "window.size.height");
		config_print_debug_int("window_height (window.size.height)", window_height);
		config_get_item_bool(&fullscreen, "window.fullscreen");
		config_print_debug_bool("fullscreen (window.fullscreen)", fullscreen);
		config_get_item_string(&description, "menu.game_description");
		config_print_debug_string("description (menu.game_description)", description);
		config_get_item_string(&art_credits, "menu.art_description");
		config_print_debug_string("art_credits (menu.art_description)", art_credits);
		config_get_item_string(&new_game, "menu.new_game_text");
		config_print_debug_string("new_game (menu.new_game_text)", new_game);
		config_get_item_string(&load_game, "menu.load_game_text");
		config_print_debug_string("load_game (menu.load_game_text)", load_game);
		config_get_item_string(&how_to_play, "menu.how_to_play_text");
		config_print_debug_string("how_to_play (menu.how_to_play_text)", how_to_play);
		config_get_item_string(&settings, "menu.settings_text");
		config_print_debug_string("settings (menu.settings_text)", settings);
		config_get_item_int(&default_select_button, "menu.default_selected_button");
		config_print_debug_int("default_select_button (menu.default_selected_button)", default_select_button);

		config_get_item_string(&select_message, "saves.select_text");
		config_print_debug_string("select_message (saves.select_text)", select_message);
		config_get_item_string(&no_saves, "saves.no_saves_detected_text");
		config_print_debug_string("no_saves (saves.no_saves_detected_text)", no_saves);
				
		config_get_item_double(&zoom, "control_handling.zoom_modifier");
		config_print_debug_double("zoom (control_handling.zoom_modifier)", zoom);
		config_get_item_double(&move_speed, "control_handling.move_speed");
		config_print_debug_double("move_speed (control_handling.move_speed)", move_speed);
		config_get_item_double(&default_render_distance, "control_handling.default_render_distance");
		render_distance = default_render_distance;
		config_print_debug_double("default_render_distance (control_handling.default_render_distance)", default_render_distance);
		config_get_item_int(&mouse_zoom_in, "control_handling.zoom_in_mouse");
		config_print_debug_int("mouse_zoom_in (control_handling.zoom_in_mouse)", mouse_zoom_in);
		config_get_item_int(&mouse_zoom_out, "control_handling.zoom_out_mouse");
		config_print_debug_int("mouse_zoom_out (control_handling.zoom_out_mouse)", mouse_zoom_out);
		config_get_item_int(&quit_key, "control_handling.quit_key");
		config_print_debug_int("quit_key (control_handling.quit_key)", quit_key);
		config_get_item_int(&move_up_key, "control_handling.move_up_key");
		config_print_debug_int("move_up_key (control_handling.move_up_key)", move_up_key);
		config_get_item_int(&move_down_key, "control_handling.move_down_key");
		config_print_debug_int("move_down_key (control_handling.move_down_key)", move_down_key);
		config_get_item_int(&move_left_key, "control_handling.move_left_key");
		config_print_debug_int("move_left_key (control_handling.move_left_key)", move_left_key);
		config_get_item_int(&move_right_key, "control_handling.move_right_key");
		config_print_debug_int("move_right_key (control_handling.move_right_key)", move_right_key);
		config_get_item_int(&tile_up_key, "control_handling.tile_up_key");
		config_print_debug_int("tile_up_key (control_handling.tile_up_key)", tile_up_key);
		config_get_item_int(&tile_down_key, "control_handling.tile_down_key");
		config_print_debug_int("tile_down_key (control_handling.tile_down_key)", tile_down_key);
		config_get_item_int(&tile_left_key, "control_handling.tile_left_key");
		config_print_debug_int("tile_left_key (control_handling.tile_left_key)", tile_left_key);
		config_get_item_int(&tile_right_key, "control_handling.tile_right_key");
		config_print_debug_int("tile_right_key (control_handling.tile_right_key)", tile_right_key);
		config_get_item_int(&zoom_in_key, "control_handling.zoom_in_key");
		config_print_debug_int("zoom_in_key (control_handling.zoom_in_key)", zoom_in_key);
		config_get_item_int(&zoom_out_key, "control_handling.zoom_out_key");
		config_print_debug_int("zoom_out_key (control_handling.zoom_out_key)", zoom_out_key);
		config_get_item_int(&reset_key,"control_handling.reset_render_distance_key");
		config_print_debug_int("reset_key (control_handloing.reset_render_distance_key)", reset_key);
		config_get_item_int(&confirm_key, "control_handling.confirm_selection_key");
		config_print_debug_int("confirm_key (control_handling.confirm_selection_key)", confirm_key);

		config_get_item_int(&unit_attacks[0], "gameplay.units.attacks.swordsman");
		config_print_debug_int("unit_attacks[0] (gameplay.units.attacks.swordsman)", unit_attacks[0]);
		config_get_item_int(&unit_attacks[1], "gameplay.units.attacks.horseman");
		config_print_debug_int("unit_attacks[1] (gameplay.units.attacks.horseman)", unit_attacks[1]);
		config_get_item_int(&unit_attacks[2], "gameplay.units.attacks.knight");
		config_print_debug_int("unit_attacks[2] (gameplay.units.attacks.knight)", unit_attacks[2]);
		config_get_item_int(&unit_attacks[3], "gameplay.units.attacks.trebuchet");
		config_print_debug_int("unit_attacks[3] (gameplay.units.attacks.trebuchet)", unit_attacks[3]);
		config_get_item_int(&unit_attacks[4], "gameplay.units.attacks.archer");
		config_print_debug_int("unit_attacks[4] (gameplay.units.attacks.archer)", unit_attacks[4]);
		config_get_item_int(&unit_healths[0], "gameplay.units.healths.swordsman");
		config_print_debug_int("unit_healths[0] (gameplay.units.healths.swordsman)", unit_healths[0]);
		config_get_item_int(&unit_healths[1], "gameplay.units.healths.horseman");
		config_print_debug_int("unit_healths[1] (gameplay.units.healths.horseman)", unit_healths[1]);
		config_get_item_int(&unit_healths[2], "gameplay.units.healths.knight");
		config_print_debug_int("unit_healths[2] (gameplay.units.healths.knight)", unit_healths[2]);
		config_get_item_int(&unit_healths[3], "gameplay.units.healths.trebuchet");
		config_print_debug_int("unit_healths[3] (gameplay.units.healths.trebuchet)", unit_healths[3]);
		config_get_item_int(&unit_healths[4], "gameplay.units.healths.archer");
		config_print_debug_int("unit_healths[4] (gameplay.units.healths.archer)", unit_healths[4]);

		printf("\n");
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
