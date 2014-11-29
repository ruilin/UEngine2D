/*
 * logic.c
 *
 *  Created on: 2013-9-3
 *      Author: Ryan
 */

#include "logic.h"

PRIVATE View_SysMsg *viewDebug;

PUBLIC BOOL logic_init(Engine *engine) {
	engine->canvasWidth = SCREEN_WIDTH;
	engine->canvasHeight = SCREEN_HEIGHT;
	viewDebug = view_debug_create();
	ui_addView(viewDebug->view);
	return TRUE;
}

PUBLIC BOOL logic_end(Engine *engine) {
	return TRUE;
}

PUBLIC BOOL logic_onEvent(MOTION_ACTION action, int x, int y) {
	return FALSE;
}

PUBLIC BOOL logic_run(Engine *engine) {
	return TRUE;
}

