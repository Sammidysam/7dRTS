#include "menu.h"

void draw_menu()
{
	char *game_name = "7dRTS";

	void *font = GLUT_BITMAP_9_BY_15;

	glRasterPos3i(0, 0, -40);
	
	for (int i = 0; game_name[i] != '\0'; i++) {
		glutBitmapCharacter(font, game_name[i]);
	}
}
