#pragma once
//============================================================================
// Copyright (C) 2009 Brett R. Jones 
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

#include "VxSktThrottle.h"
#include "VxSktBaseMgr.h"

#include <CoreLib/VxThread.h>
#include <vector>

// from header in case not defined
#ifndef FD_SETSIZE
#define FD_SETSIZE      64
#endif /* FD_SETSIZE */

// implements a manager to manage accept sockets
class VxServerMgr : public VxSktBaseMgr 
{
public:
	VxServerMgr();
	virtual ~VxServerMgr();

	// overrides SktMgrBase
	virtual void				sktMgrShutdown( void );

	// overrides SktMgrBase
	//! make a new socket... give derived classes a chance to override
	virtual VxSktBase *			makeNewAcceptSkt( void );

	virtual bool				isReadyToAcceptConnections( void )			{ return m_IsReadyToAcceptConnections; }
    virtual bool				isListening( void );
	virtual uint16_t			getListenPort( void )						{ return m_u16ListenPort; }
	virtual InetAddrAndPort		getLocalIp( void )							{ return m_LclIp; }
	virtual void				fromGuiKickWatchdog( void );
	virtual bool				checkWatchdog( void );

    virtual bool				startListening( uint16_t u16ListenPort, const char* ip = nullptr );
    virtual RCODE				stopListening( void );
    virtual bool				restartListening( void );

    // old attempts to make listen for incomming connection stable.. just for reference
    virtual bool                startListeningOldAttepts( uint16_t u16ListenPort, const char* ip );
    virtual bool                startListeningNoBind( uint16_t u16ListenPort );

	void						listenForConnectionsToAccept( VxThread* poVxThread );
    bool                        createNewListenSocket( uint16_t listenPort, SOCKET& retListenSock );

public:
    VxThread					m_ListenVxThread;		// thread to listen for incoming connections
    int							m_iMgrId = 0;									// unique id for this manager

protected:
    bool                        createListenSocket( std::string &preferredIp, SOCKET &listenSock, uint16_t listenPort, std::string &actualListenLocalIp );
    RCODE						internalStartListen( void );
    RCODE 						acceptConnection( VxThread* poVxThread, SOCKET oListenSkt );
    void                        closeListenSocket( void );

    static constexpr size_t     m_u32MaxConnections = 10000;
    static int					m_iAcceptMgrCnt;				    // number of managers created
    RCODE						m_rcLastError = 0;					// last error that occurred

    uint16_t					m_u16ListenPort = 0;				// what port to listen on
    SOCKET						m_aoListenSkts[ FD_SETSIZE ];	    // array of listen sockets
    int							m_iActiveListenSktCnt = 0;			// number of active listening sockets
	InetAddrAndPort				m_LclIp;
	int64_t						m_LastWatchdogKickMs = 0;
    bool						m_IsReadyToAcceptConnections{ false };
    VxMutex                     m_ListenMutex;
    bool                        m_ListenSktIsBoundToIp{ false };
    bool                        m_IsAndroidOs{ false };
    bool                        m_RestartListen{ false };
};

