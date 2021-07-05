#pragma once
//============================================================================
// Copyright (C) 2015 Brett R. Jones
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
// http://www.nolimitconnect.com
//============================================================================

#include <memory.h>

class RawVideo
{
public:
	RawVideo( 	uint32_t		fourCc,
		uint8_t *	vidData,
		uint32_t		vidDataLen, 
		int		width,
		int		height,
		int		rotation,
		bool	needToDelete )
		: m_FourCc( fourCc ) // should always be FOURCC_RGB
		, m_VidData( vidData )
		, m_VidDataLen( vidDataLen )
		, m_Width( width )
		, m_Height( height )
		, m_Rotation( rotation )
	{
		if( !needToDelete )
		{
			// need to make copy because will be deleted by caller
			uint8_t * newVidData = new uint8_t[ vidDataLen ];
			memcpy( newVidData, vidData, vidDataLen );
			m_VidData = newVidData;
		}
	}

	uint32_t							m_FourCc;
	uint8_t *						m_VidData;
	uint32_t							m_VidDataLen; 
	int							m_Width;
	int							m_Height; 
	int							m_Rotation;
};
