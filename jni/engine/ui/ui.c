/*
 * ui.c
 *
 *  Created on: 2013-10-23
 *      Author: ryan
 */

#include "ui.h"


typedef struct {
	void *linked_view;
	unsigned short viewWidth;
	unsigned short viewHeight;
	Debug *sys;
} Ui_Manager;

PRIVATE Ui_Manager ui;
PRIVATE float xRaito = 1;
PRIVATE float yRaito = 1;

PUBLIC void ui_init(unsigned short screenWidth, unsigned short screenHeight,
							unsigned short canvasWidth, unsigned short canvasHeight) {
	ui.linked_view = linked_list_create(NULL);
	ui.viewWidth = canvasWidth;
	ui.viewHeight = canvasHeight;
	ui.sys = NULL;
	xRaito = ((float)canvasWidth / screenWidth);
	yRaito = ((float)canvasHeight / screenHeight);
	return;
}

PUBLIC void ui_end() {
	linked_list_destroy(ui.linked_view);
	ui.linked_view = NULL;
	return;
}

PUBLIC void ui_setSystemMsg(Debug *_sys) {
	ui.sys = _sys;
	return;
}

PRIVATE BOOL _ui_doEvent(MOTION_ACTION action, int x, int y, Ui_Manager *ui) {
	/* do input events */
	void *iterator = linked_list_iterator(ui->linked_view, LINKED_LIST_HEAD);
	while (NULL != iterator) {
		View *view = linked_list_iterator_next(&iterator);
		if (TRUE == view->isShowing && TRUE == view_onEvent(view, NULL, action, x, y)) {
			/* LOGI("_ui_doEvent(): view: '%s' got the events", view->name); */
			return TRUE;
		}
	}
	return FALSE;
}

PUBLIC void ui_addView(View *view) {
	if (NULL == view) {
		LOGE("ui_addView() view == null");
		return;
	}
	linked_list_addItem(ui.linked_view, &(view->linkedItem), (void *)view);
	view->isShowing = TRUE;
	return;
}

PUBLIC void ui_removeView(View *view) {
	if (NULL == view) {
		LOGE("ui_removeView() view == null");
		return;
	}
	linked_list_removeItem(ui.linked_view, &(view->linkedItem), TRUE);
	view->isShowing = FALSE;
	return;
}

PUBLIC BOOL ui_run(MotionEvent *event) {
	if (NULL != event) {
		if (0 < event->pointerSize) {
			int x = event->pointers[0].left * xRaito;
			int y = event->pointers[0].y * yRaito;
			MOTION_ACTION action = event->pointers[0].action;
			if (FALSE == _ui_doEvent(action, x, y, &ui)) {
				logic_onEvent(action, x, y);
			}
		}
	}
	return TRUE;
}

PUBLIC BOOL ui_render(Graphic *g) {
	void *iterator = linked_list_iterator(ui.linked_view, LINKED_LIST_TAIL);
	while (NULL != iterator) {
		View *view = linked_list_iterator_prev(&iterator);
		if (TRUE == view->isShowing)
			view_onDraw(view, NULL, g);
	}
	return TRUE;
}

PUBLIC Debug *ui_getSystemMsg() {
	return ui.sys;
}

