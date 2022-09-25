#pragma once
//============================================================================
// Copyright (C) 2014 Brett R. Jones
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

#include "PluginSessionBase.h"

class TxSession : public PluginSessionBase
{
public:
	TxSession( VxSktBase* sktBase, VxNetIdent* netIdent, EPluginType pluginType );
	TxSession( VxGUID& lclSessionId, VxSktBase* sktBase, VxNetIdent* netIdent, EPluginType pluginType );
	virtual ~TxSession();

	void						setOutstandingAckCnt( int cnt )				{ m_iOutstandingAckCnt = cnt; }
	int							getOutstandingAckCnt( void )				{ return m_iOutstandingAckCnt; }
	void						incrementOutstandingAckCnt( void )			{ m_iOutstandingAckCnt++; }
	void						decrementOutstandingAckCnt( void )			{ if( m_iOutstandingAckCnt ) m_iOutstandingAckCnt--; }

	void						setIsSendingPkts( bool isSending )			{ m_bSendingPkts = isSending; }
	int							getIsSendingPkts( void )					{ return m_bSendingPkts; }

protected:
	//=== vars ===//
	int							m_iOutstandingAckCnt; // how many receive acks are outstanding
	bool						m_bSendingPkts;
};
