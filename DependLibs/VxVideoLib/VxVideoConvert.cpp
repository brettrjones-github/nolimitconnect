//============================================================================
// Copyright (C) 2013 Brett R. Jones 
// All Rights Reserved ( After My Death I Brett R. Jones Issue this source code to Public Domain )
//
// You may not modify or redistribute this code for any reason 
// without written consent of Brett R. Jones
//
// This code is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// brettjones1900@gmail.com
// http://www.p2panarchy.com
//============================================================================

#include "VxVideoLib.h"
#include "GdvBufferUtil.h"
#include "VxRescaleRgb.h"

#include <libjpg/VxJpgLib.h>
#include <CoreLib/VxDebug.h>

#include <string.h>

//! convert image
// NOTE 1: caller must delete the returned buffer
uint8_t * VxConvertToJpg(	uint32_t	u32FourCcIn,		// FOURCC of format to convert
							uint8_t *	pu8DataIn,			// data to convert
							int			iDataLen,			// length of data 
							int 		iImageWidthIn,		// width of image in pixels
							int 		iImageHeightIn,		// height of image in pixels
							uint32_t&	u32RetDataLen,		// data length of jpg image
							int			iResizeToWidth,		// if not zero then resize to given length
							int			iResizeToHeight		// if not zero then resize to given height
							)
{
	uint8_t * pu8VidData = pu8DataIn;
	uint8_t * pu8DataToDelete = 0;

	LogMsg( LOG_DEBUG, "VxConvertToJpg fourcc 0x%X", u32FourCcIn );
	bool bConvert =  (FOURCC_RGB != u32FourCcIn);
	if( bConvert )
	{
		// need to convert to rgb.. remember to delete returned data
		pu8VidData = VxConvertImage(	u32FourCcIn,		// FOURCC of format to convert
										pu8DataIn,			// data to convert
										iImageWidthIn,		// width of image in pixels
										iImageHeightIn,		// height of image in pixels
										FOURCC_RGB,			// FOURCC of format to convert to 
										u32RetDataLen );	// return data length of converted image
		pu8DataToDelete = pu8VidData;
	}
	if( iResizeToWidth &&
		((iResizeToWidth != iImageWidthIn) || (iResizeToHeight != iImageHeightIn)) )
	{
		pu8VidData = VxResizeRgbImage( pu8VidData, iImageWidthIn, iImageHeightIn, iResizeToWidth, iResizeToHeight, 0 );
		if( pu8DataToDelete )
		{
			delete pu8DataToDelete;
		}
		pu8DataToDelete = pu8VidData;
		iImageWidthIn = iResizeToWidth;
		iImageHeightIn = iResizeToHeight;
	}

	long s32JpgDataLen = 0;
	// compress from rgb to jpg for sending
	// take a guess at what size the jpg will be
	int iMaxJpgSize = iImageWidthIn * iImageHeightIn * 3;

	uint8_t* pu8JpgData = new uint8_t[ iMaxJpgSize ];
	RCODE rc = VxBmp2Jpg(	24,					// number of bits each pixel..(For now must be 24)
							pu8VidData,			// bits of bmp to convert
							iImageWidthIn,		// width of image in pixels
							iImageHeightIn,		// height of image in pixels
							75,					// quality of image
							iMaxJpgSize,		// maximum length of pu8RetJpg
							pu8JpgData,			// buffer to return Jpeg image
							&s32JpgDataLen );	// return length of jpeg image

	// clean up allocated data
	delete pu8DataToDelete;
	if( 0 != rc )
	{
		LogMsg( LOG_ERROR, "VxConvertToJpg: JPEG Conversion error %d", rc );
		u32RetDataLen = 0;
	}
	else
	{
		u32RetDataLen = s32JpgDataLen;
		LogMsg( LOG_INFO, "VxConvertToJpg: width %d height %d len %d.. compressed to %d", iImageWidthIn, iImageHeightIn, iDataLen, s32JpgDataLen  );
	}
	// return jpg data
	return pu8JpgData;
}


//============================================================================
//! convert image
void decodeYUV420SP( uint8_t * rgb, uint8_t * yuv420sp, int width, int height)
{
	int frameSize = width * height;

	for (int j = 0, yp = 0; j < height; j++)
	{
		int uvp = frameSize + (j >> 1) * width, u = 0, v = 0;
		for (int i = 0; i < width; i++, yp++)
		{
			int y = (0xff & ((int) yuv420sp[yp])) - 16;
			if (y < 0) y = 0;
			if ((i & 1) == 0) {
				v = (0xff & yuv420sp[uvp++]) - 128;
				u = (0xff & yuv420sp[uvp++]) - 128;
			}

			int y1192 = 1192 * y;
			int r = (y1192 + 1634 * v);
			int g = (y1192 - 833 * v - 400 * u);
			int b = (y1192 + 2066 * u);

			if (r < 0) r = 0; else if (r > 262143) r = 262143;
			if (g < 0) g = 0; else if (g > 262143) g = 262143;
			if (b < 0) b = 0; else if (b > 262143) b = 262143;

			*rgb = ( r >> 10 ) & 0xff;
			rgb++;
			*rgb = ( g >> 10 ) & 0xff;
			rgb++;
			*rgb = (b>>10)&0xff;
			rgb++;

			//rgb[yp] = 0xff000000 | ((r << 6) & 0xff0000) | ((g >> 2) & 0xff00) | ((b >> 10) & 0xff);
		}
	}
}

