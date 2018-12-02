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
// brett.jones@engineer.com
// http://www.gotvptop.net
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
: m_rcLastError( 0 )
, m_eSktMgrType( eSktMgrTypeNone )
, m_pfnUserReceive( 0 )
, m_pfnOurReceive( 0 )
, m_pfnUserTransmit( 0 )
, m_pfnOurTransmit( 0 )
, m_pvRxCallbackUserData( 0 )
, m_pvTxCallbackUserData( 0 )
, m_u32MaxConnections( 10000 )
, m_uiCreatorVxThreadId( 0 )
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
	std::vector<VxSktBase *>::iterator iter;
	for( iter = m_aoSktsToDelete.begin(); iter != m_aoSktsToDelete.end(); ++iter )
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
	//LogMsg( LOG_INFO, "Removing Skt ID %d  type %s from VxSktBaseMgr skt list\n", sktBase->getSktId(), sktBase->describeSktType().c_str() );
	RCODE rc = -1;
	std::vector<VxSktBase *>::iterator iter;
	sktBaseMgrLock();
	for( iter = m_aoSkts.begin(); iter != m_aoSkts.end(); ++iter )
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
		LogMsg( LOG_INFO, "Deleting Skt ID %d  type %s from VxSktBaseMgr skt list\n", sktBase->getSktId(), sktBase->describeSktType().c_str() );
		delete sktBase;
	}

	return rc;
}

//============================================================================
bool VxSktBaseMgr::isSktActive( VxSktBase * sktBase )
{
	bool isActive = false;
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
			(*iter)->closeSkt();
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
	//LogMsg( LOG_INFO, "VxSktBaseMgr::handleSktCloseEvent: for skt %d 0x%x \n", sktBase->m_iSktId, sktBase );
	uint64_t timeNowMs = GetGmtTimeMs();
	bool deletedSkt = true;
	while( deletedSkt )
	{
		VxSktBase * sktToDelete = NULL;
		deletedSkt = false;
		sktBaseMgrLock();
		std::vector<VxSktBase *>::iterator iter = m_aoSktsToDelete.begin();
		// to be deleted sockets delete after 10 seconds
		while( iter != m_aoSktsToDelete.end() )
		{
			sktToDelete = (*iter);
			if( timeNowMs > sktToDelete->m_ToDeleteTimeGmtMs )
			{
				iter = m_aoSktsToDelete.erase( iter );
				//LogMsg( LOG_INFO, "deleting skt %d\n", sktToDelete->m_iSktId );
				deletedSkt = true;
				break;
			}
			else
			{
				++iter;
			}
		}

		sktBaseMgrUnlock();
		if( deletedSkt )
		{
			//LogMsg( LOG_INFO, "VxSktBaseMgr::handleSktCloseEvent: deleting skt %s \n", sktToDelete->describeSktType().c_str() );
			delete sktToDelete;
		}
	}

	sktBaseMgrLock();
	// put this skt in delete list to be deleted later
	std::vector<VxSktBase *>::iterator activeSktIter = m_aoSkts.begin();
	while( activeSktIter != m_aoSkts.end() )
	{
		VxSktBase * thisSkt = (*activeSktIter);
		if( ( thisSkt == sktBase )
			|| ( false == thisSkt->isConnected() ) )
		{
			//if( !thisSkt->isUdpSocket() )
			//{
			//	LogMsg( LOG_INFO, "moving skt %s to delete list\n", thisSkt->describeSktType().c_str() );
			//}

			thisSkt->m_ToDeleteTimeGmtMs = timeNowMs + 30000;
			m_aoSktsToDelete.push_back( thisSkt );
			activeSktIter = m_aoSkts.erase( activeSktIter );
		}
		else
		{
			++activeSktIter;
		}
	}

	sktBaseMgrUnlock();
}

