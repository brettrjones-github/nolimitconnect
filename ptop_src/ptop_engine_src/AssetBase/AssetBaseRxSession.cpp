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

#include <config_appcorelibs.h>
#include "AssetBaseRxSession.h"

#include <CoreLib/VxFileUtil.h>

#include <stdio.h>

//============================================================================
AssetBaseRxSession::AssetBaseRxSession( P2PEngine& engine )
: AssetBaseXferSession( engine )
{
	getXferInfo().setXferDirection( eXferDirectionRx );
}

//============================================================================
AssetBaseRxSession::AssetBaseRxSession( P2PEngine& engine, VxSktBase * sktBase, VxNetIdent * netIdent )
: AssetBaseXferSession( engine, sktBase, netIdent )
{
	getXferInfo().setXferDirection( eXferDirectionRx );
}

//============================================================================
AssetBaseRxSession::AssetBaseRxSession( P2PEngine& engine, VxGUID& lclSessionId, VxSktBase * sktBase, VxNetIdent * netIdent )
: AssetBaseXferSession( engine, lclSessionId, sktBase, netIdent )
{
	getXferInfo().setXferDirection( eXferDirectionRx );
}

//============================================================================
AssetBaseRxSession::~AssetBaseRxSession()
{
}

//============================================================================
void AssetBaseRxSession::cancelDownload( VxGUID& lclSessionId )
{
	VxFileXferInfo& xferInfo = getXferInfo();
	if( xferInfo.m_hFile )
	{
		fclose( xferInfo.m_hFile );
	}

	VxFileUtil::deleteFile( xferInfo.getLclFileName().c_str() );

}
