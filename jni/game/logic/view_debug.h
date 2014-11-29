/*
 * view_debug.h
 *
 *  Created on: 2013-10-31
 *      Author: ryan
 */

#ifndef VIEW_DEBUG_H_
#define VIEW_DEBUG_H_

#include <base.h>
#include <ui/ui.h>
#include <ui/view.h>
#include <ui/button.h>
#include <ui/motion_event.h>
#include <render2d/canvas.h>
#include <render2d/graphic.h>
#include "../game.h"

typedef struct {
	View *view;
} View_SysMsg;


View_SysMsg *view_debug_create();
void view_debug_destroy(View_SysMsg *sysMsg);

#endif /* VIEW_DEBUG_H_ */
