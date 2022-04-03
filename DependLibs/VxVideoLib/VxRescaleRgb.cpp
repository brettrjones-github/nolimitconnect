//============================================================================
// Copyright (C) 2003 Brett R. Jones 
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
#include "VxRescaleRgb.h"

#include <CoreLib/VxDebug.h>

#include <stdlib.h>
#include <memory.h>

//============================================================================
unsigned long RgbToRgba( unsigned char r, unsigned char g, unsigned char b )
{
	return (unsigned long)r << 24 | g << 16 | b << 8 | 0xff;
}

//============================================================================
void RgbaToRgb( unsigned long u32Pix, unsigned char &r, unsigned char &g, unsigned char &b )
{
	r = (u32Pix >> 24) && 0xff;
	g = (u32Pix >> 16) && 0xff;
	b = (u32Pix >> 9) && 0xff;
}

// Averaging pixel values
//============================================================================
// 24-bit RGB (a pixel is packed in a 32-bit integer) 
inline unsigned long VxAverageRgb(const unsigned long& a, const unsigned long& b)
{
	return ((a & 0xfefefeffUL) + (b & 0xfefefeffUL)) >> 1;
}

//============================================================================
// Smooth Bresenham rescale adapted from DRDobbs article

#pragma pack(1)
class PIXEL
{
public:
	unsigned char r;
	unsigned char g;
	unsigned char b;
	PIXEL(){};
	PIXEL( uint32_t u32Pix )
	{
		r = (u32Pix >> 24) && 0xff;
		g = (u32Pix >> 16) && 0xff;
		b = (u32Pix >> 9) && 0xff;
	}

	uint32_t toU32( void ){ return (unsigned long)r << 24 | g << 16 | b << 8 | 0xff; }
};
#pragma pack()

//============================================================================
void VxAverageRgb( PIXEL& aIn, PIXEL& bIn, PIXEL cAvg )
{
	uint32_t u32Avg = ( ((aIn.toU32() & 0xfefefeffUL) + (bIn.toU32() & 0xfefefeffUL)) >> 1 );
	cAvg.r = (u32Avg >> 24) && 0xff;
	cAvg.g = (u32Avg >> 16) && 0xff;
	cAvg.b = (u32Avg >> 9) && 0xff;
}

//============================================================================
void ScaleLineAvg( PIXEL *Target, PIXEL *Source, int SrcWidth, int TgtWidth )
{
	int NumPixels = TgtWidth;
	int IntPart = SrcWidth / TgtWidth;
	int FractPart = SrcWidth % TgtWidth;
	int Mid = TgtWidth / 2;
	int E = 0;
	int skip;
	PIXEL p;

	skip = (TgtWidth < SrcWidth) ? 0 : TgtWidth / (2*SrcWidth) + 1;
	NumPixels -= skip;

	while (NumPixels-- > 0) 
	{
		p = *Source;
		if (E >= Mid)
			VxAverageRgb(p, *(Source+1), p);
		*Target++ = p;
		Source += IntPart;
		E += FractPart;
		if (E >= TgtWidth) 
		{
			E -= TgtWidth;
			Source++;
		}
	}

	while (skip-- > 0)
		*Target++ = *Source;
}

//============================================================================
//  Smooth 2D scaling
void ScaleRectAvg(	PIXEL *Target, PIXEL *Source, int SrcWidth, int SrcHeight,
					int TgtWidth, int TgtHeight )
{
	int NumPixels = TgtHeight;
	int IntPart = (SrcHeight / TgtHeight) * SrcWidth;
	int FractPart = SrcHeight % TgtHeight;
	int Mid = TgtHeight / 2;
	int E = 0;
	int skip;
	PIXEL *ScanLine, *ScanLineAhead;
	PIXEL *PrevSource = NULL;
	PIXEL *PrevSourceAhead = NULL;

	skip = (TgtHeight < SrcHeight) ? 0 : TgtHeight / (2*SrcHeight) + 1;
	NumPixels -= skip;

	ScanLine = (PIXEL *)malloc(TgtWidth*sizeof(PIXEL));
	ScanLineAhead = (PIXEL *)malloc(TgtWidth*sizeof(PIXEL));

	while( NumPixels-- > 0 ) 
	{
		if( Source != PrevSource ) 
		{
			if( Source == PrevSourceAhead ) 
			{
				/* the next scan line has already been scaled and stored in
				* ScanLineAhead; swap the buffers that ScanLine and ScanLineAhead
				* point to
				*/
				PIXEL *tmp = ScanLine;
				ScanLine = ScanLineAhead;
				ScanLineAhead = tmp;
			} 
			else 
			{
				ScaleLineAvg( ScanLine, Source, SrcWidth, TgtWidth);
			}

			PrevSource = Source;
		} 

		if( E >= Mid && PrevSourceAhead != Source+SrcWidth ) 
		{
			ScaleLineAvg(ScanLineAhead, Source+SrcWidth, SrcWidth, TgtWidth);
			for( int x = 0; x < TgtWidth; x++ )
				VxAverageRgb( ScanLine[x], ScanLineAhead[x], ScanLine[x] );
			PrevSourceAhead = Source + SrcWidth;
		} 

		memcpy(Target, ScanLine, TgtWidth*sizeof(PIXEL));
		Target += TgtWidth;
		Source += IntPart;
		E += FractPart;
		if (E >= TgtHeight) 
		{
			E -= TgtHeight;
			Source += SrcWidth;
		}
	}

	if( skip > 0 && Source != PrevSource )
		ScaleLineAvg(ScanLine, Source, SrcWidth, TgtWidth);
	while( skip-- > 0 ) 
	{
		memcpy(Target, ScanLine, TgtWidth*sizeof(PIXEL));
		Target += TgtWidth;
	} 

	free( ScanLine );
	free( ScanLineAhead );
}

