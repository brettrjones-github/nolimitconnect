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

#include <config_gotvcore.h>
#include "OfferClientRxSession.h"

#include <CoreLib/VxFileUtil.h>

#include <stdio.h>

//============================================================================
OfferClientRxSession::OfferClientRxSession( P2PEngine& engine, OfferBaseMgr& offerMgr )
: OfferClientXferSession( engine, offerMgr )
{
	getXferInfo().setXferDirection( eXferDirectionRx );
}

//============================================================================
OfferClientRxSession::OfferClientRxSession( P2PEngine& engine, OfferBaseMgr& offerMgr, VxSktBase * sktBase, VxNetIdent * netIdent )
: OfferClientXferSession( engine, offerMgr, sktBase, netIdent )
{
	getXferInfo().setXferDirection( eXferDirectionRx );
}

//============================================================================
OfferClientRxSession::OfferClientRxSession( P2PEngine& engine, OfferBaseMgr& offerMgr, VxGUID& lclSessionId, VxSktBase * sktBase, VxNetIdent * netIdent )
: OfferClientXferSession( engine, offerMgr, lclSessionId, sktBase, netIdent )
{
	getXferInfo().setXferDirection( eXferDirectionRx );
}

//============================================================================
void OfferClientRxSession::cancelDownload( VxGUID& lclSessionId )
{
	VxFileXferInfo& xferInfo = getXferInfo();
	if( xferInfo.m_hFile )
	{
		fclose( xferInfo.m_hFile );
	}

	VxFileUtil::deleteFile( xferInfo.getLclFileName().c_str() );

}
