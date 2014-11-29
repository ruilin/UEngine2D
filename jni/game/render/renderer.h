/*
 * renderer.h
 *
 *  Created on: 2013-9-2
 *      Author: Ryan
 */

#ifndef RENDERER_H_
#define RENDERER_H_

#include <GLES/gl.h>
#include <GLES/glext.h>

#include <base.h>
#include <util/linked_list.h>
#include <engine.h>

#include <render2d/canvas.h>
#include <render2d/graphic.h>
#include <logic.h>
#include <resource/sprite_data.h>
#include <util/data/rect.h>


void renderer_init(Engine *engine);
void renderer_end(Engine *engine);
void renderer_draw(Engine *engine, Graphic *g);
void renderer_drawLoading(Engine *engine, Graphic *g);

#endif /* RENDERER_H_ */
