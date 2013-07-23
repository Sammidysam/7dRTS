#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <texture.h>

#include <png.h>
#include <GL/glut.h>

#define KEY_ESCAPE 27

// normally good is 0 for development, 1 for release
int fullscreen = 0;

int window_width = 512;
int window_height = 512;

int window_midw;
int window_midh;

void handle_mouse_motion(int x, int y)
{
	glutWarpPointer(window_midw, window_midh);
}

void _set_window_mids(void)
{
	window_midw = window_width / 2;
	window_midh = window_height / 2;
}

void handle_key_press(unsigned char key, int x, int y)
{
	switch(key) {
	case 'q':
	case 'Q':
	case KEY_ESCAPE:
		exit(0);
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

	glutSetCursor(GLUT_CURSOR_NONE);
	
	glutWarpPointer(window_midw, window_midh);
}

void handle_resize(int w, int h)
{
	window_width = w;
	window_height = h;
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

void draw_screen()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glBegin(GL_TRIANGLES); //Begin triangle coordinates
	glVertex3d(-0.5, 0.5, -5.0);
	glVertex3d(-1.0, 1.5, -5.0);
	glVertex3d(-1.5, 0.5, -5.0);
	glEnd(); //End triangle coordinates
	
	glutSwapBuffers(); //Send the 3D scene to the screen
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

	glutPassiveMotionFunc(handle_mouse_motion);
	glutDisplayFunc(draw_screen);
	glutKeyboardFunc(handle_key_press);
	glutReshapeFunc(handle_resize);
	glutTimerFunc(16, update, 0);
	glutMainLoop();

	return 0;
}
