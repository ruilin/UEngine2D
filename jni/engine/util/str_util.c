/*
 * str_util.c
 *
 *  Created on: 2014-4-3
 *      Author: ryan
 */

#include "str_util.h"

/*
 * 字符串分段
 * 按字符宽度和行宽计算每段的字数
 * 返回 二维数组
 */
PUBLIC unsigned char **str_util_section(const char *str, unsigned char frontSize, unsigned short lineWidth, unsigned short *sectionSize) {
	unsigned short strLen = strlen(str);
	unsigned char frontWidth = frontSize >> 1;	/* 英文字符宽度 == 字符高度的一半 */
	unsigned char frontWidth_cn = frontSize;		/* 中文字符宽度 == 字符高度 */
	unsigned short lineCharCount = lineWidth / frontWidth;
	unsigned short lineCount = strLen / lineCharCount + 1;	/* +1: 结束标志\0 */
	unsigned char **sections = MALLOC(sizeof(unsigned char *) * lineCount);

	unsigned short sectionWidth = 0;

	unsigned short lineIndex = 0;
	unsigned short start = 0;										/* 起始标记 */
	unsigned short end = 0;										/* 结束标记 */
	while (end < strLen) {
		if (str_util_isCN(str[end])) {								/* 判断字符是否为中文 */
			end += 3;														/* utf-8编码 一个中文字符占3个字节 */
			sectionWidth += frontWidth_cn;
		} else {
			end += 1;
			sectionWidth += frontWidth;
		}
		if (sectionWidth >= lineWidth || end == strLen) {
			unsigned short lineSize = end - start;
			sections[lineIndex] = MALLOC(sizeof(unsigned char) * (lineSize + 1));	/* 加一个字节结束符 */
			memcpy(sections[lineIndex], str + start, lineSize);
			sections[lineIndex][lineSize] = '\0';
			sectionWidth = 0;
			start = end;
			lineIndex++;
		}
	}
	if (NULL != sectionSize) *sectionSize = lineIndex;
	return sections;
}

/*
 * 释放字符串分段的二进制数组
 */
PUBLIC void str_util_destroySection(unsigned char **sections, unsigned short size) {
	if (NULL != sections) {
		int i;
		for (i = 0; i < size; i++) {
			FREE(sections[i]);
		}
		FREE(sections);
	}
	return;
}
