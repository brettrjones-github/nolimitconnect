#ifndef VX_SKT_WAITING_LIST_H
#define VX_SKT_WAITING_LIST_H

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

#include <CoreLib/VxMutex.h>

#include <vector>

class VxSktBase;
class VxPktHdr;

class IVxSktWaitingCallback
{
public:
	virtual	void					onSktWaitExpired(	VxSktBase *		sktBase, 
														uint32_t				u32WaitReason, 
														VxPktHdr*		poPkt,
														void *			pvWaitInstance,
														bool			bConnectionLost ) = 0;
};

class VxSktWaitReason
{
public:
	VxSktWaitReason();
	VxSktWaitReason( VxSktBase* sktBase, uint32_t waitReason, uint64_t timeExpiresSysTimeMs, VxPktHdr* poPkt, void * pvWaitInstance );
	VxSktWaitReason( const VxSktWaitReason& rhs );
	virtual ~VxSktWaitReason();

	VxSktWaitReason& operator =( const VxSktWaitReason& rhs );

	VxSktBase *				m_Skt;
	VxPktHdr*				m_Pkt;
	uint64_t						m_u64TimeExpires; 
	uint32_t						m_u32WaitReason;
	void *					m_pvWaitInstance;
};

class VxSktWaitingList
{
public:
	VxSktWaitingList();
	virtual ~VxSktWaitingList();

	void						setSktWaitCallback( IVxSktWaitingCallback * sktWaitCallback );

	virtual void				onOncePerSecond( void );		
	virtual void				onConnectionLost( VxSktBase* sktBase );	

	virtual void				addWaiting(		VxSktBase *		sktBase, 
												uint32_t				u32WaitReason, 
												uint64_t				u64TimeExpiresSysTimeMs, 
												VxPktHdr*		poPkt = 0, 
												void *			pvWaitInstance = 0 );		
	virtual void				removeWaiting( VxSktBase* sktBase, uint32_t waitReason, void * pvWaitInstance = 0 );		
	virtual void				clearAllWaiting( void );

protected:
	std::vector<VxSktWaitReason *>	m_SktWaitList;
	VxMutex							m_SktWaitListMutex;
	IVxSktWaitingCallback *			m_SktWaitCallback;
};

#endif // VX_SKT_WAITING_LIST_H
