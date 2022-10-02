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
// http://www.nolimitconnect.org
//============================================================================

#include <CoreLib/VxDefs.h>

#include <string>
#include <vector>

#ifndef TARGET_OS_WINDOWS
    #include <netdb.h>        /* getaddrinfo(3) et al.                       */
    #include <netinet/in.h>   /* sockaddr_in & sockaddr_in6 definition.      */
	#include <unistd.h> 
#endif // TARGET_OS_WINDOWS

#pragma pack(push)
#pragma pack(1)

class InetAddress;
class InetAddrAndPort;
class InetAddrIPv4;
class InetAddrIPv4AndPort;
class PktBlobEntry;

//============================================================================
// size 16 bytes
// contains internet address in network order. can be IPv4 or IPv6
class InetAddress
{
public:
	InetAddress() = default;	
	InetAddress( const char* pIpAddress );
	InetAddress( uint32_t u32IpAddr );
    InetAddress( const InetAddress& rhs );
    bool                        addToBlob( PktBlobEntry& blob );
    bool                        extractFromBlob( PktBlobEntry& blob );

    InetAddress&                operator=(const InetAddress& inetAddr);
	bool                        operator == (const InetAddress& inetAddr) const;
    bool                        operator != (const InetAddress& inetAddr) const;

	InetAddress&                operator=(const InetAddrIPv4& inetAddr);
	bool                        operator == (const InetAddrIPv4& inetAddr) const;
	bool                        operator != (const InetAddrIPv4& inetAddr) const;

    bool                        isValid( void ) const;
    void                        setToInvalid( void );
    bool                        isIPv4( void ) const;
	bool                        isIPv6( void ) const;
    bool                        isLoopBack( void ) const;
	bool					    isLocalAddress( bool forLocalListening = false ) const;
	bool                        isIPv6GlobalAddress( void ) const;

    void                        setIp( const char* pIpAddress );
	// note.. u32IPv4Addr must be in host order
    void                        setIp( uint32_t u32IPv4Addr, bool bIsHostOrder = false );
	//! returns port in host order
    uint16_t                    setIp( struct sockaddr_storage& ipAddr );
	//! returns port in host order
    uint16_t                    setIp( struct sockaddr& ipAddr );
	
	//! fill address with this ip address and the given port.. returns struct len
    int                         fillAddress( struct sockaddr_storage& oAddr, uint16_t port );

    std::string                 toStdString( void );

    uint32_t                    getIPv4AddressInHostOrder( void ) const;
    uint32_t                    getIPv4AddressInNetOrder( void ) const;

	//! returns port in host order
	static uint16_t				getIpFromAddr(const struct sockaddr *sa, std::string& retStr);
	static InetAddress		    getDefaultIp( void );
    static int				    getAllAddresses( std::vector<InetAddress>& retAddress );
    static void                 dumpAddresses( std::vector<InetAddress>& addressList );

protected:
	//! returns port in host order
    uint16_t                    setIp( struct sockaddr_in& oIPv4Addr );
	//! returns port in host order
    uint16_t                    setIp( struct sockaddr_in6& oIPv6Addr );
	//! fill address with this ip address and the given port.. returns struct len
    int                         fillAddress( struct sockaddr_in& oIPv4Addr, uint16_t port );
	//! fill address with this ip address and the given port.. returns struct len
    int                         fillAddress( struct sockaddr_in6& oIPv6Addr, uint16_t port );

    bool                        isIPv4String( const char* pIpAddress ) const;
	// set ip from string
    void                        fromString( const char* pIpAddress );

    static bool                 isLittleEndian( void );
    static void                 litteEndianToNetIPv6( uint16_t * src, uint16_t * dest);
    static uint64_t             swap64Bit( uint64_t src );

public:
	uint64_t					m_u64AddrHi{ 0 };
	uint64_t					m_u64AddrLo{ 0 };
};

//============================================================================
class InetAddrAndPort : public InetAddress
{
public:
    InetAddrAndPort() = default;
	InetAddrAndPort( const char* ipAddr );
	InetAddrAndPort( const char* ipAddr, uint16_t port );
    InetAddrAndPort( const InetAddrAndPort& rhs );
    InetAddrAndPort( const InetAddress& rhs );
    bool                        addToBlob( PktBlobEntry& blob );
    bool                        extractFromBlob( PktBlobEntry& blob );

	InetAddrAndPort&            operator=(const InetAddress& inetAddr);
	InetAddrAndPort&            operator=(const InetAddrAndPort& inetAddr);

	InetAddrAndPort&            operator=(const InetAddrIPv4& inetAddr);
	InetAddrAndPort&            operator=(const InetAddrIPv4AndPort& inetAddr);

    uint16_t                    getPort( void )	const			                    { return m_u16Port; }
    void                        setPort( uint16_t u16Port )			                { m_u16Port = u16Port; }

    void                        setIpAndPort( struct sockaddr_storage& oAddr );
	void                        setIpAndPort( struct sockaddr& oAddr );
    void                        setIpAndPort( const char* ipAddr, uint16_t port );

