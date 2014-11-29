/*
 * renderer2d.c
 *
 *  Created on: 2013-10-26
 *      Author: ryan
 */

#include "renderer2d.h"


PUBLIC void renderer2d_init(Engine *engine) {
	text_init();
	renderer_init(engine);
	return;
}

PUBLIC void renderer2d_end(Engine *engine) {
	renderer_end(engine);
	text_end();
	return;
}

PUBLIC void renderer2d_draw(Engine *engine) {
	text_update();
	if (FALSE == engine->isLoading) {
		renderer_draw(engine, engine->g);
		ui_render(engine->g);
		if (TRUE == engine->ifDrawTransform) {
			renderer_drawTransform(engine, engine->g);
		}
	} else {
		renderer_drawLoading(engine, engine->g);
	}
	return;
}
