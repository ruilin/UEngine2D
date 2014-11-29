/*
 * graphic.c
 *
 *  	Created on: 2013-9-21
 *      Author: ryan
 *      纹理管理
 */

#include "graphic.h"

PUBLIC Graphic *graphic_create() {
	Graphic *g = MALLOC(sizeof(Graphic));
	if (NULL == g) {
		LOGE("graphic_create() MALLOC fail!");
		goto _exit;
	}
	g->hash_tex = (void *)hash_init(10);	//2^10 = 1024
	g->rotate = GRAPHIC_ROTATE_0;
	g->turnover = GRAPHIC_TURN_NORMAL;
	g->red = g->green = g->blue = g->alpha = 1.0;
	g->fontSize = 30;
	g->strokeWidth = 0;
	g->paintStyle = 0;
	g->textRed = g->textGreen = g->textBlue = 0;
	g->textAlpha = 255;
	g->textRed_stroke = g->textGreen_stroke = g->textBlue_stroke = 0;
	g->textAlpha_stroke = 255;
_exit:
	return g;
}

PUBLIC void graphic_destroy(Graphic *g) {
	if (NULL == g) {
		LOGE("graphic_destroy() FREE fail!");
		goto _exit;
	}
	hash_destroy(g->hash_tex);
	FREE(g);
_exit:
	return;
}

/*
 * rescyle the texture
 */
PUBLIC void graphic_recyleTexture(void *graphic, void *img) {
	if (NULL == img) return;
	unsigned int texId;
	Graphic *g = (Graphic *)graphic;
	Texture *tex = (Texture *)img;
	if (TRUE == hash_get(g->hash_tex, tex, sizeof(tex), &texId, NULL)) {
		tex->isBinded = FALSE;
		glDeleteTextures(1, &texId);
		hash_unset(g->hash_tex, tex, sizeof(tex));
	} else {
		LOGE("graphic_recyleTexture(): tex is not found in hash");
	}
}

/* maps the quality (GFX_TEX_QUALITY_XXX) to specific texture parameters */
struct _tex_quality_parameters {
	GLfloat minFilter;
	GLfloat magFilter;
	GLfloat wrapModeS;
	GLfloat wrapModeT;
};

PRIVATE const struct _tex_quality_parameters _TEX_QUALITY_PARAMETERS[] = {
	/* GFX_TEX_QUALITY_NEAREST/GFX_TEX_QUALITY_LOW/GFX_TEX_QUALITY_FASTER */
	{GL_NEAREST	, GL_NEAREST, GL_REPEAT			, GL_REPEAT},
	/* GFX_TEX_QUALITY_LINEAR/GFX_TEX_QUALITY_HIGH */
	{GL_LINEAR	, GL_LINEAR , GL_CLAMP_TO_EDGE	, GL_CLAMP_TO_EDGE}
};

/*
 * 绑定opengl纹理
 */
PUBLIC unsigned int graphic_genTexture(Graphic *g, Texture *tex) {
	if (NULL == g || NULL == tex) {
		LOGE("graphic_genTexture() NULL == g || NULL == tex");
		return -1;
	}
//	if (TRUE == hash_get(g->hash_tex, tex, sizeof(tex), &(tex->texId), NULL)) {
//		goto _exit;
//	}
	if (TRUE == tex->isBinded) {
		goto _exit;
	}
	tex->isBinded = TRUE;
	glGenTextures(1, &(tex->texId));
	glBindTexture(GL_TEXTURE_2D, tex->texId);
	hash_set(g->hash_tex, tex, sizeof(tex), tex->texId, NULL);

	/* 设置纹理参数 */
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _TEX_QUALITY_PARAMETERS[tex->quality].minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _TEX_QUALITY_PARAMETERS[tex->quality].magFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _TEX_QUALITY_PARAMETERS[tex->quality].wrapModeS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _TEX_QUALITY_PARAMETERS[tex->quality].wrapModeT);

	switch (tex->bytesPerPixel) {
	case 4:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->widthPOT, tex->heightPOT, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid *)(tex->pixels));
		break;
	case 2:
		/* rgb_565 not tested */
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex->widthPOT, tex->heightPOT, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, (GLvoid *)(tex->pixels));
		break;
	case 1:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, tex->widthPOT, tex->heightPOT, 0, GL_ALPHA, GL_UNSIGNED_BYTE, (GLvoid *)(tex->pixels));
		break;
	default:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->widthPOT, tex->heightPOT, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid *)(tex->pixels));
		break;
	}
	image_setCallBackFun(tex, graphic_recyleTexture, g);
_exit:
	return tex->texId;
}

PUBLIC void graphic_setColor4f(Graphic *g, float red, float green, float blue, float alpha) {
	g->red = red;
	g->green = green;
	g->blue = blue;
	g->alpha = alpha;
	return;
}

/*
 * 0 ~ 255
 */
PUBLIC void graphic_setTextColor(Graphic *g, int red, int green, int blue, int alpha) {
	g->textRed = red;
	g->textGreen = green;
	g->textBlue = blue;
	g->textAlpha = alpha;
	return;
}

/*
 * 0 ~ 255
 */
PUBLIC void graphic_setTextStrokeColor(Graphic *g, int red, int green, int blue, int alpha) {
	g->textRed_stroke = red;
	g->textGreen_stroke = green;
	g->textBlue_stroke = blue;
	g->textAlpha_stroke = alpha;
	return;
}

PUBLIC void graphic_setTextStyle(Graphic *g, int fontSize, TEXT_STYLE paintStyle) {
	g->fontSize = fontSize;
	g->strokeWidth = fontSize >> 3;
	g->paintStyle = paintStyle;
	return;
}

