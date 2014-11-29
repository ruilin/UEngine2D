/*
 * hash.h
 *
 *  Created on: 2012-10-26
 *      Author: ottofung
 */
#ifndef HASH_H_
#define HASH_H_

#include <base.h>
#include "crc32.h"

typedef BOOL (*HASH_ENUM_PROC)(const void *key, unsigned int key_len, unsigned int value_1, unsigned int value_2, void *param);

void *hash_init(unsigned char hashSizing);
BOOL hash_set(void *hash, const void *key, unsigned int key_len, unsigned int value_1, unsigned int value_2);
BOOL hash_get(void *hash, const void *key, unsigned int key_len, unsigned int *value_1, unsigned int *value_2);
BOOL hash_unset(void *hash, const void *key, unsigned int key_len);
BOOL hash_enum(void *hash, HASH_ENUM_PROC proc, void *param);
void hash_clear(void *hash);
void hash_destroy(void *hash);
void hash_stats_dump(void *hash, char *name);

#endif /* HASH_H_ */
