//============================================================================
// Copyright (C) 2021 Brett R. Jones
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
#include "OfferHostRxSession.h"

#include <CoreLib/VxFileUtil.h>

#include <stdio.h>

//============================================================================
OfferHostRxSession::OfferHostRxSession( P2PEngine& engine, OfferBaseMgr& offerMgr )
: OfferHostXferSession( engine, offerMgr )
{
	getXferInfo().setXferDirection( eXferDirectionRx );
}

//============================================================================
OfferHostRxSession::OfferHostRxSession( P2PEngine& engine, OfferBaseMgr& offerMgr, VxSktBase * sktBase, VxNetIdent * netIdent )
: OfferHostXferSession( engine, offerMgr, sktBase, netIdent )
{
	getXferInfo().setXferDirection( eXferDirectionRx );
}

//============================================================================
OfferHostRxSession::OfferHostRxSession( P2PEngine& engine, OfferBaseMgr& offerMgr, VxGUID& lclSessionId, VxSktBase * sktBase, VxNetIdent * netIdent )
: OfferHostXferSession( engine, offerMgr, lclSessionId, sktBase, netIdent )
{
	getXferInfo().setXferDirection( eXferDirectionRx );
}

//============================================================================
OfferHostRxSession::~OfferHostRxSession()
{
}

//============================================================================
void OfferHostRxSession::cancelDownload( VxGUID& lclSessionId )
{
	VxFileXferInfo& xferInfo = getXferInfo();
	if( xferInfo.m_hFile )
	{
		fclose( xferInfo.m_hFile );
	}

	VxFileUtil::deleteFile( xferInfo.getLclFileName().c_str() );

}