//============================================================================
//! resize rgb 24 bit image
// NOTE 1: caller must delete the returned buffer
uint8_t * VxResizeRgbImage(	uint8_t *	pu8VidData, 
						int		iImageWidthIn, 
						int		iImageHeightIn, 
						int		iResizeToWidth, 
						int		iResizeToHeight,
						int		iRotation )
{
	uint8_t * pu8DestData = new uint8_t[iResizeToWidth*3*iResizeToHeight];
	ScaleRectAvg( (PIXEL *)pu8DestData, (PIXEL *)pu8VidData, iImageWidthIn, iImageHeightIn, iResizeToWidth, iResizeToHeight );
	while( iRotation > 360 )
	{
		iRotation -= 360;
	}

	if( iRotation )
	{
		uint8_t * rotatedImage = VxRotateRgbImage(	pu8DestData, 
												iResizeToWidth, 
												iResizeToHeight, 
												iRotation );
		delete pu8DestData;
		pu8DestData = rotatedImage;
	}

	return pu8DestData;
}

//============================================================================
// NOTE 1: caller must delete the returned buffer
uint8_t * VxRotateRgbImage(	uint8_t *	pu8VidData, 
						int		iImageWidthIn, 
						int		iImageHeightIn, 
						int		iRotateAngle )
{
	uint8_t * pu8DestData = 0;
	if( 180 == iRotateAngle )
	{
		int totalBytes = iImageWidthIn * 3 * iImageHeightIn;
		pu8DestData			= new uint8_t[ totalBytes ];
		int rowStride		= iImageWidthIn * 3;
		//LogMsg( LOG_DEBUG, "VxRotateRgbImage %d total stride %d\n", iRotateAngle, rowStride );
		uint8_t * destRow		= pu8DestData;
		uint8_t * srcRow			= &pu8VidData[ totalBytes - rowStride ];
		for( int i = 0; i < iImageHeightIn; i++ )
		{
			for ( int iSrcOffs = 0; iSrcOffs < rowStride; iSrcOffs += 3 )
			{
				destRow[rowStride - (iSrcOffs + 3) ]		= srcRow[iSrcOffs];
				destRow[rowStride - ( iSrcOffs + 2) ]	= srcRow[iSrcOffs + 1];
				destRow[rowStride - (iSrcOffs + 1)]	= srcRow[iSrcOffs + 2];
			}
			
			//memcpy( destRow, srcRow, rowStride );
			destRow += rowStride;
			srcRow -= rowStride;
		}
	}
	else if( ( 90 == iRotateAngle ) || ( 270 == iRotateAngle ) )
	{
		int		pixelCnt				= iImageWidthIn * iImageHeightIn;
		int		fullRowStride			= iImageWidthIn * 3;
		int		partRowStride			= iImageHeightIn * 3;
		uint8_t *	pu8RotatedData			= new uint8_t[ pixelCnt * 3 ];
		memset( pu8RotatedData, 0, pixelCnt * 3 );
		int		blankBand				= ( iImageWidthIn - iImageHeightIn ) / 2;				
		uint8_t *	destRotData				= pu8RotatedData + blankBand * 3;
		uint8_t *	srcData					= pu8VidData + blankBand * 3;
		
		if ( 90 == iRotateAngle )
		{
			//LogMsg( LOG_DEBUG, "VxRotateRgbImage %d stride %d\n", iRotateAngle, partRowStride );
			for ( int iSrcRow = 0; iSrcRow < iImageHeightIn; iSrcRow++ )
			{
				int destRowOffs = partRowStride - 3;
				for ( int iSrcOffs = 0; iSrcOffs < partRowStride; iSrcOffs += 3 )
				{
					destRotData[destRowOffs]			= srcData[iSrcOffs];
					destRotData[( destRowOffs + 1 )]	= srcData[iSrcOffs + 1];
					destRotData[( destRowOffs + 2 )]	= srcData[iSrcOffs + 2];
					destRowOffs += fullRowStride;
				}
				
				srcData		+= fullRowStride;
				destRotData -= 3;
			}
		}
		else
		{
			//LogMsg( LOG_DEBUG, "666 VxRotateRgbImage %d stride3 %d\n", iRotateAngle, partRowStride );
			destRotData += ( pixelCnt * 3 ) - fullRowStride;
			for ( int iSrcRow = 0; iSrcRow < iImageHeightIn; iSrcRow++ )
			{
				int destRowOffs = 0;
				for ( int iSrcOffs = 0; iSrcOffs < partRowStride; iSrcOffs += 3 )
				{
					destRotData[destRowOffs]		= srcData[iSrcOffs];
					destRotData[destRowOffs + 1]	= srcData[iSrcOffs + 1];
					destRotData[destRowOffs + 2]	= srcData[iSrcOffs + 2];
					destRowOffs -= fullRowStride;
				}
				
				srcData		+= fullRowStride;
				destRotData += 3;
			}
		}

		pu8DestData = pu8RotatedData;
	}
	else
	{
		pu8DestData = new uint8_t[ iImageWidthIn * 3 * iImageHeightIn ];
		memcpy( pu8DestData, pu8VidData, iImageWidthIn * 3 * iImageHeightIn );
	}

	return pu8DestData;
}




