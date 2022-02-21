#pragma once
//============================================================================
// Copyright (C) 2017 Brett R. Jones 
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

#include <CoreLib/VxGUID.h>
#include <GuiInterface/IDefs.h>

class VxPtopUrl
{
public:
	VxPtopUrl() = default;
	VxPtopUrl( std::string& url );
	VxPtopUrl( const char* url );
    VxPtopUrl( const VxPtopUrl& rhs );

    VxPtopUrl&                  operator = ( const VxPtopUrl& rhs );
    bool                        operator == ( const VxPtopUrl& rhs ) const;

	bool						isValid( void );

	void						setUrl( std::string& url );
	void						setUrl( const char* url );
	std::string&				getUrl( void )							    { return m_Url; }

	std::string&				getProtocol( void )							{ return m_Protocol; }
    std::string&                getHost( void )								{ return m_Host; }
	uint16_t					getPort( void )								{ return m_Port; }
	VxGUID&						getOnlineId( void )							{ return m_OnlineId; }
	EHostType					getHostType( void )							{ return m_HostType; }

	std::string					stripHost( const std::string& url ) const; // remove suffix invite type if exists

	bool						setUrlHostType( EHostType hostType ); 
	static bool					setUrlHostType( std::string& url, EHostType hostType );

protected:
    std::string					m_Url{ "" };
	std::string					m_Protocol{ "" };
	std::string					m_Host{ "" };
	uint16_t					m_Port{ 0 };
	VxGUID						m_OnlineId;
	EHostType					m_HostType{ eHostTypeUnknown };
};


