/*
 * image.h
 *
 *  Created on: 2013-9-8
 *      Author: Administrator
 */

#ifndef IMAGE_H_
#define IMAGE_H_


#include <stdio.h>
#include <base.h>
#include <jni_lib.h>
#include "../ext/libpng/png.h"

/* 纹理质量 */
typedef enum {
	IMG_QUALITY_NEAREST	= 0,
	IMG_QUALITY_LOW		= 0,
	IMG_QUALITY_FASTER	= 0,
	IMG_QUALITY_LINEAR	= 1,
	IMG_QUALITY_HIGH	= 1
} IMG_QUALITY;

typedef enum {
	IMG_TYPE_RES	= 0,
	IMG_TYPE_TEXT	= 1
} IMG_TYPE;

typedef void (*CALLBACK_FUN_DELETE_BIND)(void *graphic, void *tex);

typedef struct Image {
	unsigned int texId;
	char filePath[128];
	IMG_TYPE type;
	CALLBACK_FUN_DELETE_BIND callback_deleteBind;
	void *callback_param_graphic;
	unsigned int size;
	unsigned char *pixels;
	unsigned char bytesPerPixel;
	IMG_QUALITY quality;
	unsigned int width;
	unsigned int height;
	unsigned int widthPOT;
	unsigned int heightPOT;
	unsigned short usedCount;	/* 被占用的个数 */
	BOOL isBinded;
} Texture;

unsigned char *image_readPng(const char* filePath);
unsigned char *ReadPNG(const char *file);
char *png_reader_parser(unsigned char *png_buf,
						unsigned int *png_width,
						unsigned int *png_height,
						unsigned char *png_channels,
						unsigned int *png_rowbytes);
Texture *image_createEmptyPng(unsigned int width, unsigned int height);
Texture *image_createPngPOT(const char *filePath, IMG_QUALITY quality);
Texture *image_crateTextPngPOT(const char *text, int fontsize, int strokeWidth, int paintStyle,
												int r, int g, int b, int a, int strokeR, int strokeG, int strokeB, int strokeA, int *textPixelSize);
void image_destroy(Texture *img);
void image_setCallBackFun(Texture *tex, CALLBACK_FUN_DELETE_BIND callbackFun, void *graphic);
unsigned char *image_iconMergeTint(unsigned char *image, unsigned char *mask,
									unsigned char *backgroundImage,
									/*unsigned char *resultBitmap,*/
									unsigned char *rgba);

#endif /* IMAGE_H_ */
