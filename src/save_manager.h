#ifndef SAVE_MANAGER_H
#define SAVE_MANAGER_H

#include <stdbool.h>

int save_manager_get_save_number();

void save_manager_get_saves(char **list);

void save_manager_draw_saves(char **list, char *select_message, char *no_saves, int selected);

void save_manager_move_select(int *selection, bool up);

#endif
