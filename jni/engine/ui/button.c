/*
 * button.c
 *
 *  Created on: 2013-10-27
 *      Author: ryan
 */

#include "button.h"

PUBLIC BOOL button_onClick(void *_button, void *super, MOTION_ACTION action, int x, int y) {
	Button *button = (Button *)_button;
	switch (button->state) {
	case BUTTON_STATE_NOTHING:
	case BUTTON_STATE_CLICK_UP:{
		switch (action) {
		case ACTION_DOWN:
		case ACTION_POINTER_1_DOWN:
		case ACTION_POINTER_2_DOWN:
		case ACTION_POINTER_3_DOWN:
			if (TRUE == rect_isInside(&button->view->rect, x, y)) {
				button->state = BUTTON_STATE_CLICK_DOWN;
				button->callback_onClick(button, super, button->state);
				return TRUE;
			}
			break;
		case ACTION_MOVE:
		case ACTION_UP:
		default:
			break;
		}
	}break;
	case BUTTON_STATE_CLICK_DOWN:{
		switch (action) {
		case ACTION_MOVE:
			if (TRUE == rect_isInside(&button->view->rect, x, y)) {
				return TRUE;
			}
		case ACTION_UP:
		case ACTION_POINTER_1_UP:
		case ACTION_POINTER_2_UP:
		case ACTION_POINTER_3_UP:
			button->state = BUTTON_STATE_CLICK_UP;
			button->callback_onClick(button, super, button->state);
			button->state = BUTTON_STATE_NOTHING;
			return TRUE;
		default:
			break;
		}
	}break;
	}

	return FALSE;
}

PUBLIC BOOL button_onDraw(void *_button, void *super, Graphic *g) {
	Button *button = (Button *)_button;
	if (NULL != button->callback_onDraw) {
		button->callback_onDraw(button, super, g);
		return TRUE;
	}
	return FALSE;
}

PUBLIC BOOL button_layout(void *_button, void *super, Rect rect) {
	Button *button = (Button *)_button;
	return TRUE;
}

PUBLIC Button *button_create(Rect rect) {
	Button *button = MALLOC(sizeof(Button));
	button->view = view_createByRect("button", rect);
	view_extend(button->view, button, button_onClick, button_onDraw, button_layout);
	button->state = BUTTON_STATE_NOTHING;
	return button;
}

PUBLIC void button_destroy(Button *button) {
	if (NULL != button) {
		view_destroy(button->view);
		FREE(button);
	}
	return;
}

PUBLIC void button_setListener(Button *button,
								BUTTON_CALLBACK_TRIGGER callback_onClick,
								BUTTON_CALLBACK_RENDER callback_onDraw) {
	button->callback_onClick = callback_onClick;
	button->callback_onDraw = callback_onDraw;
	return;
}

