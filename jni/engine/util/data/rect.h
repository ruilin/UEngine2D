/*
 * rect.h
 *
 *  Created on: 2013-10-18
 *      Author: ryan
 */

#ifndef RECT_H_
#define RECT_H_

#include <base.h>

typedef struct {
	int left;
	int top;
	int right;
	int bottom;
	int width;
	int height;
} Rect;

typedef struct {
	float left;
	float top;
	float right;
	float bottom;
	float width;
	float height;
} Rectf;


void rect_init(Rect *rect);
void rect_set(Rect *rect, int x, int y, int w, int h);
BOOL rect_isInside(Rect *rect, int x, int y);

void rectf_init(Rectf *rect);
void rectf_set(Rectf *rect, float x, float y, float w, float h);
BOOL rectf_isInside(Rectf *rect, float x, float y);

#endif /* RECT_H_ */
