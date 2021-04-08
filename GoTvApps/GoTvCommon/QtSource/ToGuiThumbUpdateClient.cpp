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

#include <app_precompiled_hdr.h>
#include "ToGuiThumbUpdateClient.h"

//============================================================================
ToGuiThumbUpdateClient::ToGuiThumbUpdateClient( ToGuiThumbUpdateInterface * callback )
: m_Callback( callback )
{
}

//============================================================================
ToGuiThumbUpdateClient::ToGuiThumbUpdateClient( const ToGuiThumbUpdateInterface &rhs )
{
	*this = rhs;
}

//============================================================================
ToGuiThumbUpdateClient& ToGuiThumbUpdateClient::operator =( const ToGuiThumbUpdateClient &rhs )
{
	if( this != &rhs )
	{
		m_Callback			= rhs.m_Callback;
	}

	return *this;
}

