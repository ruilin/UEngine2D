/*
 * rect.c
 *
 *  Created on: 2013-10-18
 *      Author: ryan
 */

#include "rect.h"


PUBLIC void rect_init(Rect *rect) {
	rect->left = rect->right = rect->top = rect->bottom = rect->width = rect->height = 0;
	return;
}

PUBLIC void rect_set(Rect *rect, int x, int y, int w, int h) {
	rect->left = x;
	rect->top = y;
	rect->width = w;
	rect->height = h;
	rect->right = rect->left + rect->width;
	rect->bottom = rect->top + rect->height;
	return;
}

PUBLIC BOOL rect_isInside(Rect *rect, int x, int y) {
	return (x >= rect->left
			&& x <= (rect->right)
			&& y >= rect->top
			&& y <= (rect->bottom));
}

PUBLIC void rectf_init(Rectf *rect) {
	rect->left = rect->right = rect->top = rect->bottom = rect->width = rect->height = 0;
	return;
}

PUBLIC void rectf_set(Rectf *rect, float x, float y, float w, float h) {
	rect->left = x;
	rect->top = y;
	rect->width = w;
	rect->height = h;
	rect->right = rect->left + rect->width;
	rect->bottom = rect->top + rect->height;
	return;
}

PUBLIC BOOL rectf_isInside(Rectf *rect, float x, float y) {
	return (x > rect->left
			&& x < (rect->right)
			&& y > rect->top
			&& y < (rect->bottom));
}
