/*
 * gthread.c
 *	线程管理模块
 *  Created on: 2014-3-27
 *      Author: ryan
 */

#include "gthread.h"

typedef struct {
	pthread_mutex_t mut;
} Thread_Mng;

PRIVATE Thread_Mng mng;

PUBLIC void gthread_init() {
	pthread_mutex_init(&mng.mut,NULL);
	return;
}

PUBLIC void gthread_end() {

	return;
}

PUBLIC BOOL gthread_new(Gthread *gt, GTHREAD_RUN callbackfun, void *param) {
	int code;
	 if((code = pthread_create(&gt->thread, NULL, callbackfun, param)) != 0) {
		 LOGE("pthread_create() ERROR: %d", code);
		 return FALSE;
	 }
	return TRUE	;
}

PUBLIC BOOL gthread_wait(Gthread *gt) {
	if (NULL == gt) {
		return FALSE;
	}
	pthread_join(gt->thread, NULL);
	return TRUE;
}

PUBLIC BOOL gthread_exit() {
	pthread_exit(NULL);
	return;
}

PUBLIC void gthread_lock() {
	pthread_mutex_lock(&mng.mut);
	return;
}

PUBLIC void gthread_unlock() {
	pthread_mutex_unlock(&mng.mut);
	return;
}
