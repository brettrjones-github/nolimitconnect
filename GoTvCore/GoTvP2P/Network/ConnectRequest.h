#pragma once
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
// http://www.nolimitconnect.com
//============================================================================

#include <PktLib/VxConnectInfo.h>

#include <GoTvInterface/IDefs.h>

class ConnectRequest
{
public:
	ConnectRequest( EConnectReason eConnectReason = eConnectReasonAnnouncePing );
	ConnectRequest( VxConnectInfo& connectInfo, EConnectReason eConnectReason = eConnectReasonAnnouncePing );
	ConnectRequest( const ConnectRequest& rhs );

	ConnectRequest& operator =( const ConnectRequest& rhs );

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


