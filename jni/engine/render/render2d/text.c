/*
 * text.c
 *	字符纹理
 *  Created on: 2013-10-31
 *      Author: ryan
 */

#include "text.h"

typedef struct {
	void *hash_tex;
} Text_Manager;

typedef struct {
	Texture *img;
	int textPixelSize;
	BOOL isUsed;
} Text;

PRIVATE Text_Manager textMng;
PRIVATE unsigned short textCount;

PRIVATE Text *_text_create(const char *str, int fontSize, int strokeWidth, int paintStyle,
							int r, int g, int b, int a, int strokeR, int strokeG, int strokeB, int strokeA) {
	Text *text = MALLOC(sizeof(Text));
	text->textPixelSize = 0;
	text->isUsed = TRUE;
	text->img = image_crateTextPngPOT(str, fontSize, strokeWidth, paintStyle, r, g, b, a, strokeR, strokeG, strokeB, strokeA, &(text->textPixelSize));
	textCount++;
	return text;
}

PRIVATE void *_text_destroy(Text *text) {
	if (NULL != text) {
		image_destroy(text->img);
		FREE(text);
		textCount--;
	}
	return;
}

PUBLIC void text_init() {
	textMng.hash_tex = hash_init(9);	//size 2^9 = 512
	textCount = 0;
	return;
}

PRIVATE BOOL _hash_tex_enum_clean(const void *key, unsigned int key_len,
							unsigned int value1, unsigned int value2,
							void *param) {
	Text *text = (Text *)value1;
	_text_destroy(text);
	text = NULL;
	return TRUE;
}

PUBLIC void text_end() {
	hash_enum(textMng.hash_tex, _hash_tex_enum_clean, NULL);
	hash_destroy(textMng.hash_tex);
	return;
}

PRIVATE BOOL _hash_tex_enum_update(const void *key, unsigned int key_len,
							unsigned int value1, unsigned int value2,
							void *param) {
	Text *text = (Text *)value1;
	char *_key = (char *)key;
	if (FALSE == text->isUsed) {
		_text_destroy(text);
		hash_unset(textMng.hash_tex, key, key_len);
		text = NULL;
	} else {
		text->isUsed = FALSE;
	}
	return TRUE;
}

PUBLIC void text_update() {
	if (textCount > 10)
		hash_enum(textMng.hash_tex, _hash_tex_enum_update, NULL);
	return;
}

PUBLIC Texture *text_getTextImage(const char *str, int fontsize, int strokeWidth, int paintStyle,
									int r, int g, int b, int a,
									int strokeR, int strokeG, int strokeB, int strokeA,
									int *textPixelSize) {
	char key[strlen(str) + 128];
	sprintf(key, "%s/%d/%d/%d/%d/%d/%d/%d/%d/%d/%d/%d",str, fontsize, strokeWidth, paintStyle, r, g, b, a, strokeR, strokeG, strokeB, strokeA);
	Text *text = NULL;
	if (FALSE == hash_get(textMng.hash_tex, key, strlen(key), (unsigned int *)&text, NULL)) {
		text = _text_create(str, fontsize, strokeWidth, paintStyle, r, g, b, a, strokeR, strokeG, strokeB, strokeA);
		hash_set(textMng.hash_tex, key, strlen(key), (unsigned int)text, (unsigned int)NULL);
	}
	text->isUsed = TRUE;
	*textPixelSize = text->textPixelSize;
	return text->img;
}


