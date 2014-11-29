/*
 * map_data.h
 *
 *  Created on: 2013-9-25
 *      Author: ryan
 */

#ifndef MAP_DATA_H_
#define MAP_DATA_H_

#include <stdlib.h>
#include <base.h>

typedef struct {
	unsigned short clipid;
	short left;
	short y;
} MapData_Tile;

typedef struct {
	unsigned short width;
	unsigned short height;
	BOOL hasRefPoint;
	MapData_Tile refTile;	/* 遮挡参考点 / reference tile of sequence layers */
	unsigned short tilesSize;
	MapData_Tile *tileList;
} MapData_Layer;

typedef struct {
	unsigned short width;
	unsigned short height;
	unsigned short sourceId;
} MapData_Tileset_ResImg;

typedef struct {
	unsigned short firstgrid;
	unsigned short margin;
	unsigned short spacing;
	unsigned short tileHeight;
	unsigned short tileWidth;
	unsigned short quality;
	MapData_Tileset_ResImg image;
} MapData_Tileset;

typedef struct MapData {
	unsigned short width;
	unsigned short height;
	unsigned short tilewidth;
	unsigned short tileheight;
	/* barrier */
	unsigned short barrierSize;
	unsigned char *array_barrier;
	/* shadow */
	unsigned short shadowSize;
	unsigned char *array_shadow;
	/* tileset */
	unsigned short tilesetSize;
	MapData_Tileset *array_tileset;
	/* layer */
	unsigned short layersSize;
	MapData_Layer *array_layer;
} MapData;

MapData *map_data_create(const char *filepath);
void map_data_destroy(MapData *map);
BOOL map_data_isBarrier(MapData *map, int locX, int locY);
BOOL map_data_isShadow(MapData *map, int locX, int locY);

#endif /* MAP_DATA_H_ */
