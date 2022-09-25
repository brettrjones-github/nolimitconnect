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
// http://www.nolimitconnect.org
//============================================================================

#include <config_appcorelibs.h>

#include "AssetBaseXferSession.h"

class AssetBaseTxSession : public AssetBaseXferSession
{
public:
	AssetBaseTxSession( P2PEngine& engine );
	AssetBaseTxSession( P2PEngine& engine, VxSktBase* sktBase, VxNetIdent* netIdent );
	AssetBaseTxSession( P2PEngine& engine, VxGUID& lclSessionId, VxSktBase* sktBase, VxNetIdent* netIdent );

	void						reset( void );
	void						cancelUpload( VxGUID& fileInstance );
	void						setQuePosition( int quePos )				{ m_QuePosition = quePos; }
	int							getQuePosition( void )						{ return m_QuePosition; }

	//=== vars ===//
	int							m_iOutstandingAckCnt; // how many receive acks are outstanding
	bool						m_bSendingPkts;
	bool						m_bViewingFileList;
	int							m_QuePosition;
	std::string					m_strOfferFile;
	std::string					m_strViewDirectory;
};
