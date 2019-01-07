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
// brett.jones@engineer.com
// http://www.gotvptop.net
//============================================================================
#include "VxBaseSktInfo.h"

//============================================================================
VxBaseSktInfo::VxBaseSktInfo()
{
}

//============================================================================
VxBaseSktInfo::~VxBaseSktInfo()
{
}

//============================================================================
//! return true if file is allready qued to be sent or has been sent
bool VxBaseSktInfo::isFileQuedOrSent( const char * pFileName )
{
	return false;
}

//============================================================================
//! que a file to send
void VxBaseSktInfo::queFileToSend( const char * pLclFileName, const char * pRmtFileName )
{
}

