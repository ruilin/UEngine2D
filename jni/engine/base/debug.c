/*
 * debug.c
 *
 *  Created on: 2013-10-31
 *      Author: ryan
 */

#include "debug.h"

PRIVATE Debug debug;

PUBLIC Debug *debug_init() {
	debug.fps = 0;
	return &debug;
}
