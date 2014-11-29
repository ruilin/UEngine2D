/*
 * graphic.h
 *
 *  Created on: 2013-9-21
 *      Author: Administrator
 */

#ifndef GRAPHIC_H_
#define GRAPHIC_H_

#include <stddef.h>
#include <GLES/gl.h>
#include <GLES/glext.h>
#include <base.h>
#include <resource/image.h>

typedef enum {
	TEXT_STYLE_FILL							= 0,
	TEXT_STYLE_STROKE					= 1,
	TEXT_STYLE_FILL_AND_STROKE	= 2
} TEXT_STYLE;

typedef enum {
	GRAPHIC_ROTATE_0		= 0,
	GRAPHIC_ROTATE_90		= 1,
	GRAPHIC_ROTATE_180		= 2,
	GRAPHIC_ROTATE_270		= 3
} GRAPHIC_ROTATE;

typedef enum {
	GRAPHIC_TURN_NORMAL		= 0,
	GRAPHIC_TURN_BY_X		= 1,
	GRAPHIC_TURN_BY_Y		= 2,
} GRAPHIC_TURN;

typedef struct Graphic {
	GRAPHIC_ROTATE rotate;
	GRAPHIC_TURN turnover;
	void *hash_tex;
	float red;
	float green;
	float blue;
	float alpha;
	/* text set */
	int fontSize;
	int strokeWidth;
	int paintStyle;
	int textRed;				/* text color (0~255) */
	int textGreen;
	int textBlue;
	int textAlpha;
	int textRed_stroke;		/* text stroke color (0~255) */
	int textGreen_stroke;
	int textBlue_stroke;
	int textAlpha_stroke;
} Graphic;

Graphic *graphic_create();
void graphic_destroy(Graphic *g);
unsigned int graphic_genTexture(Graphic *g, Texture *tex);
void graphic_recyleTexture(void *graphic, void *img);
void graphic_setColor4f(Graphic *g, float red, float green, float blue, float alpha);
void graphic_setTextStyle(Graphic *g, int fontSize, TEXT_STYLE paintStyle);
void graphic_setTextColor(Graphic *g, int red, int green, int blue, int alpha);
void graphic_setTextStrokeColor(Graphic *g, int red, int green, int blue, int alpha);

#endif /* GRAPHIC_H_ */
