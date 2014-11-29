/*
 * sprite_data.h
 *
 *  Created on: 2013-10-14
 *      Author: ryan
 */

#ifndef SPRITE_DATA_H_
#define SPRITE_DATA_H_

#include <base.h>

//frame attrib
#define	FRAME_HEADER_SIZE	8

//action attrib
#define ACTION_HEADER_SIZE	4

#define INVERTED_AXES		0x4

//sprite transform type
typedef enum {
	  TRANS_NONE = 0,
	  TRANS_ROT90 = 5,
	  TRANS_ROT180 = 3,
	  TRANS_ROT270 = 6,
	  TRANS_MIRROR = 2,
	  TRANS_MIRROR_ROT90 = 7,
	  TRANS_MIRROR_ROT180 = 1,
	  TRANS_MIRROR_ROT270 = 4
} SPRITE_TRANS;

//rotate bit
typedef enum {
	  ANTICLOCKWISE_90 = 0,
	  DEASIL_90 = 1,
	  HORIZONTAL = 2,
	  VERTICAL = 3,
} SPRITE_ROTATE;

//sprite direction
typedef enum {
	  DIRECTION_NONE = 0,
	  DIRECTION_UP = 1,
	  DIRECTION_DOWN = 2,
	  DIRECTION_LEFT = 3,
	  DIRECTION_RIGHT = 4,
} SPRITE_DATA_DIRECTION;


//frame data bit
typedef enum {
	  FRAME_TILE_COUNT_BIT = 1,
	  FRAME_COLLISION_COUNT_BIT = 2,
	  FRAME_REFERENCE_POINT_COUNT_BIT = 3,
	  FRAME_TOP_POS_BIT = 4,
	  FRAME_BOTTOM_POS_BIT = 5,
	  FRAME_LEFT_POS_BIT = 6,
	  FRAME_RIGHT_POS_BIT = 7,
} SPRITE_DATA_FRAME;


//action data bit
typedef enum {
	  ACTION_SEQUENCE_LENGTH_BIT = 1,
	  ACTION_SEQUENCE_DELAY_BIT = 2,
	  ACTION_TRANSFORM_BIT = 3,
} SPRITE_DATA_ACTION;

//collision type
typedef enum {
	  COLLISION_INTERSECT = 1,
	  COLLISION_INCLUSION = 2,
} SPRITE_DATA_COLLISION;


typedef struct {
	float tileX;
	float tileY;
	float tileWdith;
	float tileWdith_shadow;
	float tileHeight;
	float tileHeight_shadow;
	float sxRatio;
	float syRatio;
	float exRatio;
	float eyRatio;
} SpriteData_Tile;

/* 图块 */
typedef struct {
	short tileIndex;
	short x;			//在资源图中的x位置
	short y;			//在资源图中的y位置
	SPRITE_TRANS trans;
} SpriteData_Frame_Tile;

/* 碰撞的作用区域 */
typedef struct {
	short x;
	short y;
	short width;
	short height;
} SpriteData_Collision;

/* 参考点 */
typedef struct {
	short x;
	short y;
} SpriteData_ReferenccePoint;

/* 图片帧 */
typedef struct {
	unsigned int frameTileCount;
	SpriteData_Frame_Tile *frameTiles;
	/* 碰撞区域数组 */
	unsigned int collisionCount;
	SpriteData_Collision *collisions;
	/* 参考点数组 */
	unsigned int referencePointCount;
	SpriteData_ReferenccePoint *rPoints;
	short topOffset;
	short bottomOffset;
	short leftOffset;
	short rightOffset;
} SpriteData_Frame;

/* 动作帧 */
typedef struct {
	short frameIndex;	/* 图片帧索引 */
	short delayTime;	/* 播放的延迟时间 */
} SpriteData_Action_Frame;

/* 动作：一个动作相当于一个独立的动画，比如一个方向的行走 */
typedef struct {
	unsigned int frameCount;
	SpriteData_Action_Frame *array_actionFrame;
} SpriteData_Action;

typedef struct {
	unsigned char version;
	unsigned char flag;
	unsigned int tileCount;
	SpriteData_Tile *array_spriteTile;
	unsigned int frameCount;
	SpriteData_Frame *array_frame;
	unsigned int actionCount;
	SpriteData_Action *array_action;
} SpriteData;


SpriteData *sprite_data_create(const char *filepath, unsigned short imgWidth, unsigned short imgHeight);
void sprite_data_destroy(SpriteData *data);

#endif /* SPRITE_DATA_H_ */
