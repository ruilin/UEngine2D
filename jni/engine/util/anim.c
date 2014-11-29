/*
 * anim.c
 *
 *  Created on: 2014-4-8
 *      Author: ryan
 */

#include "anim.h"

typedef struct {
	Linked_Item linked;
	unsigned long long delayTime;
	float sxRatio;
	float syRatio;
	float exRatio;
	float eyRatio;
	float potW;
	float potH;
} Anim_Frame;

typedef struct {
	Texture *tex;
	void *lnk_frames;
	Anim_Frame *curFrame;
	unsigned long long sTime;
	Rect rect;
} Anim;

PRIVATE Anim_Frame *_anim_frame_create(unsigned long long delayTime, float sxRatio, float syRatio,
																		float exRatio, float eyRatio, float potW, float potH) {
	Anim_Frame *frame = MALLOC(sizeof(Anim_Frame));
	if (NULL == frame) {
		LOGE("_anim_frame_new() malloc fail");
		return NULL;
	}
	frame->delayTime = delayTime;
	frame->sxRatio = sxRatio;
	frame->syRatio = syRatio;
	frame->exRatio = exRatio;
	frame->eyRatio = eyRatio;
	frame->potW = potW;
	frame->potH = potH;
	return frame;
}

PRIVATE void _anim_frame_destroy(void *frame) {
	FREE(frame);
	return;
}

PUBLIC void *anim_create(unsigned char *filepath, IMG_QUALITY quality) {
	Anim *ani = MALLOC(sizeof(Anim));
	if (NULL == ani) {
		LOGE("anim_create() malloc fail: NULL == ani");
		return NULL;
	}
	ani->tex = res_newPngPOT(filepath, quality);
	rect_set(&ani->rect, 0, 0, ani->tex->widthPOT, ani->tex->heightPOT);
	ani->lnk_frames = linked_list_create(_anim_frame_destroy);
	ani->curFrame = NULL;
	ani->sTime = 0;
	return ani;
}

PUBLIC void anim_destroy(void *anim) {
	Anim *ani = (Anim *)anim;
	linked_list_destroy(ani->lnk_frames);
	res_releasePng(ani->tex);
	FREE(ani);
	return;
}

PUBLIC Rect *anim_getRect(void *anim) {
	Anim *ani = (Anim *)anim;
	return &ani->rect;
}

PUBLIC void anim_addFrame(void *anim, unsigned long long delayTime, int sx, int sy, int potW, int potH) {
	Anim *ani = (Anim *)anim;
	Anim_Frame *frame = _anim_frame_create(delayTime,
																		((float)sx / ani->tex->widthPOT),
																		((float)sy / ani->tex->heightPOT),
																		((float)(sx + potW) / ani->tex->widthPOT),
																		((float)(sy + potH) / ani->tex->heightPOT),
																		potW, potH);
	linked_list_addItem(ani->lnk_frames, &frame->linked, frame);
	if (NULL == ani->curFrame) ani->curFrame = frame;
	return;
}

PUBLIC BOOL anim_drawNextFrame(void *anim, Graphic *g, unsigned long long gametime, int x, int y) {
	Anim *ani = (Anim *)anim;
	if (NULL == ani->curFrame) {
		return FALSE;
	}
	canvas_draw_bitmapClipRatio(ani->tex, g, x, y, ani->curFrame->potW, ani->curFrame->potH,
													ani->curFrame->sxRatio, ani->curFrame->exRatio,
													ani->curFrame->syRatio, ani->curFrame->eyRatio);
	if (gametime - ani->sTime >= ani->curFrame->delayTime) {
		ani->sTime = gametime;
		ani->curFrame = linked_list_getNext(&ani->curFrame->linked);
		if (NULL == ani->curFrame) {
			ani->curFrame = linked_list_getHead(ani->lnk_frames);
		}
	}
	return TRUE;
}

