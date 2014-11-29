/*
 * tmem.c
 *
 *  Created on: 2012-11-1
 *      Author: ottofung
 */
#include "mem.h"

#ifdef B_COMPILE_ENABLE_TMEM
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <pthread.h>

//#include <util.h>

struct _mem_node {
	char filename[128];
	unsigned int line;
	unsigned int sizeBytes;
	struct _mem_node *next;
	struct _mem_node *prev;
};

PRIVATE struct _mem_node *_tracking = NULL;					/* tracking linked list */
PRIVATE pthread_mutex_t _mutex = PTHREAD_MUTEX_INITIALIZER;	/* mutex for tracking list locking */
PRIVATE unsigned int _currentAllocatedBytes = 0;
PRIVATE unsigned int _maxAllocatedBytes = 0;
PRIVATE unsigned long long _malloc_count = 0;
PRIVATE unsigned long long _realloc_count = 0;
PRIVATE unsigned long long _free_count = 0;

PUBLIC B_MALLOC void *mem_malloc(size_t sizeToAllocate, const char *filename, unsigned int line) {
	struct _mem_node *node;
	node = (struct _mem_node *)malloc(sizeof(struct _mem_node) + sizeToAllocate);
	if (NULL == node) {
		LOGE("mem_alloc() failed: %s line %u size %u bytes", filename, line, sizeToAllocate);
		return NULL;
	}
	strcpy(node->filename, filename);
	node->line = line;
	node->sizeBytes = sizeToAllocate;
	/* locks the tracking list */
	pthread_mutex_lock(&_mutex);
	_malloc_count++;
	node->prev = NULL;
	node->next = _tracking;
	if (NULL != node->next) {
		node->next->prev = node;
	}
	_tracking = node;
	/* we only count the actual allocation, no need to count tracking overhead */
	/* since this represents the real usage if we turn off the tracking code at production */
	_currentAllocatedBytes += sizeToAllocate;
	if (_maxAllocatedBytes < _currentAllocatedBytes) {
		_maxAllocatedBytes = _currentAllocatedBytes;
	}
	pthread_mutex_unlock(&_mutex);
	/* done locking */
	return (void *)((unsigned char *)node + sizeof(struct _mem_node));
}

PUBLIC B_MALLOC void *mem_realloc(void *mem, size_t sizeToReallocate, const char *filename, unsigned int line) {
	struct _mem_node *node;
	if (NULL == mem) {
		return mem_malloc(sizeToReallocate, filename, line);
	}
	node = (struct _mem_node *)((unsigned char *)mem - sizeof(struct _mem_node));
	pthread_mutex_lock(&_mutex);
	_realloc_count++;
	/*
	 * this is locked since realloc() might change the node pointer
	 * (which other nodes in the tracking list points to)
	 */
	node = realloc(node, sizeof(struct _mem_node) + sizeToReallocate);
	if (NULL == node) {
		LOGE("mem_realloc() failed: %s line %u size %u bytes", filename, line, sizeToReallocate);
		pthread_mutex_unlock(&_mutex);
		return NULL;
	}
	_currentAllocatedBytes -= node->sizeBytes;
	node->sizeBytes = sizeToReallocate;
	_currentAllocatedBytes += sizeToReallocate;
	if (_maxAllocatedBytes < _currentAllocatedBytes) {
		_maxAllocatedBytes = _currentAllocatedBytes;
	}
	if (NULL != node->prev) {
		node->prev->next = node;
	} else {
		_tracking = node;
	}
	if (NULL != node->next) {
		node->next->prev = node;
	}
	pthread_mutex_unlock(&_mutex);
	/* we can do the following safely after unlock */
	strcpy(node->filename, filename);
	node->line = line;
	return (void *)((unsigned char *)node + sizeof(struct _mem_node));
}

PUBLIC void mem_free(void *mem, const char *filename, unsigned int line) {
	struct _mem_node *node;
	node = (struct _mem_node *)((unsigned char *)mem - sizeof(struct _mem_node));
	/* locks the tracking list */
	pthread_mutex_lock(&_mutex);
	_free_count++;
	_currentAllocatedBytes -= node->sizeBytes;
	/* removes it from the tracking list */
	if (NULL != node->prev) {
		node->prev->next = node->next;
	} else {
		/* prev is NULL, we are the head */
		_tracking = node->next;
	}
	if (NULL != node->next) {
		node->next->prev = node->prev;
	}
	pthread_mutex_unlock(&_mutex);
	/* done locking */
	free((void *)node);
	return;
}
#endif	/* COMPILE_ENABLE_TMEM */

PUBLIC void mem_dump(void) {
#ifdef B_COMPILE_ENABLE_TMEM
	struct _mem_node *node;
	LOGI("Outstanding allocations:");
	/* locks the tracking list */
	pthread_mutex_lock(&_mutex);
	node = _tracking;
	while (NULL != node) {
		LOGW("%s:%u %u bytes", node->filename, node->line, node->sizeBytes);
		node = node->next;
	}
	LOGI("Malloc: %llu, Realloc: %llu, Free: %llu", _malloc_count, _realloc_count, _free_count);
	LOGI("Current: %u bytes Max: %u bytes",	_currentAllocatedBytes, _maxAllocatedBytes);
	pthread_mutex_unlock(&_mutex);
	/* done locking */
#endif	/* COMPILE_ENABLE_TMEM */
	if (_malloc_count + _realloc_count == _free_count)
		LOGI("NORMAL DUMP!!");
	else
		LOGE("DUMP ERROR!!");
	return;
}

PUBLIC void mem_stats(unsigned int *current, unsigned int *max,
							unsigned long long *malloc_count,
							unsigned long long *realloc_count,
							unsigned long long *free_count) {
#ifdef B_COMPILE_ENABLE_TMEM
	/* locks the tracking list */
	pthread_mutex_lock(&_mutex);
	if (NULL != current) *current = _currentAllocatedBytes;
	if (NULL != max) *max = _maxAllocatedBytes;
	if (NULL != malloc_count) *malloc_count = _malloc_count;
	if (NULL != realloc_count) *realloc_count = _realloc_count;
	if (NULL != free_count) *free_count = _free_count;
	pthread_mutex_unlock(&_mutex);
	/* done locking */
#else	/* COMPILE_ENABLE_TMEM */
	if (NULL != current) *current = 0;
	if (NULL != max) *max = 0;
	if (NULL != malloc_count) *malloc_count = 0;
	if (NULL != realloc_count) *realloc_count = 0;
	if (NULL != free_count) *free_count = 0;
#endif	/* COMPILE_ENABLE_TMEM */
	return;
}

//PUBLIC void *mem_malloc_proc(int sizeToAllocate, const char *filename I_UNUSED, unsigned int line I_UNUSED) {
//	return mem_malloc(sizeToAllocate, filename, line);
//}
//
//PUBLIC void mem_free_proc(void *mem, const char *filename I_UNUSED, unsigned int line I_UNUSED) {
//	return mem_free(mem, filename, line);
//}
