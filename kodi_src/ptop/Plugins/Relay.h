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
// http://www.nolimitconnect.com
//============================================================================


// forward declare
class PluginBaseRelay;
class VxSktBase;
class VxPktHdr;

class Relay 
{
public:
	Relay();
	virtual ~Relay();

	void						setPlugin( PluginBaseRelay * poPlugin ) { m_Plugin = poPlugin; }

	virtual void				onPktRelaySessionReq	( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent ) = 0;
	virtual void				onPktRelaySessionReply	( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent ) = 0;
	virtual void				onPktRelayConnect		( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent ) = 0;
	virtual void				onPktRelayDisconnect	( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent ) = 0;

	//=== constants ===//
	typedef long (Relay::*PROXY_PKT_FUNCTION)( VxSktBase *, VxPktHdr *, VxNetIdent * );  

	//=== vars ====//
	int							m_iMaxRelayPktType;
	PROXY_PKT_FUNCTION *		m_paRelayPktFuncTable;
	PluginBaseRelay *			m_Plugin;
};
