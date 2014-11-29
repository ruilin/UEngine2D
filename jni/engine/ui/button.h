/*
 * button.h
 *
 *  Created on: 2013-10-27
 *      Author: ryan
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include <base.h>
#include <ui/view.h>
#include <render2d/canvas.h>
#include <render2d/graphic.h>
#include <ui/motion_event.h>

typedef enum {
	BUTTON_STATE_NOTHING		= 0,
	BUTTON_STATE_CLICK_DOWN		= 1,
	BUTTON_STATE_CLICK_UP		= 2,
} BUTTON_STATE;

typedef BOOL (*BUTTON_CALLBACK_TRIGGER)(void *instance, void *super, BUTTON_STATE state);
typedef BOOL (*BUTTON_CALLBACK_RENDER)(void *instance, void *super, Graphic *g);

typedef struct {
	View *view;
	BUTTON_CALLBACK_TRIGGER callback_onClick;
	BUTTON_CALLBACK_RENDER callback_onDraw;
	BUTTON_STATE state;
} Button;

Button *button_create(Rect rect);
void button_destroy(Button *button);
BOOL button_onClick(void *_button, void *super, MOTION_ACTION action, int x, int y);
BOOL button_onDraw(void *_button, void *super, Graphic *g);
void button_setListener(Button *button,
					BUTTON_CALLBACK_TRIGGER callback_trigger,
					BUTTON_CALLBACK_RENDER callback_render);
#endif /* BUTTON_H_ */
