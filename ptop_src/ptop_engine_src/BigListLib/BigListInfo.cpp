//============================================================================
// Copyright (C) 2003 Brett R. Jones 
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

#include <config_appcorelibs.h>
#include <ptop_src/ptop_engine_src/BigListLib/BigListInfo.h>
#include <CoreLib/VxTimeDefs.h>
#include <memory.h>

#define ONLINE_STATUS_TIMEOUT_SECONDS	600
//we want to try at least twice before giving up a
#define NEED_PING_TIMEOUT_SECONDS	((ONLINE_STATUS_TIMEOUT_SECONDS >> 1)-120) 


//============================================================================
//=== constructor ===//
RcConnectStats::RcConnectStats()
: m_u64TimeFirstConnectAttempt(0)		// time of first attempt to connect
, m_u64TimeFirstConnectSuccess(0)		// time of first successful connect
, m_u64TimeLastConnectSuccess(0)		// time of last successful attempt to connect
, m_u32ConnectErrCnt(0)				// error count of last failed attempts
, m_u32ConnectSuccessCnt(0)			// number of times successfully connected
, m_u32ConnectRes(0)			
{
}

//============================================================================
//! return true if still is a chance we might connect
bool RcConnectStats::isLikelyToConnect( void )
{
	if( m_u32ConnectSuccessCnt && (( GetGmtTimeMs() - m_u64TimeLastConnectSuccess ) < MONTH_OF_MIILISECONDS ) )
	{
		// we have connected in last month
		return true;
	}

	if( (GetGmtTimeMs() - m_u64TimeFirstConnectAttempt ) < WEEK_OF_MIILISECONDS )
	{
		// we haven't been trying for over a week
		return true;
	}

	return false;
}

//============================================================================
uint64_t RcConnectStats::getTimeFirstConnectAttempt( void )
{
	return m_u64TimeFirstConnectAttempt;
}

//============================================================================
void RcConnectStats::setTimeLastConnectSuccess( uint64_t u32TimeSec )
{
	m_u64TimeLastConnectSuccess = u32TimeSec;
	if( 0 == m_u64TimeFirstConnectSuccess )
	{
		m_u64TimeFirstConnectSuccess = u32TimeSec;
	}
}

//============================================================================
uint64_t RcConnectStats::getTimeLastConnectSuccess( void )
{
	return m_u64TimeLastConnectSuccess;
}

//============================================================================
uint64_t RcConnectStats::getTimeFirstConnectSuccess( void )
{
	return m_u64TimeFirstConnectSuccess;
}

//============================================================================
void RcConnectStats::setConnectErrCnt( uint32_t u32ErrCnt )
{
	m_u32ConnectErrCnt = u32ErrCnt;
}

//============================================================================
uint32_t RcConnectStats::getConnectErrCnt( void )
{
	return m_u32ConnectErrCnt;
}

//============================================================================
void RcConnectStats::setConnectSuccessCnt( uint32_t u32SuccessCnt )
{
	m_u32ConnectSuccessCnt = u32SuccessCnt;
}

//============================================================================
uint32_t RcConnectStats::getConnectSuccessCnt( void )
{
	return m_u32ConnectSuccessCnt;
}

//============================================================================
int RcConnectStats::getConnectSuccessPercent( void )
{
	if( 0 == getConnectSuccessCnt() )
	{
		return 0;
	}

	int percent =  (getConnectSuccessCnt() * 100) / ( getConnectErrCnt() + getConnectSuccessCnt() );
	if( 0 == percent )
	{
		if(  getTimeLastConnectSuccess() && ( GetGmtTimeMs() - getTimeLastConnectSuccess() < MONTH_OF_MIILISECONDS ) )
		{
			// if connected in last month then always say 1 if has connected even though less than 1 percent
			percent = 1;
		}
	}

	return percent;
}

//============================================================================
void BigListInfoBase::setTimeLastConnectAttempt2( uint64_t u64TimeMs )
{
	setTimeLastConnectAttemptMs( u64TimeMs );
	if( 0 == m_u64TimeFirstConnectAttempt )
	{
		m_u64TimeFirstConnectAttempt = u64TimeMs;
	}
}

//============================================================================
void BigListInfoBase::contactWasAttempted( bool wasSuccessfull )
{
	setTimeLastConnectAttemptMs( GetGmtTimeMs() );
	if( wasSuccessfull )
	{
		setConnectSuccessCnt( getConnectSuccessCnt() + 1 );
		setTimeLastConnectAttemptMs( GetGmtTimeMs() );
	}
	else
	{
		setConnectErrCnt( getConnectErrCnt() + 1 );
	}
}

