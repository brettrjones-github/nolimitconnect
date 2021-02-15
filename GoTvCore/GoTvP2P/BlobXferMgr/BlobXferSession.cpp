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
#include "BlobXferSession.h"
#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>
#include <GoTvInterface/IToGui.h>

#include <CoreLib/VxGlobals.h>

//============================================================================
BlobXferSession::BlobXferSession( P2PEngine& engine )
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
BlobXferSession::BlobXferSession( P2PEngine& engine, VxSktBase * sktBase, VxNetIdent * netIdent )
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
BlobXferSession::BlobXferSession( P2PEngine& engine, VxGUID& lclSessionId, VxSktBase * sktBase, VxNetIdent * netIdent )
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
BlobXferSession::~BlobXferSession()
{
}

//============================================================================
void BlobXferSession::reset( void )
{
	m_iPercentComplete = 0;
}

//============================================================================
void BlobXferSession::initLclSessionId( void )
{
	if( false == m_FileXferInfo.getLclSessionId().isVxGUIDValid() )
	{
		m_FileXferInfo.getLclSessionId().initializeWithNewVxGUID();
	}
}

//============================================================================
bool BlobXferSession::isXferingFile( void )
{
	if( m_FileXferInfo.m_hFile )
	{
		return true;
	}

	return false;
}

//============================================================================
void BlobXferSession::setBlobStateSendBegin( void )
{
	if( eXferDirectionRx == getXferDirection() )
	{
		m_BlobInfo.setBlobSendState( eBlobSendStateRxProgress );
		m_Engine.getToGui().toGuiBlobAction( eBlobActionRxBegin, m_BlobInfo.getBlobUniqueId(), 0 );
	}
	else
	{
		m_BlobInfo.setBlobSendState( eBlobSendStateTxProgress );
		m_Engine.getToGui().toGuiBlobAction( eBlobActionTxBegin, m_BlobInfo.getBlobUniqueId(), 0 );
	}
}

//============================================================================
void BlobXferSession::setBlobStateSendCanceled( void )
{
	if( eXferDirectionRx == getXferDirection() )
	{
		m_BlobInfo.setBlobSendState( eBlobSendStateRxFail );
		m_Engine.getToGui().toGuiBlobAction( eBlobActionRxCancel, m_BlobInfo.getBlobUniqueId(), 0 );
	}
	else
	{
		m_BlobInfo.setBlobSendState( eBlobSendStateTxFail );
		m_Engine.getToGui().toGuiBlobAction( eBlobActionTxCancel, m_BlobInfo.getBlobUniqueId(), 0 );
	}
}

//============================================================================
void BlobXferSession::setBlobStateSendFail( void )
{
	if( eXferDirectionRx == getXferDirection() )
	{
		m_BlobInfo.setBlobSendState( eBlobSendStateRxFail );
		m_Engine.getToGui().toGuiBlobAction( eBlobActionRxError, m_BlobInfo.getBlobUniqueId(), 0 );
	}
	else
	{
		m_BlobInfo.setBlobSendState( eBlobSendStateTxFail );
		m_Engine.getToGui().toGuiBlobAction( eBlobActionTxError, m_BlobInfo.getBlobUniqueId(), 0 );
	}
}

//============================================================================
void BlobXferSession::setBlobStateSendProgress( int progress )
{
	if( eXferDirectionRx == getXferDirection() )
	{
		m_BlobInfo.setBlobSendState( eBlobSendStateRxProgress );
		m_Engine.getToGui().toGuiBlobAction( eBlobActionRxProgress, m_BlobInfo.getBlobUniqueId(), progress );
	}
	else
	{
		m_BlobInfo.setBlobSendState( eBlobSendStateTxProgress );
		m_Engine.getToGui().toGuiBlobAction( eBlobActionTxProgress, m_BlobInfo.getBlobUniqueId(), progress );
	}
}

//============================================================================
void BlobXferSession::setBlobStateSendSuccess( void )
{
	if( eXferDirectionRx == getXferDirection() )
	{
		m_BlobInfo.setBlobSendState( eBlobSendStateRxSuccess );
		m_Engine.getToGui().toGuiBlobAction( eBlobActionRxSuccess, m_BlobInfo.getBlobUniqueId(), 100 );
		m_Engine.getToGui().toGuiBlobAction( eBlobActionRxNotifyNewMsg, m_BlobInfo.getCreatorId(), 100 );
	}
	else
	{
		m_BlobInfo.setBlobSendState( eBlobSendStateRxSuccess );
		m_Engine.getToGui().toGuiBlobAction( eBlobActionTxSuccess, m_BlobInfo.getBlobUniqueId(), 100 );
	}
}
