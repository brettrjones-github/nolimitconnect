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
// http://www.nolimitconnect.org
//============================================================================

#include <config_appcorelibs.h>
#include "AssetXferSession.h"
#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <GuiInterface/IToGui.h>

#include <CoreLib/VxGlobals.h>

//============================================================================
AssetXferSession::AssetXferSession( P2PEngine& engine )
: m_Engine( engine )
, m_FileXferInfo()
, m_iPercentComplete(0)
, m_Skt(NULL)
, m_Ident(NULL)
, m_Error( 0 )
{
	initLclSessionId();
}

//============================================================================
AssetXferSession::AssetXferSession( P2PEngine& engine, VxSktBase * sktBase, VxNetIdent * netIdent )
: m_Engine( engine )
, m_FileXferInfo()
, m_iPercentComplete(0)
, m_Skt( sktBase )
, m_Ident( netIdent )
, m_Error( 0 )
{
	initLclSessionId();
}

//============================================================================
AssetXferSession::AssetXferSession( P2PEngine& engine, VxGUID& lclSessionId, VxSktBase * sktBase, VxNetIdent * netIdent )
: m_Engine( engine )
, m_FileXferInfo( lclSessionId )
, m_iPercentComplete(0)
, m_Skt( sktBase )
, m_Ident( netIdent )
, m_Error( 0 )
{
	initLclSessionId();
}

//============================================================================
AssetXferSession::~AssetXferSession()
{
}

//============================================================================
void AssetXferSession::reset( void )
{
	m_iPercentComplete = 0;
}

//============================================================================
void AssetXferSession::initLclSessionId( void )
{
	if( false == m_FileXferInfo.getLclSessionId().isVxGUIDValid() )
	{
		m_FileXferInfo.getLclSessionId().initializeWithNewVxGUID();
	}
}

//============================================================================
bool AssetXferSession::isXferingFile( void )
{
	if( m_FileXferInfo.m_hFile )
	{
		return true;
	}

	return false;
}

//============================================================================
void AssetXferSession::setAssetStateSendBegin( void )
{
	if( eXferDirectionRx == getXferDirection() )
	{
		m_AssetInfo.setAssetSendState( eAssetSendStateRxProgress );
		m_Engine.getToGui().toGuiAssetAction( eAssetActionRxBegin, m_AssetInfo.getAssetUniqueId(), 0 );
	}
	else
	{
		m_AssetInfo.setAssetSendState( eAssetSendStateTxProgress );
		m_Engine.getToGui().toGuiAssetAction( eAssetActionTxBegin, m_AssetInfo.getAssetUniqueId(), 0 );
	}
}

//============================================================================
void AssetXferSession::setAssetStateSendCanceled( void )
{
	if( eXferDirectionRx == getXferDirection() )
	{
		m_AssetInfo.setAssetSendState( eAssetSendStateRxFail );
		m_Engine.getToGui().toGuiAssetAction( eAssetActionRxCancel, m_AssetInfo.getAssetUniqueId(), 0 );
	}
	else
	{
		m_AssetInfo.setAssetSendState( eAssetSendStateTxFail );
		m_Engine.getToGui().toGuiAssetAction( eAssetActionTxCancel, m_AssetInfo.getAssetUniqueId(), 0 );
	}
}

//============================================================================
void AssetXferSession::setAssetStateSendFail( void )
{
	if( eXferDirectionRx == getXferDirection() )
	{
		m_AssetInfo.setAssetSendState( eAssetSendStateRxFail );
		m_Engine.getToGui().toGuiAssetAction( eAssetActionRxError, m_AssetInfo.getAssetUniqueId(), 0 );
	}
	else
	{
		m_AssetInfo.setAssetSendState( eAssetSendStateTxFail );
		m_Engine.getToGui().toGuiAssetAction( eAssetActionTxError, m_AssetInfo.getAssetUniqueId(), 0 );
	}
}

//============================================================================
void AssetXferSession::setAssetStateSendProgress( int progress )
{
	if( eXferDirectionRx == getXferDirection() )
	{
		m_AssetInfo.setAssetSendState( eAssetSendStateRxProgress );
		m_Engine.getToGui().toGuiAssetAction( eAssetActionRxProgress, m_AssetInfo.getAssetUniqueId(), progress );
	}
	else
	{
		m_AssetInfo.setAssetSendState( eAssetSendStateTxProgress );
		m_Engine.getToGui().toGuiAssetAction( eAssetActionTxProgress, m_AssetInfo.getAssetUniqueId(), progress );
	}
}

//============================================================================
void AssetXferSession::setAssetStateSendSuccess( void )
{
	if( eXferDirectionRx == getXferDirection() )
	{
		m_AssetInfo.setAssetSendState( eAssetSendStateRxSuccess );
		m_Engine.getToGui().toGuiAssetAction( eAssetActionRxSuccess, m_AssetInfo.getAssetUniqueId(), 100 );
		m_Engine.getToGui().toGuiAssetAction( eAssetActionRxNotifyNewMsg, m_AssetInfo.getCreatorId(), 100 );
	}
	else
	{
		m_AssetInfo.setAssetSendState( eAssetSendStateRxSuccess );
		m_Engine.getToGui().toGuiAssetAction( eAssetActionTxSuccess, m_AssetInfo.getAssetUniqueId(), 100 );
	}
}
