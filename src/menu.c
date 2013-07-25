#include <stdio.h>
#include <stdbool.h>

#include <GL/glut.h>

#include <src/point.h>
#include <src/global.h>
#include <src/direction.h>

#include <src/menu.h>

layout_t _menu_get_button_layout(size_t buttons_len)
{
	layout_t layout;
	
	switch(buttons_len) {
	case 2:
		layout.width = 1;
		layout.height = 1;
		layout.type = LAYOUT_TYPE_HORIZONTAL;
		return layout;
	case 3:
		layout.width = 1;
		layout.height = 1;
		layout.type = LAYOUT_TYPE_VERTICAL;
		return layout;
	case 4:
		layout.width = 2;
		layout.height = 2;
		layout.type = LAYOUT_TYPE_GRID;
		return layout;
	default:
		printf("Invalid button number %i!\n", buttons_len);
		layout.type = LAYOUT_TYPE_ERROR;
		return layout;
	}
}

void menu_set_button_dimensions(double *button_width, double *button_height, layout_t layout)
{
	*button_width = 40.0 / layout.width;
	*button_height = 25.0 / layout.height;
}

void menu_set_start_values(double *x_start, double *y_start, layout_t layout, int loop_point)
{
	double button_width;
	double button_height;
	menu_set_button_dimensions(&button_width, &button_height, layout);
	
	*x_start = (double)window_width * ((-18.0 + ((double)(loop_point % (int)layout.width) * button_width)) / 1024.0);
	*y_start = (double)window_height * ((9.0 - ((double)(loop_point / layout.height) * button_height)) / 768.0);
}

void menu_draw_buttons(button_t buttons[], size_t button_num)
{
	void *font = GLUT_BITMAP_9_BY_15;

	layout_t layout = _menu_get_button_layout(button_num);

	if (layout.type == LAYOUT_TYPE_ERROR)
		return;

	double button_width;
	double button_height;
	menu_set_button_dimensions(&button_width, &button_height, layout);

	double x_start;
	double y_start;

	for (int i = 0; i < button_num; i++) {
		menu_set_start_values(&x_start, &y_start, layout, i);

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

void menu_draw_text(char *name, char *description)
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

void menu_select(direction_t direction, button_t *buttons, size_t button_len)
{
	layout_t layout = _menu_get_button_layout(button_len);

	if (layout.type == LAYOUT_TYPE_ERROR)
		return;

	int selected_button;
	for (int i = 0; i < button_len; i++)
		if (buttons[i].selected)
			selected_button = i;

	int selected_button_x = point_one_d_x(selected_button, layout.width, layout.height);
	int selected_button_y = point_one_d_y(selected_button, layout.width, layout.height);

	switch (direction) {
	case DIRECTION_UP:
		if (selected_button_y > 0) {
			buttons[selected_button].selected = false;
			buttons[point_two_d_to_one_d(point_new(selected_button_x, selected_button_y - 1), layout.width, layout.height)].selected = true;
		}
		break;
	case DIRECTION_DOWN:
		if (selected_button_y < layout.height - 1) {
			buttons[selected_button].selected = false;
			buttons[point_two_d_to_one_d(point_new(selected_button_x, selected_button_y + 1), layout.width, layout.height)].selected = true;
		}
		break;
	case DIRECTION_LEFT:
		if (selected_button_x > 0) {
			buttons[selected_button].selected = false;
			buttons[point_two_d_to_one_d(point_new(selected_button_x - 1, selected_button_y), layout.width, layout.height)].selected = true;
		}
		break;
	case DIRECTION_RIGHT:
		if (selected_button_x < layout.width - 1) {
			buttons[selected_button].selected = false;
			buttons[point_two_d_to_one_d(point_new(selected_button_x + 1, selected_button_y), layout.width, layout.height)].selected = true;
		}
		break;
	}
}
