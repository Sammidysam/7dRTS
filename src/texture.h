#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/gl.h>

struct gl_texture_t {
	GLsizei width;
	GLsizei height;
	
	GLenum format;
	GLint internalFormat;
	GLuint id;

	GLubyte *texels;
};

static void _texture_get_png_texture_info(int color_type, struct gl_texture_t *texinfo);
static struct gl_texture_t *_texture_read_png_from_file(const char *filename);
GLuint _texture_load_png_texture(const char *filename);

void load_textures();

GLuint grass_texture;
GLuint forest_texture;
GLuint water_texture;
GLuint stone_texture;
GLuint farm_texture;
GLuint castle_wall_texture;
GLuint castle_tower_texture;
GLuint error_texture;
GLuint uninitialized_texture;

GLuint arrow_texture;
GLuint archer_still_texture;
GLuint archer_left_texture;
GLuint archer_right_texture;

GLuint horseman_still_texture;

GLuint knight_still_texture;

GLuint swordsman_still_texture;
GLuint swordsman_left_texture;
GLuint swordsman_right_texture;

GLuint trebuchet_still_texture;

#endif
