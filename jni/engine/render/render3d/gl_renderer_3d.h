/*
 * gl_lib.h
 *
 *  Created on: 2013-8-30
 *      Author: Administrator
 */

#ifndef GL_LIB_H_
#define GL_LIB_H_

#include <android/log.h>

#include <GLES/gl.h>
#include <GLES/glext.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <sys/time.h>
#include <base.h>

void renderer3d_init(int width, int height);
void renderer3d_draw();

#endif /* GL_LIB_H_ */
