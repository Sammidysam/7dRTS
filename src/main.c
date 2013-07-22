#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <texture.h>

#include <png.h>
#include <GL/glut.h>

#define ESCAPE 27

double rAngle = 0.0;

typedef enum tile_type_t {
	TILE_TYPE_GRASS,
	TILE_TYPE_FOREST,
	TILE_TYPE_LAKE,
	
	TILE_TYPE_MAX
} tile_type_t;

typedef struct tile_t {
	double min;
	double max;

	tile_type_t tile_type;

	texture_t *texture;
} tile_t;

texture_t *grass_texture;
texture_t *forest_texture;
texture_t *lake_texture;
texture_t *error_texture;

void handle_key_press(unsigned char key, int x, int y)
{
	switch(key) {
	case ESCAPE:
		exit(0);
	}
}

void load_textures()
{
	grass_texture = texture_new();
	forest_texture = texture_new();
	lake_texture = texture_new();
	error_texture = texture_new();

	texture_load(grass_texture, "img/grass.png");
	texture_load(forest_texture, "img/forest.png");
	texture_load(lake_texture, "img/lake.png");
	texture_load(error_texture, "img/error.png");
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
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (double)w / (double)h, 1.0, 200.0);
}

void update(int value)
{
	rAngle += 1.0;
	if(rAngle > 360.0) {
		rAngle -= 360.0;
	}

	glutPostRedisplay();
	glutTimerFunc(20, update, 0);
}

void draw_screen()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotated(-rAngle, 0.0, 0.0, -1.0);

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
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(512, 512);
	glutCreateWindow("Simple Animation Test");

	init_rendering();
	
	glutDisplayFunc(draw_screen);
	glutKeyboardFunc(handle_key_press);
	glutReshapeFunc(handle_resize);
	glutTimerFunc(20, update, 0);
	glutMainLoop();

	return 0;
}
