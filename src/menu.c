#include "menu.h"

layout_t get_button_layout(size_t buttons_len)
{
	layout_t layout;
	
	switch(buttons_len) {
	case 2:
		layout.layout_grid.width = 1;
		layout.layout_grid.height = 1;
		layout.type = LAYOUT_TYPE_HORIZONTAL;
		return layout;
	case 3:
		layout.layout_grid.width = 1;
		layout.layout_grid.height = 1;
		layout.type = LAYOUT_TYPE_VERTICAL;
		return layout;
	case 4:
		layout.layout_grid.width = 2;
		layout.layout_grid.height = 2;
		layout.type = LAYOUT_TYPE_GRID;
		return layout;
	default:
		printf("Invalid button number %i!\n", buttons_len);
		layout.type = LAYOUT_TYPE_ERROR;
		return layout;
	}
}

void set_button_dimensions(double *button_width, double *button_height, layout_t layout)
{
	*button_width = 40.0 / layout.layout_grid.width;
	*button_height = 25.0 / layout.layout_grid.height;
}

void set_start_values(double *x_start, double *y_start, layout_t layout, int loop_point)
{
	double button_width;
	double button_height;
	set_button_dimensions(&button_width, &button_height, layout);
	
	*x_start = (double)window_width * ((-18.0 + ((double)(loop_point % (int)layout.layout_grid.width) * button_width)) / 1024.0);
	*y_start = (double)window_height * ((9.0 - ((double)(loop_point / layout.layout_grid.height) * button_height)) / 768.0);
}

void draw_menu_buttons(button_t buttons[], int button_num)
{
	void *font = GLUT_BITMAP_9_BY_15;

	layout_t layout = get_button_layout(button_num);

	if (layout.type == LAYOUT_TYPE_ERROR)
		return;

	double button_width;
	double button_height;
	set_button_dimensions(&button_width, &button_height, layout);

	double x_start;
	double y_start;

	for (int i = 0; i < button_num; i++) {
		set_start_values(&x_start, &y_start, layout, i);

		/* draw rectangle */
		glColor3d(1.0, 1.0, 1.0);
		glBegin(GL_QUADS);
		glVertex3d(x_start, y_start, -40.0);
		glVertex3d(x_start, y_start - button_height + BUTTON_GAP, -40.0);
		glVertex3d(x_start + button_width - BUTTON_GAP, y_start - button_height + BUTTON_GAP, -40.0);
		glVertex3d(x_start + button_width - BUTTON_GAP, y_start, -40.0);
		glEnd();

		/* draw red border around selected button */
		if (buttons[i].selected) {
			glColor3d(1.0, 0.0, 0.0);
			glLineWidth(BORDER_WIDTH);
			glBegin(GL_LINE_LOOP);
			glVertex3d(x_start, y_start, -40.0);
			glVertex3d(x_start, y_start - button_height + BUTTON_GAP, -40.0);
			glVertex3d(x_start + button_width - BUTTON_GAP, y_start - button_height + BUTTON_GAP, -40.0);
			glVertex3d(x_start + button_width - BUTTON_GAP, y_start, -40.0);
			glEnd();
		}

		/* draw text */
		glColor3d(0.0, 0.0, 0.0);
		glRasterPos3d(x_start + (button_width / 2.0) - 2.0, y_start - (button_height / 2.0), -39.0);

		for (int j = 0; buttons[i].text[j] != '\0'; j++)
			glutBitmapCharacter(font, buttons[i].text[j]);
	}
}

void draw_menu_text(char *name, char *description)
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
