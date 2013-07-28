#include <src/osdetect.h>

#if OS == WINDOWS
#include <windows.h>
#else
#include <glob.h>
#endif
#include <string.h>
#include <stdlib.h>

#include <GL/glut.h>

#include <src/global.h>

#include <src/save_manager.h>

int save_manager_get_save_number()
{
	int number = 0;
	
	char pattern [1024];
	strcpy(pattern, "saves");
	strcat(pattern, PATH_SEP);
	strcat(pattern, "*.save");

#if OS == WINDOWS
	WIN32_FIND_DATA data;
	HANDLE h_find = FindFirstFile(pattern, &data);

	if (h_find == INVALID_HANDLE_VALUE)
		return 0;

	number = 1;
	while (FindNextFile(h_find, &data) != 0)
		number++;

	FindClose(h_find);
#else
	glob_t result;

	glob(pattern, GLOB_ERR, NULL, &result);

	number = 0;
	for (int i = 0; i < result.gl_pathc; i++)
		if (result.gl_pathv[i] != NULL)
			number++;

	globfree(&result);
#endif

	return number;
}

void save_manager_get_saves(char **list)
{
	char pattern [1024];
	strcpy(pattern, "saves");
	strcat(pattern, PATH_SEP);
	strcat(pattern, "*.save");
	
#if OS == WINDOWS
	WIN32_FIND_DATA save;
	HANDLE h_find = FindFirstFile(pattern, &save);
	
	strcpy(list[0], save.cFileName);
	for (int i = 0; i < save_manager_get_save_number(); i++) {
		FindNextFile(h_find, &save);
		strcpy(list[i], save.cFileName);
	}
	
	FindClose(h_find);
#else
	glob_t result;

	glob(pattern, GLOB_ERR, NULL, &result);

	for (int i = 0; i < save_manager_get_save_number(); i++)
		strcpy(list[i], result.gl_pathv[i]);

	globfree(&result);
#endif
}

void save_manager_draw_saves(char **list, char *select_message, char *no_saves, int selected)
{
	void *font = GLUT_BITMAP_9_BY_15;

	double x_multiplier = -20.0 / 1024.0;
	double y_multiplier = 15.0 / 768.0;

	glRasterPos3d((double)window_width * x_multiplier, (double)window_height * y_multiplier, -40.0);

	for (int i = 0; select_message[i] != '\0'; i++)
		glutBitmapCharacter(font, select_message[i]);

	if (save_manager_get_save_number() > 0) {
		for (int i = 0; i < save_manager_get_save_number(); i++) {
			x_multiplier = -18.0 / 1024.0;
			y_multiplier = (13.0 - (i * 2.0)) / 768.0;

			glRasterPos3d((double)window_width * x_multiplier, (double)window_height * y_multiplier, -40.0);

			for (int j = 0; list[i][j] != '\0'; j++)
				glutBitmapCharacter(font, list[i][j]);

			/* draw arrow if selected */
			if (selected == i) {
				x_multiplier = -20.0 / 1024.0;

				glLineWidth(1.0f);
				glBegin(GL_LINES);
				glVertex3d((double)window_width * x_multiplier, (double)window_height * y_multiplier + 0.2, -40.0);
				glVertex3d((double)window_width * x_multiplier + 0.75, (double)window_height * y_multiplier + 0.2, -40.0);
				glVertex3d((double)window_width * x_multiplier + 1.5, (double)window_height * y_multiplier + 0.2, -40.0);
				glVertex3d((double)window_width * x_multiplier + 0.75, (double)window_height * y_multiplier + 0.7, -40.0);
				glVertex3d((double)window_width * x_multiplier + 0.75, (double)window_height * y_multiplier + 0.7, -40.0);
				glVertex3d((double)window_width * x_multiplier + 0.75, (double)window_height * y_multiplier - 0.3, -40.0);
				glVertex3d((double)window_width * x_multiplier + 0.75, (double)window_height * y_multiplier - 0.3, -40.0);
				glVertex3d((double)window_width * x_multiplier + 1.5, (double)window_height * y_multiplier + 0.2, -40.0);
				glEnd();
			}
		}
	} else {
		x_multiplier = -18.0 / 1024.0;
		y_multiplier = 13.0 / 768.0;

		glRasterPos3d((double)window_width * x_multiplier, (double)window_height * y_multiplier, -40.0);

		for (int i = 0; no_saves[i] != '\0'; i++)
			glutBitmapCharacter(font, no_saves[i]);
	}
}

void save_manager_move_select(int *selection, bool up)
{
	if (up) {
		if (*selection + 1 < save_manager_get_save_number())
			++*selection;
	} else {
		if (*selection - 1 > -1)
			--*selection;
	}
}
