#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "texture.h"

#include <png.h>
#include <GL/glut.h>

#define KEY_ESCAPE 27

// normally good is: 0 for development, 1 for release
int fullscreen = 0;

int window_width = 1024;
int window_height = 768;

int window_midw;
int window_midh;

double render_distance = 40.0;

double zoom = 0.5;

double offset_x = 0.0;
double offset_y = 0.0;

void handle_mouse(int button, int state, int x, int y)
{
	/* scroll wheel buttons */
	/* the numbers should be in the config to be safe */
	if (state != GLUT_UP) {
		switch (button) {
		case 3:
			/* zoom in */
			render_distance -= zoom;
			break;
		case 4:
			/* zoom out */
			render_distance += zoom;
			break;
		}
	}
}

void _set_window_mids(void)
{
	window_midw = window_width / 2;
	window_midh = window_height / 2;
}

void handle_key_press(unsigned char key, int x, int y)
{
	switch(key) {
	case KEY_ESCAPE: case 'Q': case 'q':
		exit(0);
		break;
	/* keyboard movement keys should be customizable */
	case 'W': case 'w':
		/* move up */
		offset_y -= zoom;
		break;
	case 'S': case 's':
		/* move down */
		offset_y += zoom;
		break;
	case 'A': case 'a':
		/* move left */
		offset_x -= zoom;
		break;
	case 'D': case 'd':
		/* move right */
		offset_x += zoom;
		break;
	}
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
	glutPostRedisplay();
	glutTimerFunc(16, update, 0);
}

void draw_grid()
{
	double grid_width = 8.0;
	double grid_height = 6.0;

	/* draw vertical lines */
	for(double i = -(grid_width / 2.0) + offset_x; i <= (grid_width / 2.0) + offset_x; i += 1.0) {
		glBegin(GL_LINES);
		glVertex3d( i,  (grid_height / 2.0) + offset_y, -(render_distance));
		glVertex3d( i, -(grid_height / 2.0) + offset_y, -(render_distance));
		glEnd();
	}

	/* draw horizontal lines */
	for(double i = -(grid_height / 2.0) + offset_y; i <= (grid_height / 2.0) + offset_y; i += 1.0) {
		glBegin(GL_LINES);
		glVertex3d( (grid_width / 2.0) + offset_x,  i, -(render_distance));
		glVertex3d(-(grid_width / 2.0) + offset_x,  i, -(render_distance));
		glEnd();
	}
}

void draw_screen()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	draw_grid();
	
	glutSwapBuffers();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	
	if (fullscreen) {
		window_width = glutGet(GLUT_SCREEN_WIDTH) != 0 ? glutGet(GLUT_SCREEN_WIDTH) : window_width;
		window_height = glutGet(GLUT_SCREEN_HEIGHT) != 0 ? glutGet(GLUT_SCREEN_HEIGHT) : window_height;
	}
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(window_width, window_height);
	_set_window_mids();
	glutCreateWindow("7dRTS Game by four04 and Sammidysam");

	if (fullscreen)
		glutFullScreen();

	init_rendering();

	glutMouseFunc(handle_mouse);
	glutDisplayFunc(draw_screen);
	glutKeyboardFunc(handle_key_press);
	glutReshapeFunc(handle_resize);
	glutTimerFunc(16, update, 0);
	glutMainLoop();

	return 0;
}
