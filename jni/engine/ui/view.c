/*
 * view.c
 *
 *  Created on: 2013-10-18
 *      Author: ryan
 */

#include "view.h"


static unsigned short nextId = 10;

PUBLIC View *view_create(const char *name, int x, int y, int width, int height) {
	View *view = MALLOC(sizeof(View));
	if (NULL == view) {
		LOGE("view_create() NULL == view");
		return NULL;
	}
	linked_list_item_init(&view->linkedItem);
	view->linked_subView = linked_list_create(NULL);
	view->id = nextId++;
	rect_set(&view->rect, x, y, width, height);
	view->instance = NULL;
	view->callback_onEvent = NULL;
	view->callback_onDraw = NULL;
	view->callback_onLayout = NULL;
	view->isShowing = FALSE;
	strcpy(view->name, name);
	return view;
}

PUBLIC View *view_createByRect(const char *name, Rect rect) {
	return view_create(name, rect.left, rect.top, rect.width, rect.height);
}

PUBLIC View *view_createEmpty(const char *name) {
	return view_create(name, 0, 0, 0, 0);
}

PUBLIC void view_destroy(void *_view) {
	if (NULL != _view) {
		View *view = (View *)_view;
		linked_list_destroy(view->linked_subView);
		FREE(view);
	}
	return;
}

PUBLIC void view_extend(View *view, void *instance,
						VIEW_CALLBACK_EVENT callback_trigger,
						VIEW_CALLBACK_RENDER callback_render,
						VIEW_CALLBACK_LAYOUT callback_layout) {
	view->instance = instance;
	view->callback_onEvent = callback_trigger;
	view->callback_onDraw = callback_render;
	view->callback_onLayout = callback_layout;
	return;
}

PUBLIC BOOL view_onEvent(View *view, View *superView, MOTION_ACTION action, int x, int y) {
	void *iterator = linked_list_iterator(view->linked_subView, LINKED_LIST_TAIL);
	while (NULL != iterator) {
		View *subView = linked_list_iterator_prev(&iterator);
		if (TRUE == view_onEvent(subView, view, action, x, y)) {
			/* 递归所有子视图的事件处理 */
			return TRUE;
		}
	}
	if (NULL != view->instance) {
		return view->callback_onEvent(view->instance, (NULL != superView ? superView->instance : NULL), action, x, y);
	}
	return FALSE;
}

PUBLIC BOOL view_onDraw(View *view, View *superView, Graphic *g) {
	if (NULL != view->instance) {
		view->callback_onDraw(view->instance, (NULL != superView ? superView->instance : NULL), g);
	}
	void *iterator = linked_list_iterator(view->linked_subView, LINKED_LIST_TAIL);
	while (NULL != iterator) {
		View *subView = linked_list_iterator_prev(&iterator);
		view_onDraw(subView, view, g);	/* 递归所有子视图的绘制 */
	}
	return TRUE;
}

PUBLIC void view_setHide(View *view, BOOL fHide) {
	view->isShowing = !fHide;
	return;
}

PUBLIC void view_addSubView(View *view, View *subView) {
	if (NULL == view || NULL == subView) {
		LOGE("view_addSubView() NULL == view || NULL == subView");
		return;
	}
	if (NULL == subView->callback_onLayout) {
		LOGE("view_addSubView() Error: subView does not extends view!!!");
	}
	subView->callback_onLayout(subView, view, view->rect);
	linked_list_addItem(view->linked_subView, &(subView->linkedItem), (void *)subView);
	return;
}

PUBLIC void view_removeSubView(View *view, View *subView) {
	if (NULL == view || NULL == subView) {
		LOGE("view_addSubView() NULL == view || NULL == subView");
		return;
	}
	linked_list_removeItem(view->linked_subView, &(subView->linkedItem), TRUE);
	return;
}
