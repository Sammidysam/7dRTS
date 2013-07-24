#include "menu.h"

void draw_menu_buttons(button_t buttons[], int button_num, int window_width, int window_height)
{
	void *font = GLUT_BITMAP_9_BY_15;

	double button_width = 14.0;
	double button_height = 8.5;

	for (int i = 0; i < button_num; i++) {
		double x_start = (double)window_width * (-18.0 / 1024.0);
		double y_start = (double)window_height * ((9.0 - ((double)i * (button_height))) / 768.0);

		double gap = 1.0;

		glColor3d(1.0, 1.0, 1.0);
		glBegin(GL_QUADS);
		glVertex3d(x_start, y_start - gap, -40.0);
		glVertex3d(x_start, y_start - button_height + gap, -40.0);
		glVertex3d(x_start + button_width, y_start - button_height + gap, -40.0);
		glVertex3d(x_start + button_width, y_start - gap, -40.0);
		glEnd();

		glColor3d(0.0, 0.0, 0.0);
		glRasterPos3d(x_start + (button_width / 2.0) - 1.5, y_start - (button_height / 2.0), -39.0);

		for (int j = 0; buttons[i].text[j] != '\0'; j++)
			glutBitmapCharacter(font, buttons[i].text[j]);
	}
}

void draw_menu_text(char *name, char *description, int window_width, int window_height)
{
	void *font = GLUT_BITMAP_9_BY_15;

	double x_multiplier = -20.0 / 1024.0;
	double y_multiplier = 13.0 / 768.0;
	
	glRasterPos3d((double)window_width * x_multiplier, (double)window_height * y_multiplier, -40.0);
	
	for (int i = 0; name[i] != '\0'; i++)
		glutBitmapCharacter(font, name[i]);

	x_multiplier = -18.0 / 1024.0;
	y_multiplier = 11.0 / 768.0;

	glRasterPos3d((double)window_width * x_multiplier, (double)window_height * y_multiplier, -40.0);

	for (int i = 0; description[i] != '\0'; i++)
		glutBitmapCharacter(font, description[i]);
}
