/*
 * canvas.c
 *
 *  Created on: 2013-9-21
 *      Author: ryan
 */

#include "canvas.h"

PRIVATE const GLfloat PI = 3.1415927f;
PRIVATE inline float GET_ABS(float x) {return x>0?x:-x;}

PUBLIC BOOL mTextureStateEnabled;
#define GL_ENABLE_TEXTURE()			(FALSE == mTextureStateEnabled ? ( glEnable(GL_TEXTURE_2D), mTextureStateEnabled = TRUE ) : 0)
#define GL_DISABLE_TEXTURE()		(TRUE == mTextureStateEnabled ? ( glDisable(GL_TEXTURE_2D), mTextureStateEnabled = FALSE ) : 0)

PUBLIC void canvas_init(int screenWidth, int screenHeight,
						unsigned short canvasWidth, unsigned short canvasHeight) {
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
    glDisable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glDisable(GL_DEPTH_TEST);

	glViewport(0, 0, screenWidth, screenHeight);	/* 视口尺寸 */
	glMatrixMode(GL_PROJECTION);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glLoadIdentity();
	/* locks at our desired resolution */
	glOrthof(0, canvasWidth, canvasHeight, 0, -1.0, 1.0);	/* 坐标轴映射 */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	GL_DISABLE_TEXTURE();
	return;
}

PUBLIC void canvas_fillRGB(float r, float g, float b) {
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
	return;
}

/*
 * 画图片
 */
PUBLIC void canvas_draw_bitmap(Texture *tex, Graphic *g, int x, int y) {
	unsigned int texId;
	if (NULL == tex) goto _exit;
	texId = graphic_genTexture(g, tex);
	if (-1 == texId) goto _exit;
	GL_ENABLE_TEXTURE();
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glColor4f(g->red, g->green, g->blue, g->alpha);
    GLfloat POTw = tex->widthPOT;
    GLfloat POTh = tex->heightPOT;
    GLfloat xr = 1;
    GLfloat yr = 1;
    GLfloat vertices[8] = { x+POTw, y,  x, y,  x+POTw, y+POTh,  x, y+POTh };
	GLfloat textureCoords[8] = { xr, 0.0,  0.0, 0.0,  xr, yr,  0.0, yr };
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texId);
    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glTexCoordPointer(2, GL_FLOAT, 0, textureCoords);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glPopMatrix();
_exit:
	return;
}

PUBLIC void canvas_draw_bitmap_merror(Texture *tex, Graphic *g, int x, int y) {
	unsigned int texId;
	if (NULL == tex) goto _exit;
	texId = graphic_genTexture(g, tex);
	if (-1 == texId) goto _exit;
	GL_ENABLE_TEXTURE();
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glColor4f(g->red, g->green, g->blue, g->alpha);
    GLfloat POTw = tex->widthPOT;
    GLfloat POTh = tex->heightPOT;
    GLfloat xr = 1;
    GLfloat yr = 1;
    x -= (tex->widthPOT - tex->width);
    GLfloat vertices[8] = { x, y, x+POTw, y, x, y+POTh, x+POTw, y+POTh };
	GLfloat textureCoords[8] = { xr, 0.0,  0.0, 0.0,  xr, yr,  0.0, yr };
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texId);
    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glTexCoordPointer(2, GL_FLOAT, 0, textureCoords);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glPopMatrix();
_exit:
	return;
}

PUBLIC void canvas_draw_bitmapClip(Texture *tex, Graphic *g, int x, int y, Rect rect) {
	unsigned int texId;
	if (NULL == tex) goto _exit;
	texId = graphic_genTexture(g, tex);
	if (-1 == texId) goto _exit;

	GL_ENABLE_TEXTURE();
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glColor4f(g->red, g->green, g->blue, g->alpha);
    if (rect.width > tex->widthPOT || rect.height > tex->heightPOT) {
    	goto _exit;
    	LOGE("canvas_draw_bitmapClip() rect.Width > tex->widthPOT || rect.height > tex->heightPOT");
    }
    GLfloat POTw = rect.width;
    GLfloat POTh = rect.height;
    GLfloat sxRatio = rect.left / tex->widthPOT;
    GLfloat syRatio = rect.top / tex->heightPOT;
    GLfloat exRatio = (rect.left + rect.width) / tex->widthPOT;
    GLfloat eyRatio = (rect.top + rect.height) / tex->heightPOT;
    GLfloat vertices[8] = { x+POTw, y,  x, y,  x+POTw, y+POTh,  x, y+POTh };		/* 反Z字型 */
	GLfloat textureCoords[8] = { exRatio, syRatio,  sxRatio, syRatio,  exRatio, eyRatio,  sxRatio, eyRatio };

	glBindTexture(GL_TEXTURE_2D, texId);
    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glTexCoordPointer(2, GL_FLOAT, 0, textureCoords);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
_exit:
	return;
}

