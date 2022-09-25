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

#include "VxSktWaitingList.h"

#include <PktLib/VxPktHdr.h>

#include <CoreLib/VxGlobals.h>

//============================================================================
VxSktWaitReason::VxSktWaitReason()
: m_Skt(0)
, m_Pkt(0)
, m_u64TimeExpires(0)
, m_u32WaitReason(0)
{
}

//============================================================================
VxSktWaitReason::VxSktWaitReason( VxSktBase* sktBase, uint32_t u32WaitReason, uint64_t u32TimeExpiresSysTimeMs, VxPktHdr * poPkt, void * pvWaitReason )
: m_Skt(sktBase)
, m_Pkt(0)
, m_u64TimeExpires( u32TimeExpiresSysTimeMs )
, m_u32WaitReason( u32WaitReason )
, m_pvWaitInstance( pvWaitReason )
{
	if( poPkt )
	{
		m_Pkt = poPkt->makeCopy();
	}
}

//============================================================================
VxSktWaitReason::VxSktWaitReason( const VxSktWaitReason& rhs )
{
	*this = rhs;
}

//============================================================================
VxSktWaitReason::~VxSktWaitReason()
{
	delete m_Pkt;
	m_Pkt = 0;
}

//============================================================================
VxSktWaitReason& VxSktWaitReason::operator =( const VxSktWaitReason& rhs )
{
	// Check for self-assignment!
	if( this != &rhs ) 
	{
		// Not same object so copy
		m_Skt					= rhs.m_Skt;
		m_u32WaitReason			= rhs.m_u32WaitReason;
		m_u64TimeExpires		= rhs.m_u64TimeExpires;
		if( rhs.m_Pkt )
		{
			m_Pkt					= rhs.m_Pkt->makeCopy();
		}
		else
		{
			m_Pkt = NULL;
		}
		m_pvWaitInstance		= rhs.m_pvWaitInstance;
	}

	return *this;
}

//============================================================================
VxSktWaitingList::VxSktWaitingList()
: m_SktWaitCallback(0)
{
}

//============================================================================
VxSktWaitingList::~VxSktWaitingList()
{
	clearAllWaiting();
}

//============================================================================
void VxSktWaitingList::setSktWaitCallback( IVxSktWaitingCallback * sktWaitCallback )
{
	m_SktWaitCallback = sktWaitCallback;
}

//============================================================================
void VxSktWaitingList::onOncePerSecond( void )
{
	if( m_SktWaitList.size()
		&& m_SktWaitCallback )
	{
		m_SktWaitListMutex.lock();
		bool atLeastOneExpired = false;

		uint64_t sysTimeNow = GetGmtTimeMs();

		std::vector<VxSktWaitReason *>::iterator iter;
		for( iter = m_SktWaitList.begin(); iter != m_SktWaitList.end(); ++iter )
		{
			if( sysTimeNow > (*iter)->m_u64TimeExpires )
			{
				VxSktWaitReason * reason = (*iter);
				atLeastOneExpired = true;
				m_SktWaitCallback->onSktWaitExpired(	reason->m_Skt, 
														reason->m_u32WaitReason, 
														reason->m_Pkt, 
														reason->m_pvWaitInstance,
														false );
			}
		}

		if( atLeastOneExpired )
		{
			iter = m_SktWaitList.begin();
			while( iter != m_SktWaitList.end() )
			{
				if( sysTimeNow > (*iter)->m_u64TimeExpires )
				{
					delete *iter;
					iter = m_SktWaitList.erase(iter);
				}
				else
				{
					++iter;
				}
			}
		}

		m_SktWaitListMutex.unlock();
	}
}

//============================================================================
void VxSktWaitingList::onConnectionLost( VxSktBase* sktBase )
{
	if( m_SktWaitList.size() )
	{
		bool needErase = false;
		m_SktWaitListMutex.lock();
		std::vector<VxSktWaitReason *>::iterator iter;
		for( iter = m_SktWaitList.begin(); iter != m_SktWaitList.end(); ++iter )
		{
			if( sktBase == (*iter)->m_Skt )
			{
				needErase = true;
				VxSktWaitReason * reason = (*iter);
				m_SktWaitCallback->onSktWaitExpired(	reason->m_Skt, 
														reason->m_u32WaitReason,
														reason->m_Pkt, 
														reason->m_pvWaitInstance, 
														true );
			}
		}

		if( needErase )
		{
			iter = m_SktWaitList.begin();
			while( iter != m_SktWaitList.end() )
			{
				if( sktBase == (*iter)->m_Skt )
				{
					delete *iter;
					iter = m_SktWaitList.erase(iter);
				}
				else
				{
					++iter;
				}
			}
		}

		m_SktWaitListMutex.unlock();
	}
}

//============================================================================
void VxSktWaitingList::addWaiting(	VxSktBase *		sktBase, 
									uint32_t				u32WaitReason, 
									uint64_t				u64TimeExpiresSysTimeMs, 
									VxPktHdr *		poPkt, 
									void *			pvWaitInstance )
{
	m_SktWaitListMutex.lock();
	m_SktWaitList.push_back( new VxSktWaitReason(	sktBase, 
													u32WaitReason, 
													u64TimeExpiresSysTimeMs, 
													poPkt,
													pvWaitInstance ) );
	m_SktWaitListMutex.unlock();
}

//============================================================================
void VxSktWaitingList::removeWaiting( VxSktBase* sktBase, uint32_t u32WaitReason, void * pvWaitInstance )
{
	if( m_SktWaitList.size() )
	{
		std::vector<VxSktWaitReason *>::iterator iter;
		m_SktWaitListMutex.lock();
		iter = m_SktWaitList.begin();
		while( iter != m_SktWaitList.end() )
		{
			VxSktWaitReason * reason = (*iter);
			if( ( sktBase == reason->m_Skt )
				&& ( u32WaitReason == reason->m_u32WaitReason )
				&& ( pvWaitInstance == reason->m_pvWaitInstance ) )
			{
				delete *iter;
				iter = m_SktWaitList.erase(iter);
			}
			else
			{
				++iter;
			}
		}
		m_SktWaitListMutex.unlock();
	}
}

//============================================================================
void VxSktWaitingList::clearAllWaiting( void )
{
	std::vector<VxSktWaitReason *>::iterator iter;
	m_SktWaitListMutex.lock();
	iter = m_SktWaitList.begin();
	while( iter != m_SktWaitList.end() )
	{
		delete *iter;
		iter = m_SktWaitList.erase(iter);
	}
	m_SktWaitListMutex.unlock();
}