//============================================================================
//! convert image
// NOTE 1: caller must delete the returned buffer
uint8_t * VxConvertImage(	uint32_t		u32FourCcIn,		// FOURCC of format to convert
						uint8_t *	pu8DataIn,			// data to convert
						int 	iImageWidth,		// width of image in pixels
						int 	iImageHeight,		// height of image in pixels
						uint32_t		u32FourCcConverTo,	// FOURCC of format to convert to 
						uint32_t&	u32RetDataLen )		// data length of converted image
{
	uint32_t formatToConvertTo = FOURCC_RGB; 
	u32RetDataLen = VxCalcImageDataLen( formatToConvertTo, iImageWidth, iImageHeight );
	uint8_t * pu8DataOut = new uint8_t[ u32RetDataLen ];
	if( FOURCC_YUVS == u32FourCcIn ) // special for android YUV 4.2.0 SP
	{
		// decode android default format
		//LogMsg( LOG_DEBUG, "VxConvertImage decodeYUV420SP 0x%x to 0x%x\n", u32FourCcIn, formatToConvertTo );
		decodeYUV420SP( pu8DataOut, pu8DataIn, iImageWidth, iImageHeight );
		return pu8DataOut;
	}

	//LogMsg( LOG_DEBUG, "VxConvertImage  0x%x to 0x%\n", u32FourCcIn, formatToConvertTo );
	gdv_videobuffer_converter_t converter = gdv_videobuffer_get_converter( u32FourCcIn, formatToConvertTo );
	if( NULL != converter )
	{
		converter(	pu8DataIn, 
					iImageWidth, 
					iImageHeight, 
					VxCalcImageStride( u32FourCcIn, iImageWidth ), 
					pu8DataOut, 
					iImageWidth, 
					iImageHeight, 
					VxCalcImageStride( u32FourCcConverTo, iImageWidth ) );
		return pu8DataOut;
	}

	LogMsg( LOG_DEBUG, "VxConvertImage: Unrecognized video format" );
	delete[] pu8DataOut;
	return NULL;
}

//============================================================================
// calculate length of buffer required to hold image
uint32_t VxCalcImageDataLen(	uint32_t	u32FourCcIn,			// FOURCC of format
								int 		iImageWidth,		// width of image in pixels
								int 		iImageHeight )		// height of image in pixels
{
	uint32_t u32DataLen = 0;
	switch( u32FourCcIn )
	{
	case FOURCC_RGB:
		u32DataLen = iImageWidth * iImageHeight * 3;
		break;
	case FOURCC_RGBA:
		u32DataLen = iImageWidth * iImageHeight * 4;
		break;
	case FOURCC_BGR:
		u32DataLen = iImageWidth * iImageHeight * 3;
		break;
	case FOURCC_BGRA:
		u32DataLen = iImageWidth * iImageHeight * 4;
		break;
	case FOURCC_YUVS: // special for android YUV 4.2.0 SP
	case FOURCC_I420:
		u32DataLen = (uint32_t)((float)(iImageWidth * iImageHeight) * 1.5F);
		break;
	case FOURCC_YUY2:
		u32DataLen = iImageWidth * iImageHeight * 2;
		break;
	case FOURCC_YV12:
		u32DataLen = iImageWidth * iImageHeight * 2;
		break;
	case FOURCC_IYUV:
		u32DataLen = iImageWidth * iImageHeight * 2;
		break;
	}
	return u32DataLen;
}

//============================================================================
// calculate bytes per line of pixels.... assumes 4 byte boundary for RGB else pixels * bytes_per_pixels
uint32_t VxCalcImageStride(	uint32_t		u32FourCcIn,		// FOURCC of format
						int 	iImageWidth )		// width of image in pixels
{
	uint32_t u32Stride = 0;
	switch( u32FourCcIn )
	{
	case FOURCC_RGB:
		u32Stride = iImageWidth * 3;
		break;
	case FOURCC_RGBA:
		u32Stride = iImageWidth * 4;
		break;
	case FOURCC_BGR:
		u32Stride = iImageWidth * 3;
		break;
	case FOURCC_BGRA:
		u32Stride = iImageWidth * 4;
		break;
	case FOURCC_YUVS: // special for android YUV 4.2.0 SP
	case FOURCC_I420:
		u32Stride = iImageWidth * 2;
		break;
	case FOURCC_YUY2:
		u32Stride = (uint32_t)(iImageWidth * 3);
		break;
	case FOURCC_YV12:
		u32Stride = iImageWidth * 2;
		break;
	case FOURCC_IYUV:
		u32Stride = iImageWidth * 2;
		break;
	}

	return u32Stride;
}

