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

#include "VxSktBase.h"
#include "VxSktBaseMgr.h"
#include "VxSktUtil.h"
#include "VxSktAccept.h"

#include <CoreLib/VxGlobals.h>

#include <stdio.h>
#include <memory.h>

namespace
{
	//============================================================================
	void VxSktBaseMgrReceiveFunction(  VxSktBase * sktBase, void * pvUserData )
	{
		vx_assert( sktBase );
		vx_assert( sktBase->m_SktMgr );
		sktBase->m_SktMgr->doReceiveCallback( sktBase );
	}

	////============================================================================
	//void VxSktBaseMgrTransmitFunction(  VxSktBase * sktBase, void * pvUserData )
	//{
	//	vx_assert( sktBase );
	//	vx_assert( sktBase->m_SktMgr );
	//	sktBase->m_SktMgr->doTransmitCallback( sktBase );
	//}
}

//============================================================================
VxSktBaseMgr::VxSktBaseMgr()
{
	m_pfnOurReceive = VxSktBaseMgrReceiveFunction;
	m_uiCreatorVxThreadId = VxGetCurrentThreadId();
}

//============================================================================
VxSktBaseMgr::~VxSktBaseMgr()
{
	closeAllSkts();
	deleteAllSockets();
}


//============================================================================
// find a socket.. assumes list has been locked
VxSktBase* VxSktBaseMgr::findSktBase( const VxGUID& connectId, bool acceptSktsOnly )
{
    if( connectId.isVxGUIDValid() && ( !acceptSktsOnly || eSktMgrTypeTcpAccept == m_eSktMgrType ) )
    {
        for( auto sktBase : m_aoSkts )
        {
            if( sktBase && sktBase->getSocketId() == connectId )
            {
                return sktBase;
            }
        }
    }

    return nullptr;
}

//============================================================================
void VxSktBaseMgr::sktMgrSetLocalIp( InetAddress& oLclIp )
{
	m_LclIp = oLclIp;
}


//============================================================================
void VxSktBaseMgr::sktMgrShutdown( void )
{
	closeAllSkts();
}

//============================================================================
void VxSktBaseMgr::deleteAllSockets()
{
	for( auto iter = m_aoSktsToDelete.begin(); iter != m_aoSktsToDelete.end(); ++iter )
	{
		delete (*iter);
	}

	m_aoSktsToDelete.clear();
}

//============================================================================
//! User must Set Receive Callback
void VxSktBaseMgr::setReceiveCallback( VX_SKT_CALLBACK pfnReceive, void * pvUserData )
{
	m_pfnUserReceive = pfnReceive;
	m_pvRxCallbackUserData = pvUserData;
}

//============================================================================
//! lock access from other threads
void VxSktBaseMgr::sktBaseMgrLock( void )
{
    m_SktMgrMutex.lock(__FILE__, __LINE__);
}

//============================================================================
//! unlock access from other threads
void VxSktBaseMgr::sktBaseMgrUnlock( void )
{
    m_SktMgrMutex.unlock(__FILE__, __LINE__);
}

//============================================================================
//! add a new socket to manage
void VxSktBaseMgr::addSkt( VxSktBase * sktBase )
{
	//LogMsg( LOG_INFO, "Adding %s to VxSktBaseMgr skt list\n", sktBase->describeSktType().c_str() );
	sktBaseMgrLock();
	m_aoSkts.push_back( sktBase );
	sktBaseMgrUnlock();
}

//============================================================================
//! remove a socket from management
RCODE VxSktBaseMgr::removeSkt(  VxSktBase *	sktBase,		// skt to remove
								bool		bDelete )	// if true delete the skt
{
	//LogMsg( LOG_INFO, "Removing Skt ID %d  type %s from VxSktBaseMgr skt list\n", sktBase->getSktNumber(), sktBase->describeSktType().c_str() );
	RCODE rc = -1;
	sktBaseMgrLock();
	for( auto iter = m_aoSkts.begin(); iter != m_aoSkts.end(); ++iter )
	{
		if( (*iter) == sktBase )
		{
			// found it in our list
			m_aoSkts.erase( iter );
			rc = 0;
			break;
		}
	}

	sktBaseMgrUnlock();
	if( bDelete )
	{
        LogMsg( LOG_VERBOSE, "Deleting Skt ID %d type %s from VxSktBaseMgr skt list", sktBase->getSktNumber(), sktBase->describeSktType().c_str() );
		delete sktBase;
	}

	return rc;
}

