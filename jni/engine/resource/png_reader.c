/*
 * png_reader.c
 *
 *  Created on: 2013-9-11
 *      Author: Ryan
 */

#include "image.h"


struct _png_read_record {
	unsigned int offset;
	void *buf;
};

struct _png_read_file_record {
	FILE *fp;
};

struct _png_mem_track {
	unsigned int sizeAllocated;
};

PRIVATE PNGAPI void _png_custom_reader(png_structp png_ptr, png_bytep to_store, png_size_t bytes_to_read) {
	/* copies the data from our supplied buffer to libpng's buffer, copy starts from the last known offset */
	struct _png_read_record *p = (struct _png_read_record *)png_get_io_ptr(png_ptr);
	memcpy((void *)to_store, (unsigned char *)(p->buf) + p->offset, bytes_to_read);
	p->offset = p->offset + bytes_to_read;
	return;
}

PRIVATE PNGAPI void _png_custom_reader_file(png_structp png_ptr, png_bytep to_store, png_size_t bytes_to_read) {
	/* reads the data from the file descriptor into the to_store */
	struct _png_read_file_record *p = (struct _png_read_file_record *)png_get_io_ptr(png_ptr);
	if (1 != fread((void *)to_store, bytes_to_read, 1, p->fp))
		LOGE("gfx_img_png_read_file(): fread() failed.");
	return;
}

PRIVATE PNGAPI void _png_custom_error(png_structp png_ptr, png_const_charp err_text) {
	LOGE("gfx_img_png_read(): libpng error (%s).", err_text);
	return;
}

PRIVATE PNGAPI void _png_custom_warning(png_structp png_ptr, png_const_charp err_text) {
	LOGW("gfx_img_png_read(): libpng warning (%s).", err_text);
	return;
}

PRIVATE png_voidp _png_custom_malloc(png_structp png_ptr, png_alloc_size_t sizeToAlloc) {
	png_voidp buf; struct _png_mem_track *m;
	buf = (png_voidp)MALLOC(sizeToAlloc + 4);	/* allocates additional 4 bytes for size tracking */
	*(unsigned int *)buf = sizeToAlloc;			/* stores the size in front of the "real" thing */
	m = (struct _png_mem_track *)png_get_mem_ptr(png_ptr);
	m->sizeAllocated += sizeToAlloc;
	return ((unsigned char *)buf + 4);
}

PRIVATE void _png_custom_free(png_structp png_ptr, png_voidp memToFree) {
	unsigned int sizeToFree; struct _png_mem_track *m;
	sizeToFree = *(unsigned int *)((unsigned char *)memToFree - 4);	/* rewinds 4 bytes to get the size */
	m = (struct _png_mem_track *)png_get_mem_ptr(png_ptr);
	if (sizeToFree > m->sizeAllocated)
		LOGW("gfx_img_png_read(): libpng tries to free more memory than what it has allocated.");
	m->sizeAllocated -= sizeToFree;
	/* rewinds 4 bytes to get to where the real allocated block is */
	FREE((void *)((unsigned char *)memToFree - 4));
	return;
}

PRIVATE PNGAPI int _png_custom_read_chunk(png_structp png_ptr, png_unknown_chunkp unknown_chunk) {
	/*
	 * returns the following:
	 * < 0 (error)
	 * 0 (does not recognize)
	 * > 0 (success)
	 * */
	LOGI("chunk name %s", unknown_chunk->name);
	return -1;
}