/*
 * POTw / POTh 要画的区域POT尺寸
 */
PUBLIC void canvas_draw_bitmapClipRatio(Texture *tex, Graphic *g, int x, int y,
										float POTw, float POTh,
										float sxRatio, float exRatio,
										float syRatio, float eyRatio) {
	unsigned int texId;
	if (NULL == tex) goto _exit;
	texId = graphic_genTexture(g, tex);
	if (-1 == texId) goto _exit;

	GL_ENABLE_TEXTURE();
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glColor4f(g->red, g->green, g->blue, g->alpha);
    /* 反‘Z’字形 */
    GLfloat vertices[8] = {
    						x + POTw, y,
    						x, 		  y,
							x + POTw, y + POTh,
							x, 		  y + POTh
    						};
	GLfloat textureCoords[8] = {
								exRatio, syRatio,
								sxRatio, syRatio,
								exRatio, eyRatio,
								sxRatio, eyRatio
								};
    glBindTexture(GL_TEXTURE_2D, texId);
    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glTexCoordPointer(2, GL_FLOAT, 0, textureCoords);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
_exit:
	return;
}

/*
 * 镜像绘图
 */
PUBLIC void canvas_draw_bitmapClipRatio_merror(Texture *tex, Graphic *g, int x, int y,
										float POTw, float POTh,
										float sxRatio, float exRatio,
										float syRatio, float eyRatio) {
	unsigned int texId;
	if (NULL == tex) goto _exit;
	texId = graphic_genTexture(g, tex);
	if (-1 == texId) goto _exit;

	GL_ENABLE_TEXTURE();
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glColor4f(g->red, g->green, g->blue, g->alpha);
    /* 反‘Z’字形 */
    GLfloat vertices[8] = {
    						x, 		  y,
    						x + POTw, y,
							x, 		  y + POTh,
							x + POTw, y + POTh,
    					 	};
	GLfloat textureCoords[8] = {
								exRatio, syRatio,
								sxRatio, syRatio,
								exRatio, eyRatio,
								sxRatio, eyRatio,
								};
    glBindTexture(GL_TEXTURE_2D, texId);
    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glTexCoordPointer(2, GL_FLOAT, 0, textureCoords);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
_exit:
	return;
}

/*
 * POTw / POTh 要画的区域POT尺寸
 * glVertices: 反‘Z’字形
 */
PUBLIC void canvas_draw_bitmapClipRatio_trans(Texture *tex, Graphic *g, float glVertices[8],
										float sxRatio, float exRatio,
										float syRatio, float eyRatio) {
	unsigned int texId;
	if (NULL == tex) goto _exit;
	texId = graphic_genTexture(g, tex);
	if (-1 == texId) goto _exit;

	GL_ENABLE_TEXTURE();
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glColor4f(g->red, g->green, g->blue, g->alpha);
	GLfloat textureCoords[8] = {
								exRatio, syRatio,
								sxRatio, syRatio,
								exRatio, eyRatio,
								sxRatio, eyRatio
								};
    glBindTexture(GL_TEXTURE_2D, texId);
    glVertexPointer(2, GL_FLOAT, 0, glVertices);
    glTexCoordPointer(2, GL_FLOAT, 0, textureCoords);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
_exit:
	return;
}

/*
 * 画图形
 */
PUBLIC void canvas_draw_figure() {
	float width = 200;
	float height = 100;
	float x1 = 200;
	float y1 = 100;
	float x2 = x1 + width;
	float y2 = y1 + height;
	float vertex[8] = {x1, y1, x2, y1, x2, y2, x1, y2};
	GL_DISABLE_TEXTURE();
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glColor4f(0.0f, 1.0f, 0.5f, 1.0f);
//	glLineWidth(2.0f);
//	glPointSize(5.0f);
	glVertexPointer(2, GL_FLOAT, 0, vertex);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
//	glEnable(GL_TEXTURE_2D);

//	glDisableClientState(GL_VERTEX_ARRAY);
//	glDisableClientState(GL_COLOR_ARRAY);
//	glFlush();
}

