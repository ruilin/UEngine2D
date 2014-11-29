/*
 * endian.c
 *
 *  Created on: 2012-10-31
 *      Author: ottofung
 */

#include "endian.h"

/*
 *  *** NOTE *** Teon's network byte order is little endianess (i.e. != Internet network byte order),
 * 	so we need to convert the byte order before network send and after network receive
 *	followings are the helper routines
 */
PRIVATE ENDIAN _endian = ENDIAN_BIG;	/* default to ENDIAN_BIG */

PUBLIC ENDIAN endian_set(void) {
    union {
        unsigned int i;
        char c[4];
    } bint = {0x01020304};
    /* statically set the endianess, endianess never changes for the environment, so it's ok to be static */
    /* must be called upon initialization, else we default to ENDIAN_BIG */
    _endian = (bint.c[0] == 1 ? ENDIAN_BIG : ENDIAN_LITTLE);
    return _endian;
}

PUBLIC ENDIAN endian_get(void) {
	return _endian;
}

PUBLIC unsigned int endian_getLittleEndian4Bytes(const unsigned char *buf, int offset) {
	/*
	 * Little endianess to environment endianess��
	 * This is for decoding external data (files, network data etc.) buffer that is represented
	 * in little endianess to environment endianess variables.
	 */
	unsigned int ret;
	if (ENDIAN_LITTLE == _endian) {
		/* takes the buffer and assigns it directly to the unsigned int in the same order */
		ret = *(const unsigned int *)(buf+offset);	/* same endianess */
	} else {
		/* DCBA -> ABCD */
		ret = (*(buf+offset+3) << 24 ) | (*(buf+offset+2) << 16) | (*(buf+offset+1) << 8) | *(buf+offset);
	}
	return ret;
}

PUBLIC void endian_setLittleEndian4Bytes(const unsigned int value, unsigned char *buf) {
	/*
	 * Sets the value to little endian buffer (buf length must be at least 4)
	 * This is for converting internal representation to little endianess for outgoing data.
	 */
	buf[0] = (value & 0x000000FF);
	buf[1] = (value & 0x0000FF00) >> 8;
	buf[2] = (value & 0x00FF0000) >> 16;
	buf[3] = (value & 0xFF000000) >> 24;
	return;
}

PUBLIC unsigned short endian_getLittleEndian2Bytes(const unsigned char *buf, int offset) {
	/*
	 * Little endianess to environment endianess��
	 * This is for decoding external data (files, network data etc.) buffer that is represented
	 * in little endianess to environment endianess variables.
	 */
	unsigned short ret;
	if (ENDIAN_LITTLE == _endian) {
		/* takes the buffer and assigns it directly to the unsigned int in the same order */
		ret = *(const unsigned short *)(buf+offset);	/* same endianess */
	} else {
		/* BA -> AB */
		ret = (*(buf+offset+1) << 8) | *(buf+offset);
	}
	return ret;
}

PUBLIC void endian_setLittleEndian2Bytes(const unsigned short value, unsigned char *buf) {
	/*
	 * Sets the value to little endian buffer (buf length must be at least 2)
	 * This is for converting internal representation to little endianess for outgoing data.
	 */
	/* low byte */
	buf[0] = (value & 0x00FF);
	/* hight byte */
	buf[1] = (value & 0xFF00) >> 8;
	return;
}

PUBLIC int endian_getBigEndian4Bytes(const unsigned char *buf, int offset) {
	return    buf[offset+3] & 0xff
		   | (buf[offset+2] & 0xff) << 8
		   | (buf[offset+1] & 0xff) << 16
		   | (buf[offset] & 0xff) << 24;
}

PUBLIC unsigned short endian_getBigEndian2Bytes(const unsigned char *buf, int offset) {
    return (unsigned short)( buf[offset+1] & 0xff
    						|(buf[offset] & 0xff) << 8 );
}

