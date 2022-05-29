#pragma once
//============================================================================
// Copyright (C) 2003 Brett R. Jones 
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

//! resize rgb 24 bit image
// NOTE 1: caller must delete the returned buffer
uint8_t * VxResizeRgbImage(	uint8_t *	pu8VidData, 
						int		iImageWidthIn, 
						int		iImageHeightIn, 
						int		iResizeToWidth, 
						int		iResizeToHeight,
						int		iRotation );

// NOTE 1: caller must delete the returned buffer
uint8_t * VxRotateRgbImage(		uint8_t *	pu8VidData, 
							int		iImageWidthIn, 
							int		iImageHeightIn, 
							int		iRotateAngle );
