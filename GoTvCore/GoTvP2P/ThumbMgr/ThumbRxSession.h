#pragma once
//============================================================================
// Copyright (C) 2010 Brett R. Jones
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

#include <config_gotvcore.h>

#include "ThumbXferSession.h"

class ThumbRxSession : public ThumbXferSession
{
public:
	ThumbRxSession( P2PEngine& engine );
	ThumbRxSession( P2PEngine& engine, VxSktBase * sktBase, VxNetIdent * netIdent );
	ThumbRxSession( P2PEngine& engine, VxGUID& lclSessionId, VxSktBase * sktBase, VxNetIdent * netIdent );
	virtual ~ThumbRxSession();

	void cancelDownload( VxGUID& lclSessionId );

};