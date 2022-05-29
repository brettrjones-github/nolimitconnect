//============================================================================
// Copyright (C) 2013 Brett R. Jones
// Issued to MIT style license by Brett R. Jones in 2017
//
// You may use, copy, modify, merge, publish, distribute, sub-license, and/or sell this software
// provided this Copyright is not modified or removed and is included all copies or substantial portions of the Software
//
// This code is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// bjones.engineer@gmail.com
// http://www.nolimitconnect.org
//============================================================================

#include "config_corelib.h"

#include "VxDefs.h"

//============================================================================
//! blend 2 colors using alpha of src color to determine how much of src color is added to dest color
uint32_t VxBlendColors(	uint32_t u32DecalColor,	// color of upper layer ( decal or overlay )
					uint32_t u32DestColor )	// color of lower layer ( background )
{
	int32_t s32SrcAlpha	= ( u32DecalColor & 0xff000000 ) >> 24;
	// temp for debug
	if( 255 == s32SrcAlpha )
	{
		// src is completely opaque.. so it is decal color
		return u32DecalColor;
	}
	else if( 0 == s32SrcAlpha )
	{
		// src is completely transparent.. so it is dest color
		return u32DestColor;
	}

	// get componet parts
	int32_t s32SrcRed		= u32DecalColor & 0xff;
	int32_t s32SrcGreen		= (u32DecalColor & 0xff00) >> 8;
	int32_t s32SrcBlue		= (u32DecalColor & 0xff0000) >> 16;

	uint32_t u32DestAlpha	= (u32DestColor & 0xff000000) >> 24;
	int32_t s32DestRed		= u32DestColor & 0xff;
	int32_t s32DestGreen	= (u32DestColor & 0xff00) >> 8;
	int32_t s32DestBlue		= (u32DestColor & 0xff0000) >> 16;
	
	// calculate resulting alpha
	uint32_t u32ResultAlpha = (s32SrcAlpha + u32DestAlpha);
	if( u32ResultAlpha > 255 )
	{
		u32ResultAlpha = 255;
	}

	// blend the colors using alpha channel
	int32_t s32DeltaRed		= ( s32SrcRed - s32DestRed );
	int32_t s32DeltaGreen	= ( s32SrcGreen - s32DestGreen );
	int32_t s32DeltaBlue	= ( s32SrcBlue - s32DestBlue );

	uint32_t u32ResultRed = (uint32_t)( s32DestRed + (( s32DeltaRed * s32SrcAlpha)/255));
	if( u32ResultRed > 255 )
	{
		u32ResultRed = 255;
	}
	uint32_t u32ResultGreen = (uint32_t)( s32DestGreen + (( s32DeltaGreen * s32SrcAlpha)/255));
	if( u32ResultGreen > 255 )
	{
		u32ResultGreen = 255;
	}
	uint32_t u32ResultBlue = (uint32_t)( s32DestBlue + (( s32DeltaBlue * s32SrcAlpha)/255));
	if( u32ResultBlue > 255 )
	{
		u32ResultBlue = 255;
	}
	return  (u32ResultAlpha<<24) | ( u32ResultBlue << 16 ) | (u32ResultGreen << 8) | u32ResultRed;
}
