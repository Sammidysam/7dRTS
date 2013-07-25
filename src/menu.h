#ifndef MENU_H
#define MENU_H

#include <stdio.h>
#include <stdbool.h>

#include <GL/glut.h>

#include "main.h"

#define BUTTON_GAP 1.0

typedef enum layout_type_t {
	LAYOUT_TYPE_HORIZONTAL,
	LAYOUT_TYPE_VERTICAL,
	LAYOUT_TYPE_GRID,

	LAYOUT_TYPE_ERROR,

	LAYOUT_TYPE_MAX
} layout_type_t;

typedef struct layout_t {
	layout_type_t type;
	
	int rows;
	int columns;
} layout_t;

typedef struct button_t {
	char *text;

	bool selected;
} button_t;

layout_t get_button_layout(int button_num);
void set_button_dimensions(double *button_width, double *button_height, layout_t layout);
void set_start_values(double *x_start, double *y_start, layout_t layout, int loop_point);
void draw_menu_buttons(button_t buttons[], int button_num);
void draw_menu_text(char *name, char *description);

#endif
