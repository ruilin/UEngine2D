/*
 * text.h
 *
 *  Created on: 2013-10-31
 *      Author: ryan
 */

#ifndef TEXT_H_
#define TEXT_H_

#include <string.h>
#include <base.h>
#include <util/hashmap/hash.h>
#include <resource/image.h>

void text_init();
void text_end();
void text_update();
Texture *text_getTextImage(const char *str, int fontsize, int strokeWidth, int paintStyle,
										int r, int g, int b, int a,
										int strokeR, int strokeG, int strokeB, int strokeA,
										int *textPixelSize);

#endif /* TEXT_H_ */
