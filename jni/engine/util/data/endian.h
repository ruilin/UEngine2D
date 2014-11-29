/*
 * endian.h
 *
 *  Created on: 2012-10-31
 *      Author: ottofung
 */
#ifndef ENDIAN_H_
#define ENDIAN_H_

#include <base.h>

ENDIAN endian_set(void);
ENDIAN endian_get(void);
unsigned int endian_getLittleEndian4Bytes(const unsigned char *buf, int offset);
void endian_setLittleEndian4Bytes(const unsigned int value, unsigned char *buf);
unsigned short endian_getLittleEndian2Bytes(const unsigned char *buf, int offset);
void endian_setLittleEndian2Bytes(const unsigned short value, unsigned char *buf);
int endian_getBigEndian4Bytes(const unsigned char *buf, int offset);
unsigned short endian_getBigEndian2Bytes(const unsigned char *buf, int offset);

#endif /* ENDIAN_H_ */