PUBLIC char *png_reader_parser(unsigned char *png_buf,
											unsigned int *png_width,
											unsigned int *png_height,
											unsigned char *png_channels,
											unsigned int *png_rowbytes) {
	int ret; png_structp src_png_ptr; png_infop src_info_ptr;
	char *rgba_data; png_bytepp row_pointers; png_uint_32 i;
	png_uint_32 src_width; png_uint_32 src_height;
	struct _png_read_record p; struct _png_mem_track m;
	int src_bit_depth; int src_color_type; png_byte src_channels; png_uint_32 src_rowbytes;

	/* sanity check */
	if (NULL == png_buf) {
		LOGW("png_reader_parser(): png_buf is NULL.");
		return NULL;
	}
	ret = png_sig_cmp((png_bytep)png_buf, 0, 8);
	if (0 != ret) {
		LOGE("png_reader_parser(): png_sig_cmp() failed.");
		return NULL;
	}
	m.sizeAllocated = 0;
	src_png_ptr = png_create_read_struct_2(PNG_LIBPNG_VER_STRING, NULL, _png_custom_error, _png_custom_warning,
											(void *)&m, _png_custom_malloc, _png_custom_free);
	if (NULL == src_png_ptr) {
		LOGE("png_reader_parser(): png_create_read_struct() failed.");
		return NULL;
	}
	src_info_ptr = png_create_info_struct(src_png_ptr);
	if (NULL == src_info_ptr) {
		LOGE("png_reader_parser(): png_create_info_struct() failed.");
		png_destroy_read_struct(&src_png_ptr, NULL, NULL);
		return NULL;
	}
	/* setup longjmp buf */
	if (0 != setjmp(png_jmpbuf(src_png_ptr))) {
		LOGE("png_reader_parser(): error, returned from longjmp.");
		png_destroy_read_struct(&src_png_ptr, &src_info_ptr, NULL);
		return NULL;
	}
	/* done setjmp */
	p.offset = 8; p.buf = png_buf;
	png_set_sig_bytes(src_png_ptr, 8);
	png_set_read_fn(src_png_ptr, (void *)&p, (png_rw_ptr)_png_custom_reader);
	png_set_read_user_chunk_fn(src_png_ptr, NULL, _png_custom_read_chunk);
	png_read_info(src_png_ptr, src_info_ptr);
	ret = png_get_IHDR(src_png_ptr, src_info_ptr,
						&src_width,
						&src_height,
						&src_bit_depth,
						&src_color_type,
						NULL,	/* filter_method */
						NULL, 	/* compression_method */
						NULL);	/* interlace_method */
	if (1 != ret) {
		LOGE("png_reader_parser(): png_get_IHDR() failed.");
		png_destroy_read_struct(&src_png_ptr, &src_info_ptr, NULL);
		return NULL;
	}
	if (!(src_color_type & PNG_COLOR_MASK_ALPHA)) {
	  if (src_color_type == PNG_COLOR_TYPE_PALETTE) {
		  png_set_expand(src_png_ptr);
	  }
	  png_set_filler(src_png_ptr, 65535L, PNG_FILLER_AFTER);
	}
	if (16 == src_bit_depth) {
		png_set_strip_16(src_png_ptr);
	}
	if ((PNG_COLOR_TYPE_GRAY == src_color_type) || (PNG_COLOR_TYPE_GRAY_ALPHA == src_color_type)) {
		png_set_gray_to_rgb(src_png_ptr);
	}
	png_read_update_info(src_png_ptr, src_info_ptr);
	src_channels = png_get_channels(src_png_ptr, src_info_ptr);
	if (0 == src_channels) {
		LOGE("png_reader_parser(): png_get_channels() failed.");
		png_destroy_read_struct(&src_png_ptr, &src_info_ptr, NULL);
		return NULL;
	}
	src_rowbytes = png_get_rowbytes(src_png_ptr, src_info_ptr);
	if (0 == src_rowbytes) {
		LOGE("png_reader_parser(): png_get_rowbytes() failed.");
		png_destroy_read_struct(&src_png_ptr, &src_info_ptr, NULL);
		return NULL;
	}
	rgba_data = (unsigned char *)MALLOC(src_rowbytes * src_height);
	if (NULL == rgba_data) {
		LOGE("png_reader_parser(): MALLOC(rgba_data) failed.");
		png_destroy_read_struct(&src_png_ptr, &src_info_ptr, NULL);
		return NULL;
	}
	row_pointers = (png_bytepp)MALLOC(src_height * sizeof(png_bytep));
	if (NULL == row_pointers) {
		LOGE("png_reader_parser(): MALLOC(row_pointers) failed.");
		FREE((void *)rgba_data);
		png_destroy_read_struct(&src_png_ptr, &src_info_ptr, NULL);
		return NULL;
	}
	/* sets the individual row_pointers to point at the correct offsets */
	for (i = 0; i < src_height; ++i) {
		row_pointers[i] = rgba_data + i * src_rowbytes;
	}
	/* now we can go ahead and just read the whole image */
	png_read_image(src_png_ptr, row_pointers);
	/* pixel 0: r=rgba_data[0], g=rgba_data[1], b=rgba_data[2], a=rgba_data[3] */
	/* pixel 1: r=rgba_data[4], g=rgba_data[5], b=rgba_data[6], a=rgba_data[7] */
	/* pixel 2: r=rgba_data[8], g=rgba_data[9], b=rgba_data[10], a=rgba_data[11] */
	/* ...etc... */
	png_read_end(src_png_ptr, src_info_ptr);
	/*
	 * test code for reading text
	 *	png_textp text_ptr; png_uint_32 u; png_uint_32 u1;
	 *	LOGI("text count: %u", (u = png_get_text(src_png_ptr, src_info_ptr, &text_ptr, NULL)));
	 *	for (u1 = 0; u1 < u; u1++) {
	 *		LOGI("%s:%s", text_ptr[u1].key, text_ptr[u1].text);
	 *	}
	 */
	png_destroy_read_struct(&src_png_ptr, &src_info_ptr, NULL);
	FREE((void *)row_pointers);	/* row_pointers is no longer needed */
	/* makes sure that libpng has free'ed all allocated memory */
	if (0 < m.sizeAllocated) {
		LOGW("png_reader_parser(): libpng memory allocated > 0 upon exit.");
	}
	/* all done, prepares the return values */
	if (NULL != png_width) {
		*png_width = src_width;
	}
	if (NULL != png_height) {
		*png_height = src_height;
	}
	if (NULL != png_channels) {
		*png_channels = src_channels;
	}
	if (NULL != png_rowbytes) {
		*png_rowbytes = src_rowbytes;
	}
	return rgba_data;
}