PUBLIC void canvas_fill_Rect4f(Graphic *g, float x1, float y1, float width, float height) {
	float x2 = x1 + width;
	float y2 = y1 + height;
	float vertex[8] = {x1, y1, x2, y1, x2, y2, x1, y2};
	GL_DISABLE_TEXTURE();
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glColor4f(g->red, g->green, g->blue, g->alpha);
	glVertexPointer(2, GL_FLOAT, 0, vertex);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}

PUBLIC void canvas_fill_Rect(Graphic *g, Rect *rect) {
	float x2 = rect->left + rect->width;
	float y2 = rect->top + rect->height;
	float vertex[8] = {rect->left, rect->top, x2, rect->top, x2, y2, rect->left, y2};
	GL_DISABLE_TEXTURE();
	glColor4f(g->red, g->green, g->blue, g->alpha);
	glVertexPointer(2, GL_FLOAT, 0, vertex);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

PUBLIC void canvas_draw_point(Graphic *g, float x1, float y1, float size) {
	float vertex[2] = {x1, y1};
	GL_DISABLE_TEXTURE();
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glColor4f(g->red, g->green, g->blue, g->alpha);
	glPointSize(size);
	glVertexPointer(2, GL_FLOAT, 0, vertex);
	glDrawArrays(GL_POINTS, 0, 1);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}

PUBLIC float fastmath_carmsqrt(float x) {
	/* note: might only works on 1 little-endian */
	union{
		int intPart;
		float floatPart;
	} convertor;
	union{
		int intPart;
		float floatPart;
	} convertor2;
	convertor.floatPart = x;
	convertor2.floatPart = x;
	convertor.intPart = 0x1FBCF800 + (convertor.intPart >> 1);
	convertor2.intPart = 0x5f3759df - (convertor2.intPart >> 1);
	return 0.5f*(convertor.floatPart + (x * convertor2.floatPart));
}

PRIVATE void _draw_line(float x_1, float y_1, float x_2, float y_2,	 	/* coordinates of the line */
						float w) {										/* width/thickness of the line in pixel */
	/* Caller needs to set the color/alpha: glColor4f() before calling this function. */
	float t; float R;
	float tx, ty; /* core thinkness of a line */
	float Rx, Ry; /* fading edge of a line */
	float cx ,cy; /* cap of a line */
	float ALW;
	float dx;
	float dy;
	float line_vertex[16];

	float f= w-round(w);

	t = R = 0.0;

	/* determine parameters t,R */
	if (w >= 0.0 && w < 1.0) {
		t = 0.05; R = 0.48 + 0.32 * f;
	} else if (w >= 1.0 && w < 2.0) {
		t = 0.05 + f * 0.33; R = 0.768 + 0.312 * f;
	} else if (w >= 2.0 && w < 3.0) {
		t = 0.38 + f * 0.58; R = 1.08;
	} else if (w >= 3.0 && w < 4.0) {
		t= 0.96 + f * 0.48; R = 1.08;
	} else if (w >= 4.0 && w < 5.0) {
		t= 1.44 + f * 0.46; R = 1.08;
	} else if (w >= 5.0 && w < 6.0) {
		t = 1.9 + f * 0.6; R = 1.08;
	} else if (w >= 6.0) {
		float ff = w - 6.0;
		t = 2.5 + ff * 0.50; R = 1.08;
	}

	/* determine angle of the line to horizontal */
	tx = 0; ty = 0; /* core thinkness of a line */
	Rx = 0; Ry = 0; /* fading edge of a line */
	cx = 0; cy = 0; /* cap of a line */
	ALW = 0.01;
	dx = x_2 - x_1;
	dy = y_2 - y_1;
	if (GET_ABS(dx) < ALW) { /* vertical */
		tx = t; ty = 0;
		Rx = R; Ry = 0;
		if (w > 0.0 && w < 1.0)
			tx *= 8;
		else if ((w > 0.99999) && (w < 1.00001))
			tx *= 10;
	} else if (GET_ABS(dy) < ALW) { /* horizontal */
		tx = 0; ty = t;
		Rx = 0; Ry = R;
		if (w > 0.0 && w < 1.0)
			ty *= 8;
		else if ((w > 0.99999) && (w < 1.00001))
			ty *= 10;
	} else {
		if (w < 3) { /* approximate to make things even faster */
			float m =dy / dx;
			/* and calculate tx, ty, Rx, Ry */
			if (m > -0.4142 && m <= 0.4142) {
				/* -22.5< angle <= 22.5, approximate to 0 (degree) */
				tx = t * 0.1; ty = t;
				Rx = R * 0.6; Ry = R;
			} else if (m > 0.4142 && m <= 2.4142) {
				/* 22.5< angle <= 67.5, approximate to 45 (degree) */
				tx = t * -0.7071; ty = t * 0.7071;
				Rx = R * -0.7071; Ry = R * 0.7071;
			} else if (m > 2.4142 || m <= -2.4142) {
				/* 67.5 < angle <=112.5, approximate to 90 (degree) */
				tx = t; ty = t * 0.1;
				Rx = R; Ry = R * 0.6;
			} else if (m > -2.4142 && m < -0.4142) {
				/* 112.5 < angle < 157.5, approximate to 135 (degree) */
				tx = t * 0.7071; ty = t * 0.7071;
				Rx = R * 0.7071; Ry = R * 0.7071;
			} else {
				/* error in determining angle */
				LOGE("_draw_line(): error in determining angle: m=%.4f", m);
			}
		} else { /* calculates to exact */
			float L;
			dx = y_1 - y_2;
			dy = x_2 - x_1;
			L = fastmath_carmsqrt(dx*dx+dy*dy);
			dx /= L;
			dy /= L;
			cx = -0.6 * dy; cy = 0.6 * dx;
			tx = t * dx; ty = t * dy;
			Rx = R * dx; Ry = R * dy;
		}
	}

	/* draws the line by triangle strip */
	line_vertex[0] = x_1 - tx - Rx;
	line_vertex[1] = y_1 - ty - Ry;
	line_vertex[2] = x_2 - tx - Rx;
	line_vertex[3] = y_2 - ty - Ry;
	line_vertex[4] = x_1 - tx;
	line_vertex[5] = y_1 - ty;
	line_vertex[6] = x_2 - tx;
	line_vertex[7] = y_2 - ty;
	line_vertex[8] = x_1 + tx;
	line_vertex[9] = y_1 + ty;
	line_vertex[10] = x_2 + tx;
	line_vertex[11] = y_2 + ty;
	line_vertex[12] = x_1 + tx + Rx;
	line_vertex[13] = y_1 + ty + Ry;
	line_vertex[14] = x_2 + tx + Rx;
	line_vertex[15] = y_2 + ty + Ry;
	glVertexPointer(2, GL_FLOAT, 0, line_vertex);

	if ((GET_ABS(dx) < ALW || GET_ABS(dy) < ALW) && w <= 1.0) {
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
	} else {
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 8);
	}

	/* cap */
	if (w < 3) {
		/* do not draw cap */
	} else {
		/* draws cap */
		float line_vertex_2[] = {
			x_1 - Rx + cx, y_1 - Ry + cy,	/* cap1 */
			x_1 + Rx + cx, y_1 + Ry + cy,
			x_1 - tx - Rx, y_1 - ty - Ry,
			x_1 + tx + Rx, y_1 + ty + Ry,
			x_2 - Rx - cx, y_2 - Ry - cy,	/* cap2 */
			x_2 + Rx - cx, y_2 + Ry - cy,
			x_2 - tx - Rx, y_2 - ty - Ry,
			x_2 + tx + Rx, y_2 + ty + Ry
		};
		glVertexPointer(2, GL_FLOAT, 0, line_vertex_2);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
	}
	return;
}

