/*
 * image.c
 *
 *  Created on: 2013-9-8
 *      Author: ryan
 */

#include "image.h"


/* the size of the textures must be 2^n */
#define POT_SIZE		14
PRIVATE unsigned short POT[POT_SIZE] = {0, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192};

PRIVATE BOOL _image_checkPOT(unsigned int w, unsigned int h) {
	BOOL wPOT = FALSE;
	BOOL hPOT = FALSE;
	int i = 0;
	for (i = 0; i < POT_SIZE; i++) {
		if (FALSE == wPOT) {
			wPOT = (w == POT[i]);
		}
		if (FALSE == hPOT) {
			hPOT = (h == POT[i]);
		}
	}
	return (TRUE == wPOT && TRUE == hPOT);
}
/**
 * Callback for libpng read data
 */
PRIVATE void _image_readPngData(png_structp pngPtr, png_bytep data, png_size_t length) {
    AAsset *asset = (AAsset *)png_get_io_ptr(pngPtr);
    AAsset_read(asset, data, length);
    return;
}

/**
 * Read png pixel data from file, caller must be free it
 */
PUBLIC unsigned char *image_readPng(const char* filePath) {
    void* pixelData = NULL;
    float w, h;
    AAsset* asset   = NULL;
	off_t size;
	asset = jni_lib_getAAsset(filePath, &size);
    do {
        unsigned char head[8];
        AAsset_read(asset, head, 8);
        if(png_sig_cmp(head, 0, 8)) {
            LOGE("image_readPng() File %s, is not PNG", filePath);
            break;
        }
       /* Create and initialize the png_struct with the desired error handler
        * functions.  If you want to use the default stderr and longjump method,
        * you can supply NULL for the last three parameters.  We also supply the
        * the compiler header file version, so that we know if the application
        * was compiled with a compatible version of the library.  REQUIRED
        */
        png_structp pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!pngPtr) {
            LOGE("image_readPng() Unable to create PNG structure: %s", filePath);
            break;
        }
        // Allocate/initialize the memory for image information.  REQUIRED
        png_infop infoPtr = png_create_info_struct(pngPtr);
        if (!infoPtr) {
            png_destroy_read_struct(&pngPtr, NULL, NULL);
            LOGE("image_readPng() Unable to create PNG info : %s", filePath);
            break;
        }
        png_infop endInfo = png_create_info_struct(pngPtr);
        if (!endInfo) {
            png_destroy_read_struct(&pngPtr, &infoPtr, NULL);
            LOGE("image_readPng() Unable to create PNG end info : %s", filePath);
            break;
        }
        // Set error handling if you are using the setjmp/longjmp method (this is
        // the normal method of doing things with libpng).  REQUIRED unless you
        // set up your own error handlers in the png_create_read_struct() earlier.
        if (setjmp(png_jmpbuf(pngPtr))) {
          // Free all of the memory associated with the png_ptr and info_ptr
          png_destroy_read_struct(&pngPtr, &infoPtr, &endInfo);
          LOGE("image_readPng() Error during setjmp : %s", filePath);
          break;
        }
        /* If you are using replacement read functions, instead of calling
         * png_init_io() here you would call:
         * where user_io_ptr is a structure you want available to the callbacks
         */
        png_set_read_fn(pngPtr, (void *)asset, _image_readPngData);
        // If we have already read some of the signature
        png_set_sig_bytes(pngPtr, 8);
       /* The call to png_read_info() gives us all of the information from the
        * PNG file before the first IDAT (image data chunk).  REQUIRED
        */
        png_read_info(pngPtr, infoPtr);
        int bitDepth;
        int colorType;
        int interlaceype;
        png_uint_32 width;
        png_uint_32 height;

        /*32bit*/
        png_get_IHDR(pngPtr, infoPtr,
                &width,
                &height,
                &bitDepth,&colorType, &interlaceype, NULL, NULL);
        /* Note that png_get_IHDR() returns 32-bit data into
         * the application's width and height variables.
         * This is an unsafe situation if these are 16-bit
         * variables
         */
//        width     = png_get_image_width(pngPtr, infoPtr);
//        height    = png_get_image_height(pngPtr, infoPtr);
//        bitDepth  = png_get_bit_depth(pngPtr, infoPtr);
//        colorType = png_get_color_type(pngPtr, infoPtr);

       	w = (float) width;
        h = (float) height;
        LOGD("image_readPng() PNG width = %f, height = %f", w, h);

        // force palette images to be expanded to 24-bit RGB
        // it may include alpha channel

    	if (!(colorType & PNG_COLOR_MASK_ALPHA)) {
    	  if (colorType == PNG_COLOR_TYPE_PALETTE) {
    		  png_set_expand(pngPtr);
    	  }
    	  png_set_filler(pngPtr, 65535L, PNG_FILLER_AFTER);
    	}
        if (colorType == PNG_COLOR_TYPE_PALETTE) {
            png_set_palette_to_rgb(pngPtr);
            LOGD("png_set_palette_to_rgb");
        }
        // low-bit-depth grayscale images are to be expanded to 8 bits
        if (colorType == PNG_COLOR_TYPE_GRAY && bitDepth < 8) {
            png_set_expand_gray_1_2_4_to_8(pngPtr);
            LOGD("png_set_expand_gray_1_2_4_to_8");
        }
        // expand any tRNS chunk data into a full alpha channel
        if (png_get_valid(pngPtr, infoPtr, PNG_INFO_tRNS)) {
            png_set_tRNS_to_alpha(pngPtr);
            LOGD("png_set_tRNS_to_alpha");
        }
        // reduce images with 16-bit samples to 8 bits
        if (bitDepth == 16) {
            png_set_strip_16(pngPtr);
        }
        // expand grayscale images to RGB
        if (colorType == PNG_COLOR_TYPE_GRAY || colorType == PNG_COLOR_TYPE_GRAY_ALPHA) {
            png_set_gray_to_rgb(pngPtr);
            LOGD("png_set_gray_to_rgb");
        }
        /* png_read_update_info */

        // Update the png info struct.
        png_read_update_info(pngPtr, infoPtr);

        // Allocate the memory to hold the image using the fields of info_ptr
        unsigned int rowBytes = png_get_rowbytes(pngPtr, infoPtr);
        LOGD("image_readPng() Row size: %d bytes", rowBytes);
        // Allocate the pixel data as a big block, to be given to openGL
        pixelData = png_malloc(pngPtr, rowBytes * height);
        if(!pixelData) {
            png_destroy_read_struct(&pngPtr, &infoPtr, &endInfo);
            LOGE("image_readPng() Unable to allocate PNG pixel data while loading %s", filePath);
            break;
        }
        /* Turn on interlace handling.  REQUIRED if you are not using
         * png_read_image().  To see how to handle interlacing passes,
         * see the png_read_row() method below:
         */
        // read entire image , ignore alpha channel PNG_TRANSFORM_STRIP_ALPHAȥ
//        png_read_png(pngPtr, infoPtr, PNG_TRANSFORM_EXPAND | PNG_TRANSFORM_STRIP_ALPHA, NULL);
        int numberPasses = png_set_interlace_handling(pngPtr);
        LOGD("image_readPng() interlacing passes = %d", numberPasses);
        int pass;
        for (pass = 0; pass < numberPasses; pass++) {
        	int row;
            for (row = 0; row < height; row++) {
               png_read_row(pngPtr, ((unsigned char*)pixelData + (row * rowBytes)), NULL);
            }
        }
        // Read rest of file, and get additional chunks in info_ptr - REQUIRED
        png_read_end(pngPtr, infoPtr);
        // At this point you have read the entire image
        // Clean up after the read, and free any memory allocated - REQUIRE
        png_destroy_read_struct(&pngPtr, &infoPtr, &endInfo);
    } while(0);

    AAsset_close(asset);
    return pixelData;
}

