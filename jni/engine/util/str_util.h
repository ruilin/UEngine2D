/*
 * str_util.h
 * 字符串处理工具
 *  Created on: 2014-4-3
 *      Author: ryan
 */

#ifndef STR_UTIL_H_
#define STR_UTIL_H_

#include <base.h>
#include <string.h>

/* 判断字符是否为中文 */
#define str_util_isCN(ch) (0x80 <= (ch))

unsigned char **str_util_section(const char *str, unsigned char frontSize, unsigned short lineWidth, unsigned short *sectionSize);
void str_util_destroySection(unsigned char **sections, unsigned short size);

#endif /* STR_UTIL_H_ */
