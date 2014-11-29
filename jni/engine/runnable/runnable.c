/*
 * runnable.c
 *
 *  Created on: 2013-10-26
 *      Author: ryan
 */

#include "runnable.h"

typedef struct runnable {
	BOOL isFirsh;
} Runnable;

PRIVATE Runnable runnable;

PRIVATE BOOL _runnable_loading_finish() {

	return TRUE;
}

PUBLIC BOOL runnable_init(Engine *engine) {
	runnable.isFirsh = TRUE;

	engine->g = graphic_create();
	gthread_init();
	res_init();
	ui_init(engine->screenWidth, engine->screenHeight, engine->canvasWidth, engine->canvasHeight);
	ui_setSystemMsg(engine->debug);
	logic_init(engine);
	return TRUE;
}

PUBLIC BOOL runnable_end(Engine *engine) {
	logic_end(engine);
	ui_end();
	res_end();
	graphic_destroy(engine->g); engine->g = NULL;
	gthread_end();

	return TRUE;
}

PUBLIC BOOL runnable_run(Engine *engine) {
	if (TRUE == engine->isLoading) {
		return FALSE;
	}
	MotionEvent *event = linked_list_isEmpty(engine->linked_event) ? NULL : linked_list_getTail(engine->linked_event);
	ui_run(event);
	if (NULL != event) {
		linked_list_removeItem(engine->linked_event, &event->linkedItem, TRUE);
	}
	return logic_run(engine);
}
