/*
 * res.h
 *
 *  Created on: 2014-3-14
 *      Author: ryan
 */

#ifndef RES_H_
#define RES_H_

#include <base.h>
#include <engine.h>
#include <util/hashmap/hash.h>
#include <gthread.h>
#include "image.h"
#include "map_data.h"
#include "sprite_data.h"

void res_init();
void res_end();
Texture *res_newPngPOT(const char *filePath, IMG_QUALITY quality);
void res_releasePng(Texture *img);

#endif /* RES_H_ */
