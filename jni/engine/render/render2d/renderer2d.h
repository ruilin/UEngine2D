/*
 * renderer2d.h
 *
 *  Created on: 2013-10-26
 *      Author: ryan
 */

#ifndef RENDERER2D_H_
#define RENDERER2D_H_

#include <base.h>
#include <engine.h>

#include <game/logic/logic.h>
#include <game/render/renderer.h>
#include "graphic.h"
#include "text.h"
#include "ui/ui.h"

void renderer2d_init(Engine *engine);
void renderer2d_end(Engine *engine);
void renderer2d_draw(Engine *engine);

#endif /* RENDERER2D_H_ */
