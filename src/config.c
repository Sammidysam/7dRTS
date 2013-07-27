#include <stdlib.h>
#include <string.h>

#include <libconfig.h>

#include <src/global.h>

#include <src/config.h>

void config_get_item_int(int *value, const char *path)
{
	config_setting_t *setting = 0;

	setting = config_lookup(&config, path);
	
	if (config_setting_type(setting) == CONFIG_TYPE_INT)
		value[0] = config_setting_get_int(setting);

	free(setting);
}

void config_get_item_string(char **value, const char *path)
{
	config_setting_t *setting = 0;

	setting = config_lookup(&config, path);

	if (config_setting_type(setting) == CONFIG_TYPE_STRING) {
		value[0] = (char *)malloc(strlen(config_setting_get_string(setting)) * sizeof(char));
		strcpy(value[0], config_setting_get_string(setting));
	}

	free(setting);
}

void config_get_item_bool(bool *value, const char *path)
{
	config_setting_t *setting = 0;

	setting = config_lookup(&config, path);
	
	if (config_setting_type(setting) == CONFIG_TYPE_BOOL)
		value[0] = config_setting_get_bool(setting);

	free(setting);
}

void config_get_item_double(double *value, const char *path)
{
	config_setting_t *setting = 0;

	setting = config_lookup(&config, path);
	
	if (config_setting_type(setting) == CONFIG_TYPE_FLOAT)
		value[0] = config_setting_get_float(setting);

	free(setting);
}

void config_get_item_int_list(int **list, int *length, const char *path)
{
	config_setting_t *setting = 0;

	setting = config_lookup(&config, path);

	if (config_setting_type(setting) == CONFIG_TYPE_LIST) {
		free(*list);

		*length = config_setting_length(setting);

		*list = (int *)malloc(*length * sizeof(int));
		for (int i = 0; i < *length; i++) {
			char lookup_name [1024];

			sprintf(lookup_name, "%s.[%i]", path, i);

			printf("%s\n", lookup_name);

			setting = config_lookup(&config, lookup_name);

			if (config_setting_type(setting) == CONFIG_TYPE_INT)
				*list[i] = config_setting_get_int(setting);
			else if (config_setting_type(setting) == CONFIG_TYPE_STRING)
				*list[i] = config_setting_get_string(setting)[0];
		}

		free(setting);
	}
}

void config_print_debug_int(char *var_name, int value)
{
	printf("value parsed: %s = %d\n", var_name, value);
}

void config_print_debug_string(char *var_name, char *value)
{
	printf("value parsed: %s = \"%s\"\n", var_name, value);
}

void config_print_debug_bool(char *var_name, bool value)
{
	printf("value parsed: %s = %s\n", var_name, value ? "true" : "false");
}

void config_print_debug_double(char *var_name, double value)
{
	printf("value parsed: %s = %e\n", var_name, value);
}
