/*
 * time_util.c
 *
 *  Created on: 2013-9-9
 *      Author: Administrator
 */


#include "time_util.h"

PUBLIC double time_util_now_ms(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (double)(tv.tv_sec*1000.0 + tv.tv_usec/1000.0);
}
