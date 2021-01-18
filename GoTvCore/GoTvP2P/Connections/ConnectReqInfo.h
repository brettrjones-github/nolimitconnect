#pragma once
//============================================================================
// Copyright (C) 2021 Brett R. Jones
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

#include <PktLib/VxConnectInfo.h>

#include <GoTvInterface/IDefs.h>

class ConnectReqInfo
{
public:
	ConnectReqInfo( EConnectReason eConnectReason = eConnectReasonAnnouncePing );
	ConnectReqInfo( VxConnectInfo& connectInfo, EConnectReason eConnectReason = eConnectReasonAnnouncePing );
	ConnectReqInfo( const ConnectReqInfo& rhs );

	ConnectReqInfo& operator =( const ConnectReqInfo& rhs );

	VxConnectInfo&				getConnectInfo( void )				{ return m_ConnectInfo; }
	VxGUID&						getMyOnlineId()						{ return m_ConnectInfo.getMyOnlineId(); }

	void						setConnectReason( EConnectReason eConnectReason );
	EConnectReason				getConnectReason( void );

	void						setTimeLastConnectAttemptMs( uint64_t u32TimeSec );
	uint64_t					getTimeLastConnectAttemptMs( void );
	bool						isTooSoonToAttemptConnectAgain( void );

protected:
	//=== vars ===//
	VxConnectInfo				m_ConnectInfo;
	EConnectReason				m_eConnectReason;
};


