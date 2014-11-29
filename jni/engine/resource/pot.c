/*
 * pot.c
 *
 *  Created on: 2012-10-23
 *      Author: ottofung
 */
#include <string.h>

#include "pot.h"

PRIVATE inline unsigned int _next_pot(unsigned int x) {
	if ( !(x & (x-1)) ) return x;	/* already in pot */
	x |= x>>1;
	x |= x>>2;
	x |= x>>4;
	x |= x>>8;
	x |= x>>16;
	return x+1;
}

PUBLIC unsigned char *pot_bitmapToTexturePOT(unsigned char *oldBmp, unsigned int oldWidth, unsigned int oldHeight,
													unsigned int *newWidth, unsigned int *newHeight,
													unsigned char bytesPerPixel, BOOL *fSameSizeNoCreate) {
	/* caller needs to take care of the cleanup of the oldBmp and the newBmp (i.e. FREE()) */
	unsigned char *newBmp; unsigned int i; unsigned int newLen; unsigned int bitmap_offset, bitmap_offset2, stride, newStride;
	*newWidth = _next_pot(oldWidth); *newHeight = _next_pot(oldHeight);
	if ((*newWidth == oldWidth) && (*newHeight == oldHeight)) {
		/* it's silly to create a new bitmap if no resizing is needed */
		if (NULL != fSameSizeNoCreate)
			*fSameSizeNoCreate = TRUE;
		return oldBmp;
	}
	if (NULL != fSameSizeNoCreate)
		*fSameSizeNoCreate = FALSE;
	newLen = (*newWidth) * (*newHeight) * bytesPerPixel;
	newBmp = (unsigned char *)MALLOC(newLen);	/* needs check for failure? */
	/*
	 * otto: 2013/03/27 no need to memset() the new bitmap, as it will be overwritten by the new pixels,
	 * and the unwritten pixels are not gonna to be displayed anyway (clipped).
	 */
	bitmap_offset = bitmap_offset2 = 0;
	stride = oldWidth * bytesPerPixel;		/* original bitmap stride */
	newStride = *newWidth * bytesPerPixel;	/* new bitmap stride */
	/* copies each row from the original bitmap to the new bitmap */
	if (oldWidth < *newWidth) {
		/* newWidth (in POT) is larger than the oldWidth */
		for (i = 0; i < oldHeight; i++) {
			/* copies 1 row from the old bitmap to the new bitmap */
			memcpy((void *)(newBmp + bitmap_offset2), (void *)(oldBmp + bitmap_offset),	stride);
			/* copies the row's last pixel value to the edge */
			memcpy((void *)(newBmp + bitmap_offset2 + stride), (void *)(newBmp + bitmap_offset2 + stride - bytesPerPixel), bytesPerPixel);
			/* increments the offset of old and new bitmap by 1 row */
			bitmap_offset += stride; bitmap_offset2 += newStride;
		}
	} else {	/* same width */
		for (i = 0; i < oldHeight; i++) {
			/* copies 1 row from the old bitmap to the new bitmap */
			memcpy((void *)(newBmp + bitmap_offset2), (void *)(oldBmp + bitmap_offset),	stride);
			/* increments the offset of old and new bitmap by 1 row */
			bitmap_offset += stride; bitmap_offset2 += newStride;
		}
	}
	if (oldHeight < *newHeight)
		/* the newHeight (in POT) is larger than the oldHeight */
		/* copies the last line */
		memcpy((void *)(newBmp + bitmap_offset2), (void *)(newBmp + bitmap_offset2 - newStride), newStride);
	return newBmp;
}
