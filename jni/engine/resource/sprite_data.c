/*
 * sprite_data.c
 *
 *  Created on: 2013-10-14
 *      Author: ryan
 */

#include "sprite_data.h"


PUBLIC SpriteData *sprite_data_create(const char *filepath, unsigned short imgWidthPOT, unsigned short imgHeightPOT) {
	unsigned int offset = 4;
	unsigned int fileSize = 0;
	unsigned char *file = (unsigned char *)jni_lib_readFromAssets(filepath, &fileSize);

	SpriteData *data = MALLOC(sizeof(SpriteData));
	data->version = file[offset];
	offset += 1;
	data->flag = file[offset];
	offset += 1;
	data->tileCount = endian_getBigEndian4Bytes(file, offset);
	offset += 4;

	data->array_spriteTile = MALLOC(sizeof(SpriteData_Tile) * data->tileCount);
	int i, j;
	for (i = 0; i < data->tileCount; i++) {
		data->array_spriteTile[i].tileX = endian_getBigEndian2Bytes(file, offset);
		offset += 2;
		data->array_spriteTile[i].tileY = endian_getBigEndian2Bytes(file, offset);
		offset += 2;
		data->array_spriteTile[i].tileWdith = endian_getBigEndian2Bytes(file, offset);
		data->array_spriteTile[i].tileWdith_shadow = 12;
		offset += 2;
		data->array_spriteTile[i].tileHeight = endian_getBigEndian2Bytes(file, offset);
		data->array_spriteTile[i].tileHeight_shadow = (int)data->array_spriteTile[i].tileHeight >> 1;
		offset += 2;

		data->array_spriteTile[i].sxRatio = data->array_spriteTile[i].tileX / imgWidthPOT;
		data->array_spriteTile[i].exRatio = (data->array_spriteTile[i].tileX + data->array_spriteTile[i].tileWdith) / imgWidthPOT;
		data->array_spriteTile[i].syRatio = data->array_spriteTile[i].tileY / imgHeightPOT;
		data->array_spriteTile[i].eyRatio = (data->array_spriteTile[i].tileY + data->array_spriteTile[i].tileHeight) / imgHeightPOT;
	}
	data->frameCount = endian_getBigEndian4Bytes(file, offset);
	offset += 4;

	data->array_frame = MALLOC(sizeof(SpriteData_Frame) * data->frameCount);
	for (i = 0; i < data->frameCount; i++) {
		data->array_frame[i].frameTileCount = endian_getBigEndian4Bytes(file, offset);
		offset += 4;
		data->array_frame[i].collisionCount = endian_getBigEndian4Bytes(file, offset);
		offset += 4;
		data->array_frame[i].referencePointCount = endian_getBigEndian4Bytes(file, offset);
		offset += 4;
		data->array_frame[i].topOffset = endian_getBigEndian2Bytes(file, offset);
		offset += 2;
		data->array_frame[i].bottomOffset = endian_getBigEndian2Bytes(file, offset);
		offset += 2;
		data->array_frame[i].leftOffset = endian_getBigEndian2Bytes(file, offset);
		offset += 2;
		data->array_frame[i].rightOffset = endian_getBigEndian2Bytes(file, offset);
		offset += 2;

		data->array_frame[i].frameTiles = MALLOC(sizeof(SpriteData_Frame_Tile) * data->array_frame[i].frameTileCount);
		for (j = 0; j < data->array_frame[i].frameTileCount; j++) {
			data->array_frame[i].frameTiles[j].tileIndex = endian_getBigEndian2Bytes(file, offset);
			offset += 2;
			data->array_frame[i].frameTiles[j].x = endian_getBigEndian2Bytes(file, offset);
			offset += 2;
			data->array_frame[i].frameTiles[j].y = endian_getBigEndian2Bytes(file, offset);
			offset += 2;
			data->array_frame[i].frameTiles[j].trans = endian_getBigEndian2Bytes(file, offset);
			offset += 2;
		}

		data->array_frame[i].collisions = MALLOC(sizeof(SpriteData_Collision) * data->array_frame[i].collisionCount);
		for (j = 0; j < data->array_frame[i].collisionCount; j++) {
			data->array_frame[i].collisions[j].x = endian_getBigEndian2Bytes(file, offset);
			offset += 2;
			data->array_frame[i].collisions[j].y = endian_getBigEndian2Bytes(file, offset);
			offset += 2;
			data->array_frame[i].collisions[j].width = endian_getBigEndian2Bytes(file, offset);
			offset += 2;
			data->array_frame[i].collisions[j].height = endian_getBigEndian2Bytes(file, offset);
			offset += 2;
		}

		data->array_frame[i].rPoints = MALLOC(sizeof(SpriteData_ReferenccePoint) * data->array_frame[i].referencePointCount);
		for (j = 0; j < data->array_frame[i].referencePointCount; j++) {
			data->array_frame[i].rPoints[j].x = endian_getBigEndian2Bytes(file, offset);
			offset += 2;
			data->array_frame[i].rPoints[j].y = endian_getBigEndian2Bytes(file, offset);
			offset += 2;
		}
	}
	data->actionCount = endian_getBigEndian4Bytes(file, offset);
	offset += 4;

	data->array_action = MALLOC(sizeof(SpriteData_Action) * data->actionCount);
	for (i = 0; i < data->actionCount; i++) {
		data->array_action[i].frameCount = endian_getBigEndian4Bytes(file, offset);
		offset += 4;
		char delay = file[offset];
		offset += 1;
		endian_getBigEndian4Bytes(file, offset);
		offset += 4;
		data->array_action[i].array_actionFrame = MALLOC(sizeof(SpriteData_Action_Frame) * data->array_action[i].frameCount);
		if (1 == delay) {
			for (j = 0; j < data->array_action[i].frameCount; j++) {
				data->array_action[i].array_actionFrame[j].frameIndex = endian_getBigEndian2Bytes(file, offset);
				offset += 2;
				data->array_action[i].array_actionFrame[j].delayTime = endian_getBigEndian2Bytes(file, offset);
				offset += 2;
			}
		} else {
			for (j = 0; j < data->array_action[i].frameCount; j++) {
				data->array_action[i].array_actionFrame[j].frameIndex = endian_getBigEndian2Bytes(file, offset);
				offset += 2;
				data->array_action[i].array_actionFrame[j].delayTime = 60;
			}
		}
	}
	FREE(file);
	return data;
}


PUBLIC void sprite_data_destroy(SpriteData *data) {
	if (NULL != data) {
		int i;
		for (i = 0; i < data->frameCount; i++) {
			if (NULL != data->array_frame[i].frameTiles) FREE(data->array_frame[i].frameTiles);
			if (NULL != data->array_frame[i].collisions) FREE(data->array_frame[i].collisions);
			if (NULL != data->array_frame[i].rPoints) FREE(data->array_frame[i].rPoints);
		}
		for (i = 0; i < data->actionCount; i++) {
			if (NULL != data->array_action[i].array_actionFrame) FREE(data->array_action[i].array_actionFrame);
		}
		FREE(data->array_action);
		FREE(data->array_spriteTile);
		FREE(data->array_frame);
		FREE(data);
	}
	return;
}


