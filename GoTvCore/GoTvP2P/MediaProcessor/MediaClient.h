#pragma once
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

#include <GuiInterface/IFromGui.h>
#include <CoreLib/MediaCallbackInterface.h>

class MediaClient
{
public:
	MediaClient(	EMediaInputType				mediaType, 
					MediaCallbackInterface *	callback, 
					void *						userData )
	: m_MediaInputType( mediaType )
	, m_Callback( callback )
	, m_UserData( userData )
	{
	}

	MediaClient( const MediaClient &rhs )
	{
		*this = rhs;
	}

	MediaClient&				operator =( const MediaClient &rhs )
	{
		if( this != &rhs )
		{
			m_MediaInputType	= rhs.m_MediaInputType;
			m_Callback			= rhs.m_Callback;
			m_UserData			= rhs.m_UserData;
		}

		return *this;
	}

	EMediaInputType				m_MediaInputType;
	MediaCallbackInterface *	m_Callback; 
	void *						m_UserData;
};
