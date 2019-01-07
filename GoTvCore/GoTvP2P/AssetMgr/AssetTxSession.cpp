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
// brett.jones@engineer.com
// http://www.gotvptop.net
//============================================================================

#include <config_gotvcore.h>
#include "AssetTxSession.h"

#include <CoreLib/VxFileUtil.h>

#include <stdio.h>

//============================================================================
AssetTxSession::AssetTxSession( P2PEngine& engine )
: AssetXferSession( engine )
, m_iOutstandingAckCnt( 0 )
, m_bSendingPkts( false )
, m_bViewingFileList( false )
, m_QuePosition( 0 )
{
	setXferDirection( eXferDirectionTx );
}

//============================================================================
AssetTxSession::AssetTxSession( P2PEngine& engine, VxSktBase * sktBase, VxNetIdent * netIdent )
: AssetXferSession( engine, sktBase, netIdent )
, m_iOutstandingAckCnt( 0 )
, m_bSendingPkts( false )
, m_bViewingFileList( false )
, m_QuePosition( 0 )
{
	setXferDirection( eXferDirectionTx );
}

//============================================================================
AssetTxSession::AssetTxSession( P2PEngine& engine, VxGUID& lclSessionId, VxSktBase * sktBase, VxNetIdent * netIdent )
: AssetXferSession( engine, lclSessionId, sktBase, netIdent )
, m_iOutstandingAckCnt(0)
, m_bSendingPkts(false)
, m_bViewingFileList(false)
, m_QuePosition( 0 )
{
	setXferDirection( eXferDirectionTx );
}

//============================================================================
void AssetTxSession::reset( void )
{
	AssetXferSession::reset();
	m_iOutstandingAckCnt = 0;
	m_bSendingPkts = false;
	m_bViewingFileList = false;
	m_strOfferFile = "";
	m_strViewDirectory = "";
}

//============================================================================
void AssetTxSession::cancelUpload( VxGUID& lclSessionId )
{
	if( m_FileXferInfo.m_hFile )
	{
		fclose( m_FileXferInfo.m_hFile );
	}

	setAssetStateSendFail();
}
