#ifndef MENU_H
#define MENU_H

#define BUTTON_GAP 1.0
#define BORDER_WIDTH 7.0f

typedef enum layout_type_t {
	LAYOUT_TYPE_HORIZONTAL,
	LAYOUT_TYPE_VERTICAL,
	LAYOUT_TYPE_GRID,

	LAYOUT_TYPE_ERROR,

	LAYOUT_TYPE_MAX
} layout_type_t;

typedef struct layout_t {
	layout_type_t type;
	
	int width;
	int height;
} layout_t;

typedef struct button_t {
	char *text;

	bool selected;
} button_t;

layout_t _menu_get_button_layout(size_t buttons_len);

void menu_set_button_dimensions(double *button_width, double *button_height, layout_t layout);
void menu_set_start_values(double *x_start, double *y_start, layout_t layout, int loop_point);

void menu_draw_buttons(button_t buttons[], size_t button_num);
void menu_draw_text(char *name, char *description);

void menu_select(direction_t direction, button_t *buttons[], size_t button_len);

#endif
