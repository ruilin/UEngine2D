/*
 * view_debug.c
 *
 *  Created on: 2013-10-31
 *      Author: ryan
 */

#include "view_debug.h"

PRIVATE BOOL _view_debug_onEvent(void *_this, void *super, MOTION_ACTION action, int x, int y) {
	View_SysMsg *ctrl = (View_SysMsg *)_this;
	return FALSE;
}

PRIVATE BOOL _view_debug_render(void *_this, void *super, Graphic *g) {
	View_SysMsg *ctrl = (View_SysMsg *)_this;
	Debug *sys = ui_getSystemMsg();
	char msg[256];
	sprintf(msg, "Fps: %d", sys->fps);
	graphic_setTextColor(g, 10, 255, 10, 255);
	graphic_setColor4f(g, 1.0f, 1.0f, 1.0f, 1.0f);
	graphic_setTextStyle(g, 20, TEXT_STYLE_FILL);
	canvas_draw_string(msg, g, 10, 5, ALIGN_LEFT);

	return TRUE;
}

PRIVATE BOOL _view_debug_layout(void *_this, void *super, Rect rect) {
	View_SysMsg *ctrl = (View_SysMsg *)_this;
	return TRUE;
}

PUBLIC View_SysMsg *view_debug_create() {
	View_SysMsg *sysmsg = MALLOC(sizeof(View_SysMsg));
	if (NULL == sysmsg) {
		LOGE("view_sys_msg_create() MALLOC FAIL!");
		return NULL;
	}
	sysmsg->view = view_create("debug", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	view_extend(sysmsg->view, sysmsg, _view_debug_onEvent, _view_debug_render, _view_debug_layout);
	return sysmsg;
}

PUBLIC void view_debug_destroy(View_SysMsg *sysMsg) {
	if (NULL != sysMsg) {
		view_destroy(sysMsg->view);
		FREE(sysMsg);
	}
	return;
}
