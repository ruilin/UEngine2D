/*
 * file_op.c
 *
 *  Created on: 2014-3-28
 *      Author: ryan
 */

#include "file_op.h"

#define S_DEF_MOD	0770
#define ENGINE_DIR	"ryangame"

PRIVATE unsigned char _engine_path[128];

PUBLIC void file_op_init(const char *sdcardPath) {
	sprintf(_engine_path, "%s/%s", sdcardPath, ENGINE_DIR);
	struct stat sb;
	if (0 != stat(_engine_path, &sb)) {
		/* 引擎目录不存在则创建它 */
		if (0 != mkdir(_engine_path, S_DEF_MOD)) {
			LOGE("file_op_init() mkdir() ERROR: %s", _engine_path);
		}
	}
	return;
}

PUBLIC unsigned char *file_op_getPath() {
	return _engine_path;
}

PUBLIC void file_op_test() {
	char *filename = "test.txt";
	char filePath[128];
	sprintf(filePath, "%s/%s", _engine_path, filename);
	mkdir(_engine_path, S_DEF_MOD);								/* make folder */

	struct stat sb;
	int32_t res = stat(filePath, &sb);
	if (0 != res && sb.st_mode & S_DEF_MOD) {	/* check if the file already exists */
		FILE *fp;
		fp = fopen(filePath, "w+");
		if (NULL != fp) {
			fputs("hello world", fp);
			fclose(fp);
		}
	}
	return;
}

PUBLIC BOOL file_op_isExists(const char *directory) {
	unsigned char path[128];
	sprintf(path, "%s/%s", _engine_path, directory);
	struct stat sb;
	int32_t res = stat(path, &sb);
	return (0 == res);
}

PUBLIC BOOL file_op_mkdir(const char *dirpath) {
	char path[128];
	sprintf(path, "%s/%s", _engine_path, dirpath);
	int result;
	if (0 != (result = mkdir(path, S_DEF_MOD))) {
		LOGE("file_op_mkdir() ERROR code: %d, %s", result, path);
	}
	return TRUE;
}

PUBLIC BOOL file_op_remove(const char *dirpath) {
	char path[128];
	sprintf(path, "%s/%s", _engine_path, dirpath);
	int result;
	if (0 != (result = remove(path))) {
		LOGE("file_op_remove() ERROR code: %d, %s", result, path);
	}
	return TRUE;
}

PUBLIC BOOL file_op_writeFile(const char *filename, unsigned char *content, unsigned short size) {
	char filePath[128];
	sprintf(filePath, "%s/%s", _engine_path, filename);
	FILE *fp;
	fp = fopen(filePath, "w");
	if (NULL == fp) {
		LOGE("file_op_writeFile() ERROR");
		return FALSE;
	}
	fwrite(content, sizeof(unsigned char), size, fp);
	fclose(fp);
	return TRUE;
}

/* 读取文本文件 */
PUBLIC unsigned char *file_op_readFile(unsigned char *filename, unsigned int *_size) {
	unsigned char *text;
	char filePath[128];
	sprintf(filePath, "%s/%s", _engine_path, filename);
	FILE *fp = fopen(filePath, "r");
	if (NULL == fp) {
		LOGE("ERROR: file_op_textFileRead() cannot open file: %s", filePath);
		return NULL;
	}
	fseek(fp, 0, SEEK_END);
	unsigned int size = ftell(fp);
	text = (unsigned char *)MALLOC(size);
	rewind(fp);
	fread(text, sizeof(char), size, fp);
	fclose(fp);
	if (NULL != _size) *_size = size;
	return text;
}

