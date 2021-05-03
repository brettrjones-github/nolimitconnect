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

#include "RelaySession.h"

#include <NetLib/VxSktBase.h>

//============================================================================
RelaySession::RelaySession( VxSktBase * sktBase, VxNetIdent * netIdent, EPluginType pluginType )
: PluginSessionBase( sktBase, netIdent, pluginType )
{
	m_ePluginType = ePluginTypeRelay;
}

//============================================================================
RelaySession::RelaySession( VxGUID& lclSessionId, VxSktBase * sktBase, VxNetIdent * netIdent, EPluginType pluginType )
: PluginSessionBase( lclSessionId, sktBase, netIdent, pluginType )
{
	m_ePluginType = ePluginTypeRelay;
}

//============================================================================
RelaySession::~RelaySession()
{
}

//============================================================================
bool RelaySession::proxySendPkt( VxPktHdr * poPkt, VxGUID& destOnlineId, bool bDisconnectAfterSend )
{
	bool bSendSuccess = false;
	if( m_Skt->isConnected() && m_Skt->isTxEncryptionKeySet() )
	{
		poPkt->setPluginNum( (uint8_t)(m_ePluginType) );
		m_Skt->m_u8TxSeqNum++;
		poPkt->setPktSeqNum( m_Skt->m_u8TxSeqNum );
		RCODE rc = m_Skt->txPacket( destOnlineId, poPkt, bDisconnectAfterSend );
		if( 0 == rc )
		{
			bSendSuccess = true;
		}
		else
		{
			LogMsg( LOG_ERROR, "PluginBase::txPacket error %d\n", rc );
		}
	}
	else
	{
		LogMsg( LOG_ERROR, "PluginBase::txPacket error\n");
	}
	return bSendSuccess;
}