PRIVATE void _image_init(Texture *tex, IMG_TYPE type) {
	tex->texId = -1;
	tex->type = type;
	tex->callback_deleteBind = tex->callback_param_graphic = NULL;
	tex->isBinded = FALSE;
	tex->width = tex->height = tex->widthPOT = tex->heightPOT = 0;
	tex->size = 0;
	tex->bytesPerPixel = 4;
	tex->quality = IMG_QUALITY_LINEAR;
	tex->pixels = NULL;
	tex->usedCount = 1;
	return;
}

PUBLIC Texture *image_createEmptyPng(unsigned int width, unsigned int height) {
	Texture *tex = (Texture *)MALLOC(sizeof(Texture));
	_image_init(tex, IMG_TYPE_RES);
	tex->widthPOT = tex->width = width;
	tex->heightPOT = tex->height = height;
	tex->pixels = MALLOC(width * height);
	return tex;
}

PUBLIC Texture *image_createPngPOT(const char *filePath, IMG_QUALITY quality) {
	char *resData;
	Texture *tex = (Texture *)MALLOC(sizeof(Texture));
	_image_init(tex, IMG_TYPE_RES);
	tex->quality = quality;
	strcpy(tex->filePath, filePath);
	unsigned char *file = jni_lib_readFromAssets(filePath, &(tex->size));
	resData = png_reader_parser(file, &(tex->width), &(tex->height), NULL, NULL);
	FREE(file);
	if (FALSE == _image_checkPOT(tex->width, tex->height)) {
		tex->pixels = (unsigned char *)pot_bitmapToTexturePOT(resData,
												tex->width, tex->height,
												&(tex->widthPOT), &(tex->heightPOT),
												tex->bytesPerPixel, NULL);
		FREE(resData);
	} else {
		tex->widthPOT = tex->width;
		tex->heightPOT = tex->height;
		tex->pixels = resData;
	}
	return tex;
}

