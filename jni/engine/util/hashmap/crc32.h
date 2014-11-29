/*
 * crc32.h
 *
 *  Created on: 2012-10-26
 *      Author: ottofung
 */
#ifndef CRC32_H_
#define CRC32_H_

#include <base.h>

PUBLIC const unsigned int _crc_table[256];

/* ========================================================================= */
#define _DO1(buf)	_save = _crc_table[((int)_save ^ (*buf++)) & 0xff] ^ (_save >> 8)
/* ========================================================================= */

#define CRC32(crc, buf, len, out) 																\
	do {																						\
		unsigned int _n;																		\
		const char *_key = (buf);																\
		unsigned int _save = (crc);																\
		if (NULL == (buf)) { 																	\
			(out) = 0;																			\
			break;																				\
		}																						\
		/* Duff's Device optimization, loop unroll + short-circuit the remainder */				\
		/* http://en.wikipedia.org/wiki/Duff%27s_device */										\
		_save = _save ^ 0xffffffff;																\
		_n = ((len) + 7) >> 3;		/* number of iterations */									\
		switch ((len) & 7) {		/* mod 8, the remainder to switch/jump for the first time */\
			case 0: do {	_DO1((_key));														\
			case 7: 		_DO1((_key));														\
			case 6: 		_DO1((_key));														\
			case 5: 		_DO1((_key));														\
			case 4: 		_DO1((_key));														\
			case 3: 		_DO1((_key));														\
			case 2: 		_DO1((_key));														\
			case 1: 		_DO1((_key));														\
					} while (--_n > 0);															\
					/* no break */																\
			default: break;	/* to keep the compiler happy */									\
		}																						\
		(out) = (_save ^ 0xffffffff);															\
	} while (0)

#endif /* CRC32_H_ */
