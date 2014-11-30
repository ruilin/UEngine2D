/*
 * renderer.c
 *
 *  Created on: 2013-9-2
 *      Author: Ryan
 */

#include "renderer.h"

typedef struct Renderer {
	Rect rect;
} Renderer;

PRIVATE Renderer r;

PUBLIC void renderer_init(Engine *engine) {
	rect_set(&r.rect, 0, 0, engine->canvasWidth, engine->canvasHeight);
	canvas_init(engine->screenWidth, engine->screenHeight, engine->canvasWidth, engine->canvasHeight);
	return;
}

PUBLIC void renderer_end(Engine *engine) {
	return;
}

PUBLIC void renderer_draw(Engine *engine, Graphic *g) {
	if (TRUE == engine->isLoading) {
		return;
	}
    graphic_setColor4f(g, 1.0f, 1.0f, 1.0f, 1.0f);
    canvas_fillRGB(1.0f, 1.0f, 1.0f);
    graphic_setTextColor(g, 255, 255, 255, 255);
    graphic_setTextStyle(g, 30, TEXT_STYLE_FILL_AND_STROKE);
    canvas_draw_string("Hello UEngine!!", g, 400, 220, ALIGN_CENTER);
    return;
}

PUBLIC void renderer_drawLoading(Engine *engine, Graphic *g) {
	char str[128];
	sprintf(str, "Loading...%d%%", (int)(engine->loadingRatio * 100));
	canvas_fillRGB(0.0f, 0.0f, 0.0f);
	graphic_setTextColor(g, 255, 255, 255, 255);
	graphic_setTextStyle(g, 25, TEXT_STYLE_FILL);
	canvas_draw_string("正在初始化游戏", g, 400, 300, ALIGN_CENTER_HORIZONTAL);
	graphic_setTextStyle(g, 20, TEXT_STYLE_FILL);
	canvas_draw_string(str, g, 320, 350, ALIGN_LEFT);
	return;
}

/*
 * 绘制画面切换效果
 */
PUBLIC void renderer_drawTransform(Engine *engine, Graphic *g) {
	Rect rect;
	rect_set(&rect, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	canvas_fill_Rect(g, &rect);
	return;
}

