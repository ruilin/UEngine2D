/*
 * tmem.h
 *
 *  Created on: 2012-11-1
 *      Author: ottofung
 */
#ifndef mem_H_
#define mem_H_

#include <base.h>

typedef void *(*mem_MALLOC_PROC)(int sizeToAllocate, const char *filename, unsigned int line);
typedef void (*mem_FREE_PROC)(void *mem, const char *filename, unsigned int line);

#ifdef B_COMPILE_ENABLE_TMEM
#include <stddef.h>
B_MALLOC void *mem_malloc(size_t sizeToAllocate, const char *filename, unsigned int line);
B_MALLOC void *mem_realloc(void *mem, size_t sizeToReallocate, const char *filename, unsigned int line);
void mem_free(void *mem, const char *filename, unsigned int line);
#else
#include <stdlib.h>
#define mem_malloc(x, f, l)		(malloc((x)))
#define mem_realloc(b, x, f, l)	(realloc((b), (x)))
#define mem_free(b, f, l)			(free((b)))
#endif	/* IMMERSION_COMPILE_ENABLE_TMEM */
void mem_dump(void);
void mem_stats(unsigned int *current, unsigned int *max,
				unsigned long long *malloc_count,
				unsigned long long *realloc_count,
				unsigned long long *free_count);
//void *mem_malloc_proc(int sizeToAllocate, const char *filename, unsigned int line);
//void mem_free_proc(void *mem, const char *filename, unsigned int line);

#endif /* mem_H_ */