    // these shadow InetAddress on purpose to avoid issues with override packet size change over internet
    void                        setIp( const char* pIpAddress )                            { return InetAddress::setIp( pIpAddress ); };
    // note.. u32IPv4Addr must be in host order
    void                        setIp( uint32_t u32IPv4Addr, bool bIsHostOrder = false )    { return InetAddress::setIp( u32IPv4Addr, bIsHostOrder ); };
    //! returns port in host order
    uint16_t                    setIp( struct sockaddr_storage& ipAddr )                    { return InetAddress::setIp( ipAddr ); };
    //! returns port in host order
    uint16_t                    setIp( struct sockaddr& ipAddr )                            { return InetAddress::setIp( ipAddr ); };
    //! returns port in host order
    uint16_t                    setIp( struct sockaddr_in& oIPv4Addr )                      { m_u16Port = InetAddress::setIp( oIPv4Addr ); return m_u16Port; };
    //! returns port in host order
    uint16_t                    setIp( struct sockaddr_in6& oIPv6Addr )                     { m_u16Port = InetAddress::setIp( oIPv6Addr ); return m_u16Port; };


	//=== vars ===//
    uint16_t                    m_u16Port{ 0 };
};

//============================================================================
// size 4 bytes
class InetAddrIPv4
{
public:
	InetAddrIPv4() = default;
	InetAddrIPv4( const char* pIpAddress );
	InetAddrIPv4( uint32_t u32IpAddr );
    InetAddrIPv4( const InetAddrIPv4& rhs );
    bool                        addToBlob( PktBlobEntry& blob );
    bool                        extractFromBlob( PktBlobEntry& blob );

	InetAddrIPv4&               operator=(const InetAddrIPv4& inetAddr);
	bool                        operator != (const InetAddrIPv4& inetAddr) const;
	bool                        operator == (const InetAddrIPv4& inetAddr) const;

	InetAddrIPv4&               operator=(const InetAddress& inetAddr);
	bool                        operator != (const InetAddress& inetAddr) const;
	bool                        operator == (const InetAddress& inetAddr) const;

	bool                        isValid( void ) const;
	void                        setToInvalid( void );
	bool                        isIPv4( void ) const						{ return isValid(); }
	bool                        isIPv6( void ) const						{ return false; }
	bool                        isLoopBack( void ) const;
	bool					    isLocalAddress( void ) const;

	bool                        setIp( const char* pIpAddress );
	// note.. u32IPv4Addr must be in host order
    bool                        setIp( uint32_t u32IPv4Addr, bool bIsHostOrder = false );
	//! returns port in host order
	uint16_t                    setIp( struct sockaddr_storage& ipAddr );
	//! returns port in host order
	uint16_t                    setIp( struct sockaddr& ipAddr );

	//! fill address with this ip address and the given port.. returns struct len
	int                         fillAddress( struct sockaddr_storage& oAddr, uint16_t port );

	std::string                 toStdString( void );
	InetAddress				    toInetAddress( void );

	uint32_t                    getIPv4AddressInHostOrder( void ) const;
	uint32_t                    getIPv4AddressInNetOrder( void ) const;

	//! returns port in host order
	static uint16_t				getIpFromAddr(const struct sockaddr *sa, std::string& retStr);

private:
	//! returns port in host order
	uint16_t                    setIp( struct sockaddr_in& oIPv4Addr );
	//! fill address with this ip address and the given port.. returns struct len
	int                         fillAddress( struct sockaddr_in& oIPv4Addr, uint16_t port );

	bool                        isIPv4String( const char* pIpAddress ) const;
	// set ip from string
	void                        fromString( const char* pIpAddress );

	static bool                 isLittleEndian( void );
	static uint32_t             swap32Bit( uint32_t src );

public:
    uint32_t					m_u32AddrIPv4{ 0 };
};

//============================================================================
class InetAddrIPv4AndPort : public InetAddrIPv4
{
public:
	InetAddrIPv4AndPort() = default;
    InetAddrIPv4AndPort( const InetAddrIPv4AndPort& rhs );
    bool                        addToBlob( PktBlobEntry& blob );
    bool                        extractFromBlob( PktBlobEntry& blob );

	InetAddrIPv4AndPort&        operator=(const InetAddrIPv4& inetAddr);
	InetAddrIPv4AndPort&        operator=(const InetAddrIPv4AndPort& inetAddr);

	InetAddrIPv4AndPort&        operator=(const InetAddress& oAddr);
	InetAddrIPv4AndPort&        operator=(const InetAddrAndPort& oAddr);

	uint16_t                    getPort( void )	const		    { return m_u16Port; }
	void                        setPort( uint16_t u16Port )		{ m_u16Port = u16Port; }

	void                        setIpAndPort( struct sockaddr_storage& oAddr );
	void                        setIpAndPort( struct sockaddr& oAddr );

	uint16_t                    m_u16Port{ 0 };
};

#pragma pack(pop)
