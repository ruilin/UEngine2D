/*
 * pot.h
 *
 *  Created on: 2012-10-23
 *      Author: ottofung
 */
#ifndef POT_H_
#define POT_H_

#include <base.h>

unsigned char *pot_bitmapToTexturePOT(unsigned char *oldBmp, unsigned int oldWidth, unsigned int oldHeight,
														unsigned int *newWidth, unsigned int *newHeight,
														unsigned char bytesPerPixel, BOOL *fSameSizeNoCreate);

#endif /* POT_H_ */
