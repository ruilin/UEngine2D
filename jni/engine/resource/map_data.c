/*
 * map_data.c
 *
 *  Created on: 2013-9-25
 *      Author: ryan
 */


#include "map_data.h"

PUBLIC MapData *map_data_create(const char *filepath) {
	unsigned int offset = 0;
	unsigned int fileSize = 0;
	unsigned char *mpd = (unsigned char *)jni_lib_readFromAssets(filepath, &fileSize);
	MapData *map = MALLOC(sizeof(MapData));
	map->width = endian_getBigEndian2Bytes(mpd, offset);
	offset +=2;
	map->height = endian_getBigEndian2Bytes(mpd, offset);
	offset +=2;
	map->tilewidth = endian_getBigEndian2Bytes(mpd, offset);
	offset +=2;
	map->tileheight = endian_getBigEndian2Bytes(mpd, offset);
	offset +=2;
	/* 阻挡 */
	map->barrierSize = endian_getBigEndian2Bytes(mpd, offset);
	offset +=2;
	if (map->barrierSize > 0) {
		map->array_barrier = MALLOC(map->barrierSize * sizeof(unsigned char));
		memcpy((map->array_barrier), &mpd[offset], map->barrierSize);
		offset += map->barrierSize;
	} else {
		map->array_barrier = NULL;
	}
	/* 阴影 */
	map->shadowSize = endian_getBigEndian2Bytes(mpd, offset);
	offset +=2;
	if (map->shadowSize > 0) {
		map->array_shadow = MALLOC(map->shadowSize * sizeof(unsigned char));
		memcpy((map->array_shadow), &mpd[offset], map->shadowSize);
		offset += map->shadowSize;
	} else {
		map->array_shadow = NULL;
	}

	map->tilesetSize = endian_getBigEndian2Bytes(mpd, offset);
	offset +=2;
	map->array_tileset = MALLOC(map->tilesetSize * sizeof(MapData_Tileset));
	int i, j;
	for (i = 0; i < map->tilesetSize; i++) {
		MapData_Tileset *tileset = &(map->array_tileset[i]);
		tileset->firstgrid = endian_getBigEndian2Bytes(mpd, offset);
		offset +=2;
		tileset->margin = endian_getBigEndian2Bytes(mpd, offset);
		offset +=2;
		tileset->spacing = endian_getBigEndian2Bytes(mpd, offset);
		offset +=2;
		tileset->tileWidth = endian_getBigEndian2Bytes(mpd, offset);
		offset +=2;
		tileset->tileHeight = endian_getBigEndian2Bytes(mpd, offset);
		offset +=2;
		tileset->quality = endian_getBigEndian2Bytes(mpd, offset);
		offset +=2;
		//image
		tileset->image.sourceId = endian_getBigEndian2Bytes(mpd, offset);
		offset +=2;
		tileset->image.width = endian_getBigEndian2Bytes(mpd, offset);
		offset +=2;
		tileset->image.height = endian_getBigEndian2Bytes(mpd, offset);
		offset +=2;
	}
	map->layersSize = endian_getBigEndian2Bytes(mpd, offset);
	offset +=2;
	map->array_layer = MALLOC(map->layersSize * sizeof(MapData_Layer));
	for (i = 0; i < map->layersSize; i++) {
		MapData_Layer *layer = &(map->array_layer[i]);
		layer->width = endian_getBigEndian2Bytes(mpd, offset);
		offset +=2;
		layer->height = endian_getBigEndian2Bytes(mpd, offset);
		offset +=2;
		/* TODO 待添加 */
		layer->hasRefPoint = mpd[offset];
		offset += 1;
		if (TRUE == layer->hasRefPoint) {
			layer->refTile.clipid = 0;
			layer->refTile.left = endian_getBigEndian2Bytes(mpd, offset);
			offset +=2;
			layer->refTile.y = endian_getBigEndian2Bytes(mpd, offset);
			offset +=2;
		}
		layer->tilesSize = endian_getBigEndian2Bytes(mpd, offset);
		offset +=2;
		layer->tileList = MALLOC(layer->tilesSize * sizeof(MapData_Tile));
		for (j = 0; j < layer->tilesSize; j++) {
			MapData_Tile *tile = &(layer->tileList[j]);
			tile->clipid = endian_getBigEndian2Bytes(mpd, offset);
			offset +=2;
			tile->left = endian_getBigEndian2Bytes(mpd, offset);
			offset +=2;
			tile->y = endian_getBigEndian2Bytes(mpd, offset);
			offset +=2;
		}
	}
	FREE(mpd);
	return map;
}

PUBLIC void map_data_destroy(MapData *map) {
	if (NULL != map) {
		int i, j;
		for (i = 0; i < map->layersSize; i++) {
			FREE(map->array_layer[i].tileList);
		}
		if (NULL != map->array_tileset) FREE(map->array_tileset);
		if (NULL != map->array_layer) FREE(map->array_layer);
		if (NULL != map->array_barrier) FREE(map->array_barrier);
		if (NULL != map->array_shadow) FREE(map->array_shadow);
		FREE(map);
	}
	return;
}

PUBLIC inline BOOL map_data_isBarrier(MapData *map, int locX, int locY) {
	if (NULL != map->array_barrier && (locX >= 0 && locX < map->width) && (locY >= 0 && locY < map->height))
		return (1 == map->array_barrier[locY * map->width + locX]);
	else
		return TRUE;
}

PUBLIC inline BOOL map_data_isShadow(MapData *map, int locX, int locY) {
	if (NULL != map->array_shadow && (locX >= 0 && locX < map->width) && (locY >= 0 && locY < map->height))
		return (1 == map->array_shadow[locY * map->width + locX]);
	else
		return FALSE;
}
