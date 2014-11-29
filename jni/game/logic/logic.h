/*
 * logic.h
 *
 *  Created on: 2013-9-3
 *      Author: Administrator
 */

#ifndef LOGIC_H_
#define LOGIC_H_

#include <base.h>
#include <engine.h>
#include <jni_lib.h>

#include <resource/sprite_data.h>
#include <render2d/graphic.h>
#include "../game.h"
#include "view_debug.h"

BOOL logic_init(Engine *engine);
BOOL logic_run(Engine *engine);
BOOL logic_end(Engine *engine);

#endif /* LOGIC_H_ */
