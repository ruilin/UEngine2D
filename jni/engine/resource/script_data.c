/*
 * script_data.c
 *
 *  Created on: 2014-3-24
 *      Author: ryan
 */

#include "script_data.h"

#define CHUNK_SIZE	16
PRIVATE unsigned char AES_KEY[32] = "ryanada";	/* 秘钥 */

PUBLIC ScriptData *script_data_create() {
	ScriptData *script = MALLOC(sizeof(ScriptData));
	if (NULL == script) {
		LOGE("script_data_create() ERROR: malloc fail!!");
		return NULL;
	}
	aes256_init(&script->ctx, AES_KEY);
	return script;
}

PUBLIC void script_data_destroy(ScriptData *script) {
	aes256_done(&script->ctx);
	FREE(script);
	return;
}

/*
 * aes解密脚本
 */
PUBLIC unsigned char *script_data_decript(ScriptData *script, const unsigned char *data) {
	int i, index = 0;
	unsigned short chunkCount;
	memcpy(&chunkCount,  &data[index], 2);
//	index += sizeof(unsigned short);
	unsigned short size = chunkCount << 4;
	unsigned char *result = MALLOC(size);
	memcpy(result, &data[2], size);
	for (i = 0; i < chunkCount; i++) {
		aes256_decrypt_ecb(&script->ctx, &result[index]);
		index += (CHUNK_SIZE);
	}
	return result;
}

