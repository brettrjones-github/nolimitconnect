#pragma once
//============================================================================
// Copyright (C) 2009 Brett R. Jones 
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

//#include <GoTvDependLibrariesConfig.h>
#include <GoTvInterface/IDefs.h>

class VxSktBase;
class VxNetIdent;

//! IScan is an abstract interface for scan/search results
class IScan
{
public:

	//! error occurred during scan/search
	virtual void	onScanResultError(	EScanType			eScanType,
										VxNetIdent *		netIdent,
										VxSktBase *			sktBase,  
										uint32_t			errorCode ) = 0; 

	//! About Me Web Page scan pictures success result
	virtual void	onScanResultProfilePic(	VxNetIdent *	netIdent, 
											VxSktBase *		sktBase, 
											uint8_t *		pu8JpgData, 
											uint32_t		u32JpgDataLen ) = 0;
};
