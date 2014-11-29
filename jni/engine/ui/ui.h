/*
 * ui.h
 *
 *  Created on: 2013-10-23
 *      Author: ryan
 */

#ifndef UI_H_
#define UI_H_

#include <base.h>
#include <base/debug.h>
#include <util/linked_list.h>
#include "view.h"


void ui_init(unsigned short screenWidth, unsigned short screenHeight,
					unsigned short canvasWidth, unsigned short canvasHeight);
void ui_end();
void ui_setSystemMsg(Debug *_sys);
Debug *ui_getSystemMsg();
BOOL ui_run(MotionEvent *event);
BOOL ui_render(Graphic *g);
void ui_addView(View *view);
void ui_removeView(View *view);

#endif /* UI_H_ */
