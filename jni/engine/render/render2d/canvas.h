/*
 * canvas.h
 *
 *  Created on: 2013-9-21
 *      Author: ryan
 */

#ifndef CANVAS_H_
#define CANVAS_H_

#include <math.h>
#include <GLES/gl.h>
#include <GLES/glext.h>

#include <base.h>
#include <resource/image.h>
#include <data/rect.h>

#include "graphic.h"
#include "text.h"

typedef enum {
	ALIGN_RIGHT		= 0,
	ALIGN_LEFT		= 1,
	ALIGN_CENTER_HORIZONTAL	= 2,
	ALIGN_CENTER_VERTICAL	= 3,
	ALIGN_CENTER	= 4,
} CANVAS_ALIGN;

/* 画布操作 */
#define canvas_save() (glPushMatrix())
#define canvas_load() (glPopMatrix())
#define canvas_scalef(x, y, z) (glScalef((x), (y), (z)))								/* 缩放: 沿着原点(0, 0, 0)缩放，坐标会跟着改变 */
#define canvas_translatef(x, y, z) (glTranslatef((x), (y), (z)))						/* 移动 */
#define canvas_rotatef(angle, x, y, z) (glRotatef((angle), (x), (y), (z)))		/* 旋转 */

void canvas_init(int screenWidth, int screenHeight,
				unsigned short canvasWidth, unsigned short canvasHeight);
void canvas_fillRGB(float r, float g, float b);
void canvas_draw_bitmap(Texture *tex, Graphic *g, int x, int y);
void canvas_draw_bitmap_merror(Texture *tex, Graphic *g, int x, int y);
void canvas_draw_bitmapClip(Texture *tex, Graphic *g, int x, int y, Rect rect);
void canvas_draw_bitmapClipRatio(Texture *tex, Graphic *g, int x, int y,
								float POTw, float POTh,
								float sxRatio, float exRatio,
								float syRatio, float eyRatio);
void canvas_draw_bitmapClipRatio_merror(Texture *tex, Graphic *g, int x, int y,
										float POTw, float POTh,
										float sxRatio, float exRatio,
										float syRatio, float eyRatio);
void canvas_draw_bitmapClipRatio_trans(Texture *tex, Graphic *g, float glVertices[8],
										float sxRatio, float exRatio,
										float syRatio, float eyRatio);
void canvas_draw_figure();
void canvas_fill_Rect4f(Graphic *g, float x1, float y1, float width, float height);
void canvas_draw_Rect4f(Graphic *g, float x1, float y1, float width, float height);
void canvas_draw_Rect(Graphic *g, Rect *rect);
void canvas_fill_Rect(Graphic *g, Rect *rect);
void canvas_draw_point(Graphic *g, float x1, float y1, float size);
void canvas_draw_line(Graphic *g, float x1, float y1, float x2, float y2, float width);
void canvas_fill_circle(Graphic *g, int cx, int cy, float radius);
void canvas_fill_ellipse(Graphic *g, int cx, int cy, float radius);
Texture *canvas_draw_string(const char *str, Graphic *g, int x, int y, CANVAS_ALIGN align);

#endif /* CANVAS_H_ */