//============================================================================
BigListInfo::BigListInfo()
: m_u64TimeLastPingAttempt(0)
{ 
	// empty
}

//============================================================================
BigListInfo::~BigListInfo()
{ 
	int iCnt = (int)m_aoInQue.size();
	for( int i = 0; i < iCnt; i++ )
	{
		delete m_aoInQue[ i ];
	}

	iCnt = (int)m_aoOutQue.size();
	for( int i = 0; i < iCnt; i++ )
	{
		delete m_aoOutQue[ i ];
	}
}

//============================================================================
void BigListInfo::CopyTo( BigListInfo & oInfo )
{
	memcpy( &oInfo, this, sizeof( BigListInfoBase ) );
	//stored arrays
	//! remove existing packets from destination in que
	size_t iCnt = oInfo.m_aoInQue.size();
	for( size_t i = 0; i < iCnt; i++ )
	{
		delete oInfo.m_aoInQue[i];
	}
	oInfo.m_aoInQue.clear();
	// add packets from ours into destination
	iCnt = this->m_aoInQue.size();
	for( size_t i = 0; i < iCnt; i++ )
	{
		VxPktHdr* pktHdr = this->m_aoInQue[i]->makeCopy();
		oInfo.m_aoInQue.push_back( pktHdr );
	}

	//! remove existing packets from destination in que
	iCnt = oInfo.m_aoOutQue.size();
	for( size_t i = 0; i < iCnt; i++ )
	{
		delete  oInfo.m_aoOutQue[i];
	}

	oInfo.m_aoOutQue.clear();
	// add packets from ours into destination
	iCnt = this->m_aoOutQue.size();
	for( size_t i = 0; i < iCnt; i++ )
	{
		VxPktHdr* pktHdr = this->m_aoOutQue[i]->makeCopy();
		 oInfo.m_aoOutQue.push_back( pktHdr );
	}

	//unstored vars
	oInfo.m_u64TimeLastPingAttempt	= m_u64TimeLastPingAttempt;
}

//============================================================================
BigListInfo *	BigListInfo::makeCopy( void )
{
	BigListInfo * poBigListInfo = new BigListInfo();
	this->CopyTo( *poBigListInfo );

	return poBigListInfo;
}

//============================================================================ 
uint32_t BigListInfo::CalcStoredLen( void )
{
	int iCnt;
	int i;
	uint32_t u32Len = sizeof( BigListInfoBase );
	//calc input que length
	u32Len += sizeof( uint32_t );
	iCnt = (int)m_aoInQue.size();
	for( i = 0; i < iCnt; i++ )
	{
		u32Len += ((VxPktHdr *)m_aoInQue[i] )->getPktLength();
	}
	//calc output que length
	u32Len += sizeof( uint32_t );
	iCnt = (int)m_aoOutQue.size();
	for( i = 0; i < iCnt; i++ )
	{
		u32Len += ((VxPktHdr *)m_aoOutQue[i] )->getPktLength();
	}
	return u32Len;
}

//============================================================================
bool BigListInfo::isSafeToDelete( void )
{
	if( isIgnored() || isFriend() || m_aoInQue.size() || (m_aoOutQue.size() && ( isLikelyToConnect() ) ) )
	{
		return false;
	}

	return true;
}

//============================================================================
void BigListInfo::AddInQueItem(  VxPktHdr * poPkt )
{ 
	m_aoInQue.push_back( poPkt ); 
}

//============================================================================
VxPktHdr * BigListInfo::GetInQueItem( void )
{
	VxPktHdr * poPkt;
	size_t iCnt = m_aoInQue.size();
	if( iCnt )
	{
		poPkt = m_aoInQue[ 0 ];
		m_aoInQue.erase( m_aoInQue.begin() );
		return poPkt;
	}
	return NULL;
}

//============================================================================
void BigListInfo::AddOutQueItem( VxPktHdr * poPkt )
{ 
	m_aoOutQue.push_back( poPkt ); 
}

//============================================================================
VxPktHdr * BigListInfo::GetOutQueItem( void )
{
	VxPktHdr * pTemp;
	size_t iCnt = m_aoOutQue.size();
	if( iCnt )
	{
		pTemp =  m_aoOutQue[ 0 ];
		m_aoOutQue.erase( m_aoOutQue.begin() );
		return pTemp;
	}
	return NULL;
}
