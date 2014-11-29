/*
 * gthread.h
 *
 *  Created on: 2014-3-27
 *      Author: ryan
 */

#ifndef GTHREAD_H_
#define GTHREAD_H_

#include <base.h>
#include <pthread.h>

typedef struct {
	pthread_t thread;
} Gthread;

typedef void *(*GTHREAD_RUN) (void *param);

void gthread_init();
void gthread_end();
BOOL gthread_new(Gthread *gt, GTHREAD_RUN callbackfun, void *param);
BOOL gthread_wait();
void gthread_lock();
void gthread_unlock();

#endif /* GTHREAD_H_ */
