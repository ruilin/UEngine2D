/*
 * motion_event.c
 *
 *  Created on: 2013-9-28
 *      Author: Ryan
 */

#include "motion_event.h"


PUBLIC MotionPointer *motion_pointer_create(const unsigned short num) {
	MotionPointer *p = MALLOC(num * sizeof(MotionPointer));
	int i;
	for (i = 0; i < num; i++) {
		p[i].action = -1;
		p[i].left = 0;
		p[i].y = 0;
	}
	return p;
}

PUBLIC void motion_pointer_destroy(MotionPointer *p) {
	if (NULL != p) {
		FREE(p);
	}
	return;
}

PUBLIC MotionEvent *motion_event_create() {
	MotionEvent *event = MALLOC(sizeof(MotionEvent));
	linked_list_item_init(&event->linkedItem);
	event->pointers = NULL;
	return event;
}

PUBLIC void motion_event_destroy(void *_e) {
	MotionEvent *e = (MotionEvent *)_e;
	if (NULL != e) {
		motion_pointer_destroy(e->pointers);
		FREE(e);
	}
	return;
}

PUBLIC MotionEvent *motion_event_createArray(const unsigned short num) {
	MotionEvent *event = MALLOC(num * sizeof(MotionEvent));
	event->pointers = NULL;
	return event;
}

PUBLIC void motion_event_destroyArray(MotionEvent *e, unsigned short size) {
	if (NULL != e) {
		unsigned short i;
		for (i = 0; i < size; i++) {
			motion_pointer_destroy(e[i].pointers);
		}
		FREE(e);
	}
	return;
}
