#ifndef MAIN_H
#define MAIN_H

#define KEY_ENTER 13
#define KEY_CTRL_R 18
#define KEY_CTRL_S 19
#define KEY_CTRL_W 23
#define KEY_ESCAPE 27

void zoom_in();
void zoom_out();
void handle_mouse(int button, int state, int x, int y);
void _set_window_mids();
void handle_key_up(unsigned char key, int x, int y);
void handle_key_down(unsigned char key, int x, int y);
void load_textures();
void init_rendering();
void init_game();
void init_buttons();
void handle_resize(int w, int h);
void update(int value);
void draw_screen();
void clean_up();
int main(int argc, char *argv[]);

#endif
