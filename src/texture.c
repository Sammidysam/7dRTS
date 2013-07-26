#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <src/texture.h>

#include <GL/glut.h>
#include <png.h>

static void _texture_get_png_texture_info(int color_type, struct gl_texture_t *texinfo) {
	switch(color_type) {
	case PNG_COLOR_TYPE_GRAY:
		texinfo->format = GL_LUMINANCE;
		texinfo->internalFormat = 1;
		break;
	case PNG_COLOR_TYPE_GRAY_ALPHA:
		texinfo->format = GL_LUMINANCE_ALPHA;
		texinfo->internalFormat = 2;
		break;
	case PNG_COLOR_TYPE_RGB:
		texinfo->format = GL_RGB;
		texinfo->internalFormat = 3;
		break;
	case PNG_COLOR_TYPE_RGB_ALPHA:
		texinfo->format = GL_RGBA;
		texinfo->internalFormat = 4;
		break;
	default:
		/* This is madness! */
		break;
	}
}

static struct gl_texture_t *_texture_read_png_from_file(const char *filename) {
	struct gl_texture_t *texinfo;
	png_byte magic[8];
	png_structp png_ptr;
	png_infop info_ptr;
	int bit_depth, color_type;
	FILE *fp = NULL;
	png_bytep *row_pointers = NULL;
	png_uint_32 w, h;
	int i;

	// Open image file
	fp = fopen(filename, "rb");
	if(!fp) {
		fprintf(stdout, "_texture_read_png_from_file: fopen failed on %s\n", filename);
		return NULL;
	}

	/* Read magic number */
	fread(magic, 1, sizeof(magic), fp);

	/* Check for valid magic number */
	if(!png_check_sig(magic, sizeof(magic))) {
		fprintf(stderr, "_texture_read_png_from_file: png_check_sig(%d, %d) did not work on %s, maybe not valid png file?\n", magic, sizeof(magic), filename);
		fclose(fp);
		return NULL;
	}

	/* Create a png read struct */
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(!png_ptr) {
		fclose(fp);
		return NULL;
	}

	/* Create a png info struct */
	info_ptr = png_create_info_struct(png_ptr);
	if(!info_ptr) {
		fclose(fp);
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		return NULL;
	}

	/* Create our OpenGL texture object */
	texinfo =(struct gl_texture_t *)malloc(sizeof(struct gl_texture_t));

	/* Initialize the setjmp for returning properly after a libpng
	   error occured */
	if(setjmp(png_jmpbuf(png_ptr))) {
		fclose(fp);
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

		if(row_pointers)
			free(row_pointers);

		if(texinfo) {
			if(texinfo->texels)
				free(texinfo->texels);
			free(texinfo);
		}
		return NULL;
	}

	/* Setup libpng for using standard C fread() function
	   with our FILE pointer */
	png_init_io(png_ptr, fp);
  
	/* Tell libpng that we have already read the magic number */
	png_set_sig_bytes(png_ptr, sizeof(magic));
  
	/* Read png info */
	png_read_info(png_ptr, info_ptr);
  
	/* Get some usefull information from header */
	bit_depth = png_get_bit_depth(png_ptr, info_ptr);
	color_type = png_get_color_type(png_ptr, info_ptr);
  