//============================================================================
bool VxSktBaseMgr::isSktActive( VxSktBase * sktBase )
{
    if( !sktBase )
    {
        LogMsg( LOG_ERROR, "VxSktBaseMgr::isSktActive null sktBase" );
        return false;
    }

	bool isActive = false;
    vx_assert( sktBase );
    if( sktBase == m_SktLoopback )
    {
        return true;
    }

	std::vector<VxSktBase *>::iterator iter;
	sktBaseMgrLock();
	for( iter = m_aoSkts.begin(); iter != m_aoSkts.end(); ++iter )
	{
		if( (*iter) == sktBase )
		{
			// found it in our list
			isActive = true;
			break;
		}
	}

	sktBaseMgrUnlock();
	return isActive;
}

//============================================================================
//! Send to all connections
void VxSktBaseMgr::sendToAll(	char * pData,			// data to send
								int iDataLen )			// length of data
{
	std::vector<VxSktBase *>::iterator iter;
	sktBaseMgrLock();
	for( iter = m_aoSkts.begin(); iter != m_aoSkts.end(); ++iter )
	{
		VxSktBase * skt = (*iter);
		if( skt->isTxCryptoKeySet() )
		{
			skt->txEncrypted( pData, iDataLen );
		}
		else
		{
			skt->sendData( pData, iDataLen );
		}
	}

	sktBaseMgrUnlock();
}

//============================================================================
//! get number of connected sockets
int VxSktBaseMgr::getConnectedCount( void )
{
	int iConnectedCnt = 0;
	std::vector<VxSktBase *>::iterator iter;
	sktBaseMgrLock();
	for( iter = m_aoSkts.begin(); iter != m_aoSkts.end(); ++iter )
	{
		if( (*iter)->isConnected() )
		{
			iConnectedCnt++;
		}
	}

	sktBaseMgrUnlock();
	return iConnectedCnt;
}

//============================================================================
//! close all sockets
void VxSktBaseMgr::closeAllSkts( void )
{
	std::vector<VxSktBase *>::iterator iter;
	sktBaseMgrLock();
	for( iter = m_aoSkts.begin(); iter != m_aoSkts.end(); ++iter )
	{
		if( (*iter)->isConnected() )
		{
			(*iter)->closeSkt(eSktCloseAll);
		}
	}

	sktBaseMgrUnlock();
}

//============================================================================
VxSktBase *	VxSktBaseMgr::makeNewSkt( void )					
{ 
	return new VxSktBase(); 
}

//============================================================================
VxSktBase * VxSktBaseMgr::makeNewAcceptSkt( void )				
{ 
	return new VxSktAccept(); 
}

//============================================================================
//! handle transmit callbacks from sockets
void VxSktBaseMgr::doTransmitCallback( VxSktBase * sktBase )
{
	TxedPkt( sktBase->m_iLastTxLen );
}

//============================================================================
void VxSktBaseMgr::doReceiveCallback( VxSktBase * sktBase )
{
	ESktCallbackReason eCallbackReason = sktBase->getCallbackReason();
	m_pfnUserReceive( sktBase, m_pvRxCallbackUserData );
	if( eSktCallbackReasonClosed == eCallbackReason )
	{
		//LogMsg( LOG_INFO, "VxSktBaseMgr::doReceiveCallback: closed %s \n", sktBase->describeSktType().c_str() );
	}
}

//============================================================================
void VxSktBaseMgr::handleSktCloseEvent( VxSktBase * sktBase )
{
	//LogMsg( LOG_INFO, "VxSktBaseMgr::handleSktCloseEvent: for skt %d 0x%x \n", sktBase->m_SktNumber, sktBase );
    sktBaseMgrLock();
    uint64_t timeNow = GetTimeStampMs();
    for( VxSktBase* sktBase : m_aoSkts )
    {
        if( sktBase )
        {
            if( timeNow - sktBase->getLastActiveTimeMs() > SKT_ALIVE_TIMEOUT )
            {
                LogModule( eLogSkt, LOG_DEBUG, "Closing due to alive timeout %s skt %d handle %d", DescribeSktType( sktBase->getSktType() ), sktBase->getSktNumber(), sktBase->getSktHandle() );
                sktBase->dumpSocketStats();

                sktBase->closeSkt( eSktCloseImAliveTimeout );
            }
        }
    }

	sktBaseMgrUnlock();

    // put this skt in delete list to be deleted later
    moveToEraseList( sktBase );
    doSktDeleteCleanup();
}

