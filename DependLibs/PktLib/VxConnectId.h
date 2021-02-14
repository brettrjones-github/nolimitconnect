#pragma once
//============================================================================
// Copyright (C) 2013 Brett R. Jones
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

#include <CoreLib/VxGUID.h>

#include <NetLib/InetAddress.h>

#include <string>

#pragma pack(push) 
#pragma pack(1)

class PktBlobEntry;

//! derived from 16 bytes
//! size 
//	  16 bytes VxGUID
// +  16 bytes m_IPv6OnlineIp
// +   4 bytes m_IPv4OnlineIp
// +   2 bytes m_u16OnlinePort
// =  38 bytes total
class VxConnectId : public VxGUID
{
public:
	VxConnectId() = default;
	VxConnectId( const VxConnectId& rhs );
    bool                        addToBlob( PktBlobEntry& blob );
    bool                        extractFromBlob( PktBlobEntry& blob );

	VxConnectId&                operator = ( const VxConnectId& rhs );
	bool                        operator != (const VxConnectId& rhs ) const;
	bool                        operator == (const VxConnectId& rhs ) const;

	void						setPort( uint16_t port )			{ m_u16OnlinePort = htons( port ); }
	uint16_t					getPort( void )						{ return ntohs( m_u16OnlinePort ); }

	void						setIpAddress( const char * ipAddr );
	//! get ip address based on can connect ipv6 or need ipv4
	void						getIpAddress( std::string& retString );

	void						getIPv4( std::string& retString );
	void						getIPv6( std::string& retString );

	InetAddress&				getIPv6( void )						{ return m_IPv6OnlineIp; }
	InetAddrIPv4&				getIPv4( void )						{ return m_IPv4OnlineIp; }

	void						clear( void );

	VxGUID&						getOnlineId( void )					{ return *this; }


	//=== vars ===//
	InetAddress					m_IPv6OnlineIp;						// users ip6 address
	InetAddrIPv4				m_IPv4OnlineIp;						// users ip4 address

protected:
	uint16_t					m_u16OnlinePort;						// users incoming connection port
};

#pragma pack(pop)

