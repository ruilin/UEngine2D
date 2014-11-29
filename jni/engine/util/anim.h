/*
 * anim.h
 * simple animation
 *  Created on: 2014-4-8
 *      Author: ryan
 */

#ifndef ANIM_H_
#define ANIM_H_

#include <base.h>
#include <res.h>
#include <graphic.h>
#include <canvas.h>
#include <linked_list.h>
#include <rect.h>

void *anim_create(unsigned char *filepath, IMG_QUALITY quality);
void anim_destroy(void *anim);
Rect *anim_getRect(void *anim);
void anim_addFrame(void *anim, unsigned long long delayTime, int sx, int sy, int potW, int potH);
BOOL anim_drawNextFrame(void *anim, Graphic *g, unsigned long long gametime, int x, int y);

#endif /* ANIM_H_ */
