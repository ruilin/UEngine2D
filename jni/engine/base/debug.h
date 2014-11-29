/*
 * debug.h
 *
 *  Created on: 2013-10-31
 *      Author: ryan
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#include <base.h>


typedef struct {
	unsigned short fps;
} Debug;

Debug *debug_init();

#endif /* DEBUG_H_ */
