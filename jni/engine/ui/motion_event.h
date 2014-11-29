/*
 * motion_event.h
 *
 *  Created on: 2013-9-28
 *      Author: Ryan
 */

#ifndef MOTION_EVENT_H_
#define MOTION_EVENT_H_

#include <base.h>
#include <util/linked_list.h>

typedef enum {
	ACTION_MASK = 255,
	ACTION_DOWN = 0,
	ACTION_UP = 1,
	ACTION_MOVE = 2,
	ACTION_CANCEL = 3,
	ACTION_OUTSIDE = 4,
	ACTION_POINTER_DOWN = 5,
	ACTION_POINTER_UP = 6,
	ACTION_POINTER_INDEX_MASK = 65280,
	ACTION_POINTER_INDEX_SHIFT = 8,
	ACTION_POINTER_1_DOWN = 5,
	ACTION_POINTER_2_DOWN = 261,
	ACTION_POINTER_3_DOWN = 517,
	ACTION_POINTER_1_UP = 6,
	ACTION_POINTER_2_UP = 262,
	ACTION_POINTER_3_UP = 518,
	ACTION_POINTER_ID_MASK = 65280,
	ACTION_POINTER_ID_SHIFT = 8,
	FLAG_WINDOW_IS_OBSCURED = 1,
	EDGE_TOP = 1,
	EDGE_BOTTOM = 2,
	EDGE_LEFT = 4,
	EDGE_RIGHT = 8
} MOTION_ACTION;

typedef struct {
	MOTION_ACTION action;
	int left;
	int y;
} MotionPointer;

typedef struct {
	Linked_Item linkedItem;
	MotionPointer *pointers;	/* pointers of each event */
	unsigned short pointerSize;
} MotionEvent;

MotionPointer *motion_pointer_create(const unsigned short num);
void motion_pointer_destroy(MotionPointer *p);
MotionEvent *motion_event_create();
void motion_event_destroy(void *_e);
MotionEvent *motion_event_createArray(const unsigned short num);
void motion_event_destroyArray(MotionEvent *e, unsigned short size);

#endif /* MOTION_EVENT_H_ */
