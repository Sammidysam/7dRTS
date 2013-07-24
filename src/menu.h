#ifndef MENU_H
#define MENU_H

#include <GL/glut.h>

typedef struct button_t {
	char *text;
} button_t;

void draw_menu_buttons(button_t buttons[], int button_num, int window_width, int window_height);
void draw_menu_text(char *name, char *description, int window_width, int window_height);

#endif
