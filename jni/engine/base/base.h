/*
 * base.h
 *
 *  Created on: 2012/10/18
 *      Author: ottofung
 */
#ifndef BASE_H_
#define BASE_H_

/* defines any exported calls with I_PUBLIC */
#define PUBLIC					__attribute__((externally_visible))
/* defines any static calls with I_PRIVATE */
#define PRIVATE					static
/* defines no return type of function (gcc attribute) */
#define NORETURN					__attribute__((__noreturn__))

/* gcc attribute */
#define B_MALLOC					__attribute__((malloc))
#define UNUSED					__attribute__((unused))

#define B_COMPILE_ENABLE_TMEM
#include "util/mem.h"
#define MALLOC(x)					(mem_malloc((x), __FILE__, __LINE__))
#define REALLOC(b, x)				(mem_realloc((b), (x), __FILE__, __LINE__))
#define FREE(b)						(mem_free((b), __FILE__, __LINE__))


/* provides clues to the compiler to better optimize branch prediction, also minimizes instruction pipeline stalls */
#define I_LIKELY(x)       			__builtin_expect(!!(x), 1)
#define I_UNLIKELY(x)     			__builtin_expect(!!(x), 0)

#include <android/log.h>

#define LOG_TEG "TEST"
#define LOG_TEG_ERR "ERROR"
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, LOG_TEG, __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, LOG_TEG, __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, LOG_TEG, __VA_ARGS__))
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TEG_ERR, __VA_ARGS__))

typedef enum {
	FALSE							= 0,
	TRUE							= 1
} BOOL;

typedef enum {
	ENDIAN_BIG 						= 0,
	ENDIAN_LITTLE					= 1
} ENDIAN;

#endif /* BASE_H_ */