PUBLIC Texture *image_crateTextPngPOT(const char *text, int fontsize, int strokeWidth, int paintStyle,
															int r, int g, int b, int a, int strokeR, int strokeG, int strokeB, int strokeA, int *textPixelSize) {
	Texture *tex = (Texture *)MALLOC(sizeof(Texture));
	_image_init(tex, IMG_TYPE_TEXT);
	jobject bitmap;
	if (TRUE == jni_lib_textToBitmapPOT(text, fontsize, strokeWidth, paintStyle, &bitmap,
										&(tex->width), &(tex->height),
										&(tex->widthPOT), &(tex->heightPOT),
										textPixelSize, r, g, b, a, strokeR, strokeG, strokeB, strokeA)) {
		jni_lib_bitmapToPixels(bitmap, &tex->pixels);
	}
	return tex;
}

PUBLIC void image_destroy(Texture *img) {
	if (NULL != img) {
		if (NULL != img->callback_deleteBind) {
			img->callback_deleteBind(img->callback_param_graphic, img);
		}
		if (IMG_TYPE_TEXT != img->type)
			FREE(img->pixels);
		FREE(img);
	}
	return;
}

PUBLIC void image_setCallBackFun(Texture *tex, CALLBACK_FUN_DELETE_BIND callbackFun, void *graphic) {
	tex->callback_deleteBind = callbackFun;
	tex->callback_param_graphic = graphic;
	return;
}

PRIVATE int clamp(int val) {
  if (val < 0) return 0;
  if (val > 255) return 255;
  return val;
};

