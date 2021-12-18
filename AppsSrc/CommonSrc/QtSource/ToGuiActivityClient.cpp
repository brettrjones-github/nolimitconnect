//============================================================================
// Copyright (C) 2015 Brett R. Jones 
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


#include "ToGuiActivityClient.h"

//============================================================================
ToGuiActivityClient::ToGuiActivityClient(	ToGuiActivityInterface *	callback, 
							void *					userData )
: m_Callback( callback )
, m_UserData( userData )
{
}

//============================================================================
ToGuiActivityClient::ToGuiActivityClient( const ToGuiActivityClient &rhs )
{
	*this = rhs;
}

//============================================================================
ToGuiActivityClient& ToGuiActivityClient::operator =( const ToGuiActivityClient &rhs )
{
	if( this != &rhs )
	{
		m_Callback			= rhs.m_Callback;
		m_UserData			= rhs.m_UserData;
	}

	return *this;
}

