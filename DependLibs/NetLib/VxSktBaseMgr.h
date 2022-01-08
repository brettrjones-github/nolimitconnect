#pragma once
//============================================================================
// Copyright (C) 2008 Brett R. Jones 
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

#include "VxSktThrottle.h"
#include "VxSktDefs.h"
#include "InetAddress.h"

#include <CoreLib/VxMutex.h>

#include <vector>
#include <string>

#define SKT_ALIVE_TIMEOUT (2 * 60 * 1000)

class VxSktBase;

// implements a manager to manage multiple sockets
// NOTE: applications should use VxServerMgr or ConnnectMgr
class VxSktBaseMgr : public VxSktThrottle
{
public:
	VxSktBaseMgr();
	
	virtual ~VxSktBaseMgr();
	virtual void				setRxCallbackUserData( void * pvData )			{ m_pvRxCallbackUserData = pvData; }
	virtual void *				getRxCallbackUserData( void )					{ return m_pvRxCallbackUserData; }
	virtual void				setTxCallbackUserData( void * pvData )			{ m_pvTxCallbackUserData = pvData; }
	virtual void *				getTxCallbackUserData( void )					{ return m_pvTxCallbackUserData; }

	virtual void				sktMgrSetLocalIp( InetAddress& oLclIp );
	virtual void				sktMgrShutdown( void );


	//! make a new socket... give derived classes a chance to override
	virtual VxSktBase *			makeNewSkt( void );
	//! make a new socket... give derived classes a chance to override
	virtual VxSktBase *			makeNewAcceptSkt( void );


	//! User must Set Receive Callback
	virtual void				setReceiveCallback( VX_SKT_CALLBACK pfnReceive, void * pvUserData = NULL );
	virtual void				handleSktCloseEvent( VxSktBase * sktBase );
	
	//! Send to all connections.. if crypto is set send encrypted else send raw
	virtual void				sendToAll(	char * pvData,			// data to send
											int iDataLen );			// length of data

	//! get number of connected sockets
	virtual int					getConnectedCount( void );
	//! get total number of sockets ( includes not connected sockets )
	virtual int					getSocketCount( void ){ return (int)m_aoSkts.size(); }

	//! close all sockets
	virtual void				closeAllSkts( void );

	//=== functions that should only be called by derived classes ===//
	//! handle callbacks from sockets
	virtual	void				doReceiveCallback( VxSktBase * sktBase );
	//! handle transmit callbacks from sockets
	virtual	void				doTransmitCallback( VxSktBase * sktBase );
	//! lock access from other threads
	virtual void				sktBaseMgrLock( void );
	//! unlock access from other threads
	virtual void				sktBaseMgrUnlock( void );
	//! add a new socket to manage
	virtual void				addSkt( VxSktBase * sktBase );
	//! remove a socket from management
	virtual RCODE				removeSkt(	VxSktBase *	sktBase,				// skt to remove
											bool		bDelete = true );	    // if true delete the skt
	virtual bool				isSktActive( VxSktBase * sktBase );

    //! move to erase/delete when safe to do so
    virtual void				moveToEraseList( VxSktBase * sktBase );

    /// if skt exists in connection list then lock access to connection list
    virtual bool				lockSkt( VxSktBase* sktBase );
    virtual void				unlockSkt( VxSktBase* sktBase );

    virtual void                dumpSocketStats( const char* reason = nullptr, bool fullDump = false );

    virtual void                setSktLoopback( VxSktBase * sktLoopback )       { m_SktLoopback = sktLoopback; }

	//=== vars ===//
	RCODE						m_rcLastError{ 0 };
    ESktMgrType					m_eSktMgrType{ eSktMgrTypeNone };   // type of sockets we manage
	std::vector<VxSktBase *>	m_aoSkts;					        // array of sockets to manage
	std::vector<VxSktBase *>	m_aoSktsToDelete;			        // skts that will be deleted after 10 sec 
	VxMutex						m_SktMgrMutex;			            // thread mutex

	VX_SKT_CALLBACK				m_pfnUserReceive{ nullptr };		// receive function must be set by user
	VX_SKT_CALLBACK				m_pfnOurReceive{ nullptr };		    // our receive function to receive Socket states etc
	VX_SKT_CALLBACK				m_pfnUserTransmit{ nullptr };		// receive function may be set by user
	VX_SKT_CALLBACK				m_pfnOurTransmit{ nullptr };		// our transmit function to get Socket transmit stats
	void *						m_pvRxCallbackUserData{ nullptr };	// user defined rx callback data
	void *						m_pvTxCallbackUserData{ nullptr };	// user defined tx callback data

	UINT						m_uiCreatorVxThreadId{ 0 };		    // thread id of thread that created this object

	InetAddress					m_LclIp;
    uint32_t                    m_LockRequestCnt{ 0 };

protected:
    //! delete sockets that have expired
    virtual void                doSktDeleteCleanup( void );
	virtual void				deleteAllSockets( void );

    VxSktBase *                 m_SktLoopback{ nullptr };	        
};

