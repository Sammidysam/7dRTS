#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>

#include <libconfig.h>

/*
 * a function that gets a pointer of undefined type
 * and sets it would be good here
 */

void config_get_item_int(int *value, const char *path);
void config_get_item_string(char **value, const char *path);
void config_get_item_bool(bool *value, const char *path);
void config_get_item_double(double *value, const char *path);

void config_get_item_int_list(int **list, int *length, const char *path);

void config_print_debug_int(char *var_name, int value);
void config_print_debug_string(char *var_name, char *value);
void config_print_debug_bool(char *var_name, bool value);
void config_print_debug_double(char *var_name, double value);

#endif
