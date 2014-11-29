/*
 * runnable.h
 *
 *  Created on: 2013-10-26
 *      Author: ryan
 */

#ifndef RUNNABLE_H_
#define RUNNABLE_H_

#include <base.h>
#include <base/sysinfo.h>
#include <engine.h>
#include "thread/gthread.h"
#include <io/file_op.h>

BOOL runnable_init(Engine *platform);
BOOL runnable_end(Engine *platform);
BOOL runnable_run(Engine *platform);

#endif /* RUNNABLE_H_ */
