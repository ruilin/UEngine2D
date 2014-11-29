/*
 * file_op.h
 *
 *  Created on: 2014-3-28
 *      Author: ryan
 */

#ifndef FILE_OP_H_
#define FILE_OP_H_

#include <stdio.h>
#include <sys/stat.h>
#include <base.h>

void file_op_init(const char *sdcardPath);
unsigned char *file_op_getPath();
void file_op_test();
BOOL file_op_mkdir(const char *dirpath);
BOOL file_op_remove(const char *dirpath);
BOOL file_op_writeFile(const char *filename, unsigned char *content, unsigned short size);
unsigned char *file_op_readFile(unsigned char *filename, unsigned int *_size);
BOOL file_op_isExists(const char *directory);

#endif /* FILE_OP_H_ */