PUBLIC void canvas_draw_line(Graphic *g, float x1, float y1, float x2, float y2, float width) {
	GL_DISABLE_TEXTURE();
	glColor4f(g->red, g->green, g->blue, g->alpha);
	_draw_line(x1, y1, x2, y2, width);
	return;
}

PUBLIC void canvas_draw_Rect(Graphic *g, Rect *rect) {
	float x1 = rect->left;
	float y1 = rect->top;
	float x2 = x1 + rect->width;
	float y2 = y1 + rect->height;
	GL_DISABLE_TEXTURE();
	glColor4f(g->red, g->green, g->blue, g->alpha);
	canvas_draw_line(g, x1, y1, x2, y1, 0.5);
	canvas_draw_line(g, x2, y1, x2, y2, 0.5);
	canvas_draw_line(g, x2, y2, x1, y2, 0.5);
	canvas_draw_line(g, x1, y2, x1, y1, 0.5);
	/*
	   	otto: 2013/02/04 for some reasons GL_LINE_LOOP is slow on some devices, we change from LINE_LOOP to draw 4
			 	lines instead.
		float x2 = rect->x + rect->width;
		float y2 = rect->y + rect->height;
		float vertex[8] = {rect->x, rect->y, x2, rect->y, x2, y2, rect->x, y2};
		GL_DISABLE_TEXTURE();
		glColor4f(g->red, g->green, g->blue, g->alpha);
		glVertexPointer(2, GL_FLOAT, 0, vertex);
		glDrawArrays(GL_LINE_LOOP, 0, 4);
	 */
}

