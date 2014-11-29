/*
 * view.h
 *
 *  Created on: 2013-10-18
 *      Author: ryan
 */

#ifndef VIEW_H_
#define VIEW_H_

#include <base.h>
#include <data/rect.h>
#include <util/linked_list.h>
#include <render2d/graphic.h>
#include "motion_event.h"

typedef BOOL (*VIEW_CALLBACK_EVENT)(void *instance, void *super, MOTION_ACTION action, int left, int top);
typedef BOOL (*VIEW_CALLBACK_RENDER)(void *instance, void *super, Graphic *g);
typedef BOOL (*VIEW_CALLBACK_LAYOUT)(void *instance, void *super, Rect rect);

typedef struct {
	Linked_Item linkedItem;
	unsigned short id;
	char name[32];
	void *instance;
	void *linked_subView;
	VIEW_CALLBACK_EVENT callback_onEvent;
	VIEW_CALLBACK_RENDER callback_onDraw;
	VIEW_CALLBACK_LAYOUT callback_onLayout;
	Rect rect;
	BOOL isShowing;
} View;

View *view_create(const char *name, int x, int y, int width, int height);
View *view_createByRect(const char *name, Rect rect);
View *view_createEmpty(const char *name);
void view_destroy(void *_view);
void view_extend(View *view, void *instance,
				VIEW_CALLBACK_EVENT callback_trigger,
				VIEW_CALLBACK_RENDER callback_render,
				VIEW_CALLBACK_LAYOUT callback_layout);
BOOL view_onEvent(View *view, View *superView, MOTION_ACTION action, int x, int y);
BOOL view_onDraw(View *view, View *superView, Graphic *g);
void view_setHide(View *view, BOOL fHide);
void view_addSubView(View *view, View *subView);
void view_removeSubView(View *view, View *subView);

#endif /* VIEW_H_ */