PUBLIC unsigned char *image_iconMergeTint(unsigned char *image, unsigned char *mask,
									unsigned char *backgroundImage,
									/*unsigned char *resultBitmap,*/
									unsigned char *rgba) {
	unsigned int i, byteCount;
	byteCount = (64 * 64) << 2;		/* byte count of each layer ( 4bytes / pixel ) */
	i = 0;
	unsigned char *resultBitmap = MALLOC(byteCount);
	do {
		int r = i; int g = i + 1; int b = i + 2; int a = i + 3;
		resultBitmap[r] = rgba[0];
		resultBitmap[g] = rgba[1];
		resultBitmap[b] = rgba[2];
		resultBitmap[a] = rgba[3];
		/* image */
		if (255 != resultBitmap[a] && 0 != image[a]) {
			if (0 != resultBitmap[a]) {
				float alpha1 = resultBitmap[a] / 255.0f;
				float alpha2 = image[a] / 255.0f;
				float R = resultBitmap[r] * alpha1 + image[r] * alpha2 * (1 - alpha1);
				float G = resultBitmap[g] * alpha1 + image[g] * alpha2 * (1 - alpha1);
				float B = resultBitmap[b] * alpha1 + image[b] * alpha2 * (1 - alpha1);
				float A = 1 - (1 - alpha1) * (1 - alpha2);
				resultBitmap[r] = clamp((int)(R / A) + 0.5);
				resultBitmap[g] = clamp((int)(G / A) + 0.5);
				resultBitmap[b] = clamp((int)(B / A) + 0.5);
				resultBitmap[a] = clamp((int)(A * 255) + 0.5);
			} else {
				resultBitmap[r] = image[r];
				resultBitmap[g] = image[g];
				resultBitmap[b] = image[b];
				resultBitmap[a] = image[a];
			}
		}
		/* mask */
		if (255 != resultBitmap[a] && 0 != mask[a]) {
			if (0 != resultBitmap[a]) {
				float alpha1 = resultBitmap[a] / 255.0f;
				float alpha2 = mask[a] / 255.0f;
				float R = resultBitmap[r] * alpha1 + mask[r] * alpha2 * (1 - alpha1);
				float G = resultBitmap[g] * alpha1 + mask[g] * alpha2 * (1 - alpha1);
				float B = resultBitmap[b] * alpha1 + mask[b] * alpha2 * (1 - alpha1);
				float A = 1 - (1 - alpha1) * (1 - alpha2);
				resultBitmap[r] = clamp((int)(R / A) + 0.5);
				resultBitmap[g] = clamp((int)(G / A) + 0.5);
				resultBitmap[b] = clamp((int)(B / A) + 0.5);
				resultBitmap[a] = clamp((int)(A * 255) + 0.5);
			} else {
				resultBitmap[r] = mask[r];
				resultBitmap[g] = mask[g];
				resultBitmap[b] = mask[b];
				resultBitmap[a] = mask[a];
			}
		}
		/* backgroundImage */
		if (255 != resultBitmap[a] && 0 != backgroundImage[a]) {
			if (0 != resultBitmap[a]) {
				float alpha1 = resultBitmap[a] / 255.0f;
				float alpha2 = backgroundImage[a] / 255.0f;
				float R = resultBitmap[r] * alpha1 + backgroundImage[r] * alpha2 * (1 - alpha1);
				float G = resultBitmap[g] * alpha1 + backgroundImage[g] * alpha2 * (1 - alpha1);
				float B = resultBitmap[b] * alpha1 + backgroundImage[b] * alpha2 * (1 - alpha1);
				float A = 1 - (1 - alpha1) * (1 - alpha2);
				resultBitmap[r] = clamp((int)(R / A) + 0.5);
				resultBitmap[g] = clamp((int)(G / A) + 0.5);
				resultBitmap[b] = clamp((int)(B / A) + 0.5);
				resultBitmap[a] = clamp((int)(A * 255) + 0.5);
			} else {
				resultBitmap[r] = backgroundImage[r];
				resultBitmap[g] = backgroundImage[g];
				resultBitmap[b] = backgroundImage[b];
				resultBitmap[a] = backgroundImage[a];
			}
		}
		i += 4;
	} while (i < byteCount);
	return resultBitmap;
}