PUBLIC void canvas_draw_Rect4f(Graphic *g, float x1, float y1, float width, float height) {
	float x2 = x1 + width;
	float y2 = y1 + height;
	GL_DISABLE_TEXTURE();
	glColor4f(g->red, g->green, g->blue, g->alpha);
	canvas_draw_line(g, x1, y1, x2, y1, 1);
	canvas_draw_line(g, x2, y1, x2, y2, 1);
	canvas_draw_line(g, x2, y2, x1, y2, 1);
	canvas_draw_line(g, x1, y2, x1, y1, 1);
}

#define _CIRCLE				10
#define _MAX_SEGMENTS		300		/* 300 maximum segments, 10 = 30 => 30 * 30 = 900 pixels raidus maximum*/

PUBLIC void canvas_fill_circle(Graphic *g, int cx, int cy, float radius) {
	PRIVATE float vertex[_MAX_SEGMENTS * 2];	/* static here to prevent excessive malloc()/free() */
	int num_segments = _CIRCLE * fastmath_carmsqrt(radius);
	float theta = 6.2831852 / (float)num_segments;	/* 2 * 3.1415926 */
	int ii;
	float c = cosf(theta);	/* precalculates the sine and cosine */
	float s = sinf(theta);
	float t;

	float x = radius;		/* we start at angle = 0 */
	float y = 0;

	if (_MAX_SEGMENTS < num_segments) {
		LOGE("gfx_opengl_fill_circle(): radius too large (%f)", radius);
		return;
	}
	GL_DISABLE_TEXTURE();
	glColor4f(g->red, g->green, g->blue, g->alpha);
	for(ii = 0; ii < num_segments; ii++) {
		int ii2 = ii << 1;
		vertex[ii2  ] = x + cx;
		vertex[ii2+1] = y + cy;
		/* applies the rotation matrix */
		t = x;
		x = c * x - s * y;
		y = s * t + c * y;
	}
	glVertexPointer(2, GL_FLOAT, 0, vertex);
	glDrawArrays(GL_TRIANGLE_FAN, 0, num_segments);
	return;
}

PUBLIC void canvas_fill_ellipse(Graphic *g, int cx, int cy, float radius) {
	glScalef(1.0, 0.5, 1.0);
	canvas_fill_circle(g, cx, (cy << 1), radius);
	glScalef(1.0, 2.0, 1.0);
	return;
}

PUBLIC Texture *canvas_draw_string(const char *str, Graphic *g, int x, int y, CANVAS_ALIGN align) {
	int textPixelSize;
	Texture *tex = text_getTextImage(str, g->fontSize, g->strokeWidth, g->paintStyle,
													g->textRed, g->textGreen, g->textBlue, g->textAlpha,
													g->textRed_stroke, g->textGreen_stroke, g->textBlue_stroke, g->textAlpha_stroke,
													&textPixelSize);
	switch (align) {
	case ALIGN_LEFT:
		canvas_draw_bitmap(tex, g, x, y);
		break;
	case ALIGN_RIGHT:
		canvas_draw_bitmap(tex, g, x - textPixelSize, y);
		break;
	case ALIGN_CENTER_HORIZONTAL:
		canvas_draw_bitmap(tex, g, x - (textPixelSize >> 1), y);
		break;
	case ALIGN_CENTER_VERTICAL:
		canvas_draw_bitmap(tex, g, x - textPixelSize, y - (g->fontSize >> 1));
		break;
	case ALIGN_CENTER:
		canvas_draw_bitmap(tex, g,  x - (textPixelSize >> 1), y - (g->fontSize >> 1) - 2);
		break;
	}
	return tex;
}

