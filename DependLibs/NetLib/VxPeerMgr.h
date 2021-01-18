#pragma once
//============================================================================
// Copyright (C) 2013 Brett R. Jones
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

#include "VxServerMgr.h"
#include "VxClientMgr.h"

class VxSktConnect;
class VxGUID;
class VxPktHdr;

class VxPeerMgr : public VxServerMgr
{
public:
	VxPeerMgr();
	virtual ~VxPeerMgr();

    virtual void				sktMgrShutdown( void ) override;

    /// if skt exists in connection list then lock access to connection list
    virtual bool				lockSkt( VxSktBase* sktBase ) override;
    virtual void				unlockSkt( VxSktBase* sktBase ) override;

	VxClientMgr&				getClientMgr( void )			{ return m_ClientMgr; }

    virtual void				setReceiveCallback( VX_SKT_CALLBACK pfnReceive, void * pvUserData ) override;
	void						setLocalIp( InetAddress& newLocalIp );

    virtual VxSktBase *			makeNewSkt( void ) override;

	virtual	void				handleSktCallback( VxSktBase * sktBase );

	//! Connect to ip or URL and return socket.. if cannot connect return NULL
	virtual VxSktConnect *		connectTo(	const char *	pIpOrUrl,						// remote ip or url 
											uint16_t		u16Port,						// port to connect to
											int				iTimeoutMilliSeconds = 1000 );	// seconds before connect attempt times out
	virtual VxSktConnect *		createConnectionUsingSocket( SOCKET skt, const char * rmtIp, uint16_t port );

	virtual bool				txPacket(	VxSktBase *			sktBase,
											VxGUID&				destOnlineId,			// online id of destination user
											VxPktHdr *			pktHdr, 				// packet to send
											bool				bDisconnect = false );	// if true disconnect after send
	virtual bool				txPacketWithDestId(	VxSktBase *		sktBase,
													VxPktHdr *		pktHdr, 				// packet to send
													bool			bDisconnect = false );	// if true disconnect after send

protected:
	VxClientMgr					m_ClientMgr;
};
