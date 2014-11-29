/*
 * res.c
 *	资源管理器
 *  Created on: 2014-3-14
 *      Author: ryan
 */

#include "res.h"

typedef struct {
	void *hash_img;
} Res;

PRIVATE Res res;

/* 获取图片资源 */
PUBLIC Texture *res_newPngPOT(const char *filePath, IMG_QUALITY quality) {
	Texture *tex = NULL;
	/* check whether it is already in memory */
	if (FALSE == hash_get(res.hash_img, filePath, strlen(filePath), (unsigned int *)&tex, NULL)) {
		tex = image_createPngPOT(filePath, quality);
		hash_set(res.hash_img, filePath, strlen(filePath), (unsigned int)tex, (unsigned int)NULL);
	} else {
		tex->usedCount++;
	}
	return tex;
}

PUBLIC void res_releasePng(Texture *img) {
	if (TRUE == hash_get(res.hash_img, img->filePath, strlen(img->filePath), NULL, NULL)) {
		img->usedCount--;
		if (img->usedCount <= 0) {
			hash_unset(res.hash_img, img->filePath, strlen(img->filePath));
			image_destroy(img);
		}
	} else {
		image_destroy(img);
	}
	return;
}

PRIVATE void *_res_cpyScript(void *data) {
	const char *path = (const char *)data;

	jni_lib_cpy_allFileToSdcard("script");
	engine_setLoadingRatio(0.2);
	jni_lib_cpy_allFileToSdcard("script/ini");
	engine_setLoadingRatio(0.2);
	jni_lib_cpy_allFileToSdcard("script/map");
	engine_setLoadingRatio(0.2);
	jni_lib_cpy_allFileToSdcard("script/act");
	engine_setLoadingRatio(0.2);
	jni_lib_cpy_allFileToSdcard("script/eff");
	engine_setLoadingRatio(0.2);
	jni_lib_cpy_allFileToSdcard("script/story");
	/* 拷贝完成，切换状态进入游戏 */
	engine_setLoading(FALSE, 1.0);
	return NULL;
}

PUBLIC void res_init() {
	res.hash_img = hash_init(10);	/* hashmap size 2^10 = 1024 */

	/* 拷贝脚本文件到SDcard */
//	engine_setLoading(TRUE, 1.0f);
//	Gthread gt;
//	gthread_new(&gt, _res_cpyScript, "script");
	return;
}

PUBLIC void res_end() {
	hash_destroy(res.hash_img);
	return;
}
