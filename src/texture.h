#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdbool.h>

#include <GL/gl.h>

struct gl_texture_t {
	GLsizei width;
	GLsizei height;
	
	GLenum format;
	GLint internalFormat;
	GLuint id;

	GLubyte *texels;
};

typedef struct texture_t {
	GLuint texture_id;
} texture_t;

static void _texture_get_png_texture_info(int color_type, struct gl_texture_t *texinfo);
static struct gl_texture_t *_texture_read_png_from_file(const char *filename);
GLuint _texture_load_png_texture(const char *filename);

texture_t *texture_new();
texture_t *texture_new_from_filename(char *filename);

void texture_load(texture_t *texture, char *filename);

#endif