	/* Convert index color images to RGB images */
	if(color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_palette_to_rgb(png_ptr);
  
	/* Convert 1-2-4 bits grayscale images to 8 bits
	   grayscale. */
	if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
    
#if 1
		png_set_expand_gray_1_2_4_to_8(png_ptr);
#else
	png_set_gray_1_2_4_to_8(png_ptr);
#endif

	if(png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
		png_set_tRNS_to_alpha(png_ptr);
  
	if(bit_depth == 16)
		png_set_strip_16(png_ptr);
	else if(bit_depth < 8)
		png_set_packing(png_ptr);

	/* Update info structure to apply transformations */
	png_read_update_info(png_ptr, info_ptr);
  
	/* Retrieve updated information */
	png_get_IHDR(png_ptr, info_ptr, &w, &h, &bit_depth, &color_type, NULL, NULL, NULL);
	texinfo->width = w;
	texinfo->height = h;
  
	/* Get image format and components per pixel */
	_texture_get_png_texture_info(color_type, texinfo);
  
	/* We can now allocate memory for storing pixel data */
	texinfo->texels =(GLubyte *)malloc(sizeof(GLubyte) * texinfo->width * texinfo->height * texinfo->internalFormat);

	/* Setup a pointer array.	Each one points at the begening of a row. */
	row_pointers =(png_bytep *)malloc(sizeof(png_bytep) * texinfo->height);

	for(i = 0; i < texinfo->height; ++i) {
#if 1
		row_pointers[i] =(png_bytep)(texinfo->texels +((texinfo->height -(i + 1)) * texinfo->width * texinfo->internalFormat));
#else
		row_pointers[i] =(png_bytep)(texinfo->texels +(texinfo->width * i * texinfo->internalFormat));
#endif
	}

	/* Read pixel data using row pointers */
	png_read_image(png_ptr, row_pointers);

	/* Finish decompression and release memory */
	png_read_end(png_ptr, NULL);
	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

	/* We don't need row pointers anymore */
	free(row_pointers);

	fclose(fp);
	fprintf(stdout, "_texture_read_png_from_file: %s [ \033[32mOK\033[0m ]\n", filename);
	return texinfo;
}

GLuint _texture_load_png_texture(const char *filename) {
	struct gl_texture_t *png_tex = NULL;
	GLuint tex_id = 0;
	GLint alignment;

	png_tex = _texture_read_png_from_file(filename);
  
	if(png_tex && png_tex->texels) {
		/* Generate texture */
		glGenTextures(1, &png_tex->id);
		glBindTexture(GL_TEXTURE_2D, png_tex->id);
    
		/* Setup some parameters for texture filters and mipmapping */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
		glGetIntegerv(GL_UNPACK_ALIGNMENT, &alignment);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

#if 0
		glTexImage2D(GL_TEXTURE_2D,
		             0,
		             png_tex->internalFormat,
		             png_tex->width,
		             png_tex->height,
		             0,
		             png_tex->format,
		             GL_UNSIGNED_BYTE,
		             png_tex->texels);
#else
		gluBuild2DMipmaps(GL_TEXTURE_2D,
		                  png_tex->internalFormat,
		                  png_tex->width,
		                  png_tex->height,
		                  png_tex->format,
		                  GL_UNSIGNED_BYTE,
		                  png_tex->texels);
#endif
    
		glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);

		tex_id = png_tex->id;

		/* OpenGL has its own copy of texture data */
		free(png_tex->texels);
		free(png_tex);
	}
	return tex_id;
}

void load_textures()
{
	grass_texture = _texture_load_png_texture("img/grass.png");
	forest_texture = _texture_load_png_texture("img/tree/oak.png");
	water_texture = _texture_load_png_texture("img/water.png");
	stone_texture = _texture_load_png_texture("img/stone.png");
	farm_texture = _texture_load_png_texture("img/farm.png");
	castle_wall_texture = _texture_load_png_texture("img/castle/wall/straight.png");
	castle_tower_texture = _texture_load_png_texture("img/castle/tower.png");
	error_texture = _texture_load_png_texture("img/error.png");
	uninitialized_texture = _texture_load_png_texture("img/uninitialized.png");

	arrow_texture = _texture_load_png_texture("img/units/archer/arrow.png");
	archer_still_texture = _texture_load_png_texture("img/units/archer/still.png");
	archer_left_texture = _texture_load_png_texture("img/units/archer/left.png");
	archer_right_texture = _texture_load_png_texture("img/units/archer/right.png");

	horseman_still_texture = _texture_load_png_texture("img/units/horseman/still.png");

	knight_still_texture = _texture_load_png_texture("img/units/knight/still.png");

	swordsman_still_texture = _texture_load_png_texture("img/units/swordsman/still.png");
	swordsman_left_texture = _texture_load_png_texture("img/units/swordsman/left.png");
	swordsman_right_texture = _texture_load_png_texture("img/units/swordsman/right.png");

	trebuchet_still_texture = _texture_load_png_texture("img/units/trebuchet/still.png");
}