//============================================================================
//! delete sockets that have expired
void VxSktBaseMgr::doSktDeleteCleanup()
{
    int64_t timeNowMs = GetGmtTimeMs();
    std::vector<VxSktBase *> deleteSktList;

    bool deletedSkt = true;
    while( deletedSkt )
    {
        VxSktBase * sktToDelete = nullptr;
        deletedSkt = false;
        sktBaseMgrLock();
        std::vector<VxSktBase *>::iterator iter = m_aoSktsToDelete.begin();
        // to be deleted sockets delete after 10 seconds
        while( iter != m_aoSktsToDelete.end() )
        {
            sktToDelete = ( *iter );
            if( timeNowMs > sktToDelete->getToDeleteTimeMs() )
            {
                if( sktToDelete->getInUseByRxThread() )
                {
                    LogMsg( LOG_ERROR, "socket id %d handle %d type %s still in use by thread after delete timeout %s", sktToDelete->getSktNumber(), 
                        sktToDelete->getSktHandle(),
                        DescribeSktType( sktToDelete->getSktType() ),
                        sktToDelete->describeSktConnection().c_str() );

                    if( INVALID_SOCKET != sktToDelete->getSktHandle() )
                    {
                        sktToDelete->doCloseThisSocketHandle( false );
                    }

                    ++iter;
                    break;
                }
                else
                {
                    deleteSktList.push_back( sktToDelete );
                    iter = m_aoSktsToDelete.erase( iter );
                    deletedSkt = true;
                    break;
                }
            }
            else
            {
                ++iter;
            }
        }

        sktBaseMgrUnlock();
    }   

    for( auto sktBase : deleteSktList )
    {
        LogModule( eLogConnect, LOG_VERBOSE, "deleting skt %s", sktBase->describeSktConnection().c_str() );
        delete sktBase;
    }
}

//============================================================================
//! move to erase/delete when safe to do so
void VxSktBaseMgr::moveToEraseList( VxSktBase * sktBase )
{
    m_SktMgrMutex.lock( __FILE__, __LINE__ ); // dont let other threads mess with array while we remove the socket
    for( auto iter = m_aoSkts.begin(); iter != m_aoSkts.end(); ++iter )
    {
        if( *iter == sktBase )
        {
            m_aoSkts.erase( iter );
            break;
        }
    }

    sktBase->setToDeleteTimeMs( GetGmtTimeMs() + 30000 );
    m_aoSktsToDelete.push_back( sktBase );
    m_SktMgrMutex.unlock( __FILE__, __LINE__ );
}

//============================================================================
void VxSktBaseMgr::dumpSocketStats( const char* reason, bool fullDump )
{
    std::string reasonMsg = reason ? reason : "";
    LogModule( eLogSkt, LOG_DEBUG, "%s skt active %d to delete %d total in system %d", reasonMsg.c_str(), m_aoSkts.size(), m_aoSktsToDelete.size(), VxSktBase::getCurrentSktCount() );
    if( fullDump )
    {
        int sktCnt = 0;
        uint64_t timeNow = GetTimeStampMs();
        m_SktMgrMutex.lock( __FILE__, __LINE__ ); 
        for( VxSktBase* sktBase : m_aoSkts )
        {
            sktCnt++;
            if( sktBase )
            {
                std::string dmpReason = std::to_string( sktCnt ) + " - ";
                if( timeNow - sktBase->getLastActiveTimeMs() > SKT_ALIVE_TIMEOUT )
                {
                    sktBase->dumpSocketStats( dmpReason.c_str(), fullDump );
                }
            }
        }

        m_SktMgrMutex.unlock( __FILE__, __LINE__ );
    }
}

//============================================================================
bool VxSktBaseMgr::closeConnection( VxGUID& connectId, ESktCloseReason closeReason )
{
	bool wasClosed{ false };
	if( connectId.isVxGUIDValid() )
	{
		VxSktBase* sktBase{ nullptr };
		sktBaseMgrLock();
		for( auto iter = m_aoSkts.begin(); iter != m_aoSkts.end(); ++iter )
		{
			if( ( *iter )->getSocketId() == connectId )
			{
				sktBase = (*iter);
				break;
			}
		}

		sktBaseMgrUnlock();
		if( sktBase )
		{
			sktBase->closeSkt( closeReason, true );
			wasClosed = true;
		}
	}

	return wasClosed;
}
