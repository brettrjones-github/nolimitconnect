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
// http://www.nolimitconnect.org
//============================================================================

#include <config_appcorelibs.h>
#include "OfferBaseXferSession.h"
#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <GuiInterface/IToGui.h>

#include <CoreLib/VxGlobals.h>

//============================================================================
OfferBaseXferSession::OfferBaseXferSession( P2PEngine& engine, OfferBaseMgr& offerMgr )
: m_Engine( engine )
, m_OfferMgr( offerMgr )
, m_FileXferInfo()
, m_iPercentComplete(0)
, m_Skt(NULL)
, m_Ident(NULL)
, m_Error( 0 )
{
	initLclSessionId();
}

//============================================================================
OfferBaseXferSession::OfferBaseXferSession( P2PEngine& engine, OfferBaseMgr& offerMgr, VxSktBase* sktBase, VxNetIdent* netIdent )
: m_Engine( engine )
, m_OfferMgr( offerMgr )
, m_FileXferInfo()
, m_Skt( sktBase )
, m_Ident( netIdent )
{
	initLclSessionId();
}

//============================================================================
OfferBaseXferSession::OfferBaseXferSession( P2PEngine& engine, OfferBaseMgr& offerMgr, VxGUID& lclSessionId, VxSktBase* sktBase, VxNetIdent* netIdent )
: m_Engine( engine )
, m_OfferMgr( offerMgr )
, m_FileXferInfo( lclSessionId )
, m_Skt( sktBase )
, m_Ident( netIdent )
{
	initLclSessionId();
}

//============================================================================
void OfferBaseXferSession::reset( void )
{
	m_iPercentComplete = 0;
}

//============================================================================
void OfferBaseXferSession::initLclSessionId( void )
{
	if( false == m_FileXferInfo.getLclSessionId().isVxGUIDValid() )
	{
		m_FileXferInfo.getLclSessionId().initializeWithNewVxGUID();
	}
}

//============================================================================
bool OfferBaseXferSession::isXferingFile( void )
{
	if( m_FileXferInfo.m_hFile )
	{
		return true;
	}

	return false;
}

//============================================================================
void OfferBaseXferSession::setOfferBaseStateSendBegin( void )
{
	if( eXferDirectionRx == getXferDirection() )
	{
		m_OfferBaseInfo.setOfferSendState( eOfferSendStateRxProgress );
		m_OfferMgr.announceOfferAction( m_OfferBaseInfo.getOfferId(), eOfferActionRxBegin, 0 );
	}
	else
	{
		m_OfferBaseInfo.setOfferSendState( eOfferSendStateTxProgress );
		m_OfferMgr.announceOfferAction( m_OfferBaseInfo.getOfferId(), eOfferActionTxBegin, 0 );
	}
}

//============================================================================
void OfferBaseXferSession::setOfferBaseStateSendCanceled( void )
{
	if( eXferDirectionRx == getXferDirection() )
	{
		m_OfferBaseInfo.setOfferSendState( eOfferSendStateRxFail );
		m_OfferMgr.announceOfferAction( m_OfferBaseInfo.getOfferId(), eOfferActionRxCancel, 0 );
	}
	else
	{
		m_OfferBaseInfo.setOfferSendState( eOfferSendStateTxFail );
		m_OfferMgr.announceOfferAction( m_OfferBaseInfo.getOfferId(), eOfferActionTxCancel, 0 );
	}
}

//============================================================================
void OfferBaseXferSession::setOfferBaseStateSendFail( void )
{
	if( eXferDirectionRx == getXferDirection() )
	{
		m_OfferBaseInfo.setOfferSendState( eOfferSendStateRxFail );
		m_OfferMgr.announceOfferAction( m_OfferBaseInfo.getOfferId(), eOfferActionRxError, 0 );
	}
	else
	{
		m_OfferBaseInfo.setOfferSendState( eOfferSendStateTxFail );
		m_OfferMgr.announceOfferAction( m_OfferBaseInfo.getOfferId(), eOfferActionTxError, 0 );
	}
}

//============================================================================
void OfferBaseXferSession::setOfferBaseStateSendProgress( int progress )
{
	if( eXferDirectionRx == getXferDirection() )
	{
		m_OfferBaseInfo.setOfferSendState( eOfferSendStateRxProgress );
		m_OfferMgr.announceOfferAction( m_OfferBaseInfo.getOfferId(), eOfferActionRxProgress, progress );
	}
	else
	{
		m_OfferBaseInfo.setOfferSendState( eOfferSendStateTxProgress );
		m_OfferMgr.announceOfferAction( m_OfferBaseInfo.getOfferId(), eOfferActionTxProgress, progress );
	}
}

//============================================================================
void OfferBaseXferSession::setOfferBaseStateSendSuccess( void )
{
	if( eXferDirectionRx == getXferDirection() )
	{
		m_OfferBaseInfo.setOfferSendState( eOfferSendStateRxSuccess );
		m_OfferMgr.announceOfferAction( m_OfferBaseInfo.getOfferId(), eOfferActionRxSuccess, 100 );
		m_OfferMgr.announceOfferAction( m_OfferBaseInfo.getCreatorId(), eOfferActionRxNotifyNewMsg, 100 );
	}
	else
	{
		m_OfferBaseInfo.setOfferSendState( eOfferSendStateRxSuccess );
		m_OfferMgr.announceOfferAction( m_OfferBaseInfo.getOfferId(), eOfferActionTxSuccess, 100 );
	}
}
