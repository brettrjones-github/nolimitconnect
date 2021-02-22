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

#include "VxConnectId.h"
#include "VxRelayFlags.h"
#include "VxFriendMatch.h"
#include "VxSearchFlags.h"

#define OS_APPLE_FLAG				0x01	
#define OS_ANDROID_FLAG				0x02	
#define OS_LINUX_FLAG				0x04	
#define OS_WINDOWS_FLAG				0x08	

#define MAX_ONLINE_NAME_LEN		    28	//maximum length of online name including 0 terminator
#define MAX_ONLINE_DESC_LEN		    32	//maximum length of online description including 0 terminator
#define MAX_NET_HOST_URL_LEN		64	//maximum length of a ptop url including 0 terminator

#pragma pack(push) 
#pragma pack(1)

class PktBlobEntry;

class P2PEngineVersion
{
public:
    P2PEngineVersion();
    P2PEngineVersion( const P2PEngineVersion& rhs );
    P2PEngineVersion&           operator =( const P2PEngineVersion& rhs );
    bool                        addToBlob( PktBlobEntry& blob );
    bool                        extractFromBlob( PktBlobEntry& blob );

    uint8_t						getP2PEngineVersion( void );
    void						getP2PEngineVersion( std::string& strRetP2PEngineVersion );

private:
    uint8_t						m_u8P2PEngineVersion{ 0 };
};

class MyOSVersion
{
public:
    MyOSVersion();
    MyOSVersion( const MyOSVersion& rhs );
    bool                        addToBlob( PktBlobEntry& blob );
    bool                        extractFromBlob( PktBlobEntry& blob );

    MyOSVersion&                operator =( const MyOSVersion& rhs );

    uint8_t						getOSVersion( void );
    void						getOSVersion( std::string& strRetOSVersion );

private:
    uint8_t						m_u8OSVersion{0};
};

// size
// +  1 byte P2PEngineVersion
// +  1 byte MyOSVersion
// +  1 byte VxRelayFlags
// +  1 byte FriendMatch
// +  2 bytes VxSearchFlags
// +  4 bytes m_LanIPv4
// + 38 bytes m_DirectConnectId
// + 38 bytes m_RelayConnectId 
// = 86 bytes 
class VxConnectBaseInfo : public P2PEngineVersion, public MyOSVersion, public VxRelayFlags, public FriendMatch, public VxSearchFlags
{
public:
	VxConnectBaseInfo() = default;
	VxConnectBaseInfo( const VxConnectBaseInfo& rhs );
	VxConnectBaseInfo&          operator =( const VxConnectBaseInfo& rhs );

    bool                        addToBlob( PktBlobEntry& blob );
    bool                        extractFromBlob( PktBlobEntry& blob );

    std::string                 getMyOnlineUrl( void );

	void						setMyOnlineId( uint64_t u64HiPart, uint64_t u64LoPart );
	VxGUID&						getMyOnlineId();
	bool						getMyOnlineId( std::string& strRetId );
    std::string				    getMyOnlineIdHexString( void )  { return m_DirectConnectId.toHexString(); }
    uint64_t					getMyOnlineIdLoPart();
	uint64_t					getMyOnlineIdHiPart();
	void						setMyOnlinePort( uint16_t port );
	uint16_t					getMyOnlinePort( void );

	VxGUID&						getRelayOnlineId();
    bool						getRelayOnlineId( std::string& strRetId );
	uint64_t					getRelayOnlineIdLoPart();
	uint64_t					getRelayOnlineIdHiPart();
	void						setRelayPort( uint16_t port );
	uint16_t					getRelayPort( void );

	void						getMyOnlineIPv4( std::string& strRetIp );
	void						getMyOnlineIPv6( std::string& strRetIp );
	InetAddrIPv4&				getMyOnlineIPv4( void );
	InetAddress&				getMyOnlineIPv6( void );

	InetAddrIPv4&				getLanIPv4( void )					{ return m_LanIPv4; }

	InetAddress					getOnlineIpAddress( void );
	InetAddress					getRelayIpAddress( void );

	void						setOnlineIpAddress( InetAddress& oIp );
	void						setOnlineIpAddress( const char * pIp );

	uint16_t					getOnlinePort( void );
	void						getOnlinePort( std::string& strRetPort );
	void						setOnlinePort( uint16_t u16Port );

	bool						hasValidRelay( void );

	VxConnectId&				getDirectConnectId( void )							{ return m_DirectConnectId; }
	VxConnectId&				geRelayConnectId( void )							{ return m_RelayConnectId; }

	//=== vars ===//
	InetAddrIPv4				m_LanIPv4;
	VxConnectId					m_DirectConnectId;
	VxConnectId					m_RelayConnectId;
};

// +  28 bytes Online Name
// +  32 bytes Online Mood Message
// +   8 bytes m_TimeLastContactMs
// +   2 bytes m_PrimaryLanguage
// +   1 bytes m_ContentType
// +   1 bytes m_u8Age
// +   1 bytes m_u8Gender
// +   1 bytes reserved
// +  80 bytes guids (5x16)
// = 154 bytes
// + 256  urls (4x64)
// = 410 bytes
// +  86 bytes VxConnectBaseInfo
// = 496 bytes

class VxConnectIdent : public VxConnectBaseInfo
{
public:
	VxConnectIdent();
    VxConnectIdent( const VxConnectIdent& rhs );
    bool                        addToBlob( PktBlobEntry& blob );
    bool                        extractFromBlob( PktBlobEntry& blob );

    VxConnectIdent&             operator =( const VxConnectIdent& rhs );

	VxConnectBaseInfo& 			getConnectBaseInfo( void )	                        { return * ((VxConnectBaseInfo *)this); }
	VxConnectIdent& 			getConnectIdent( void )		                        { return * ((VxConnectIdent *)this); }

	void 						setOnlineName( const char * pUserName );
    char *                      getOnlineName( void )                               { return m_OnlineName; }
	void 						setOnlineDescription( const char * pUserDesc );
	char *						getOnlineDescription( void )                        { return m_OnlineDesc; }

    void 						setNetHostUrl( const char * netHostUrl );
    char *						getNetHostUrl( void )                               { return m_NetHostUrl; }
    void 						setChatRoomHostUrl( const char * groupListUrl );
    char *						getChatRoomHostUrl( void )                          { return m_ChatRoomHostUrl; }
    void 						setGroupHostUrl( const char * groupHostUrl );
    char *						getGroupHostUrl( void )                             { return m_GroupHostUrl; }
    void 						setRandomConnectUrl( const char * groupHostUrl );
    char *						getRandomConnectUrl( void )                         { return m_RandomConnectUrl; }

	void 						setTimeLastContact( int64_t timeStamp )				{ m_TimeLastContactMs = timeStamp; }
	int64_t	    				getTimeLastContact( void )					        { return m_TimeLastContactMs; }

    /// @brief return indenty unique folder name in the form of OnlineName_GuidHexString
    std::string	    			getIdentFolderName( void );

    void                        setAvatarGuid( VxGUID& guid )           { m_AvatarGuid = guid;  }
    VxGUID&                     getAvatarGuid( void )                   { return m_AvatarGuid; }
    void                        setNetHostGuid( VxGUID& guid )          { m_NetHostGuid = guid; }
    VxGUID&                     getNetHostGuid( void )                  { return m_NetHostGuid; }
    void                        setChatRoomHostGuid( VxGUID& guid )     { m_ChatRoomHostGuid = guid; }
    VxGUID&                     getChatRoomHostGuid( void )             { return m_ChatRoomHostGuid; }
    void                        setGroupHostGuid( VxGUID& guid )        { m_GroupHostGuid = guid; }
    VxGUID&                     getGroupHostGuid( void )                { return m_GroupHostGuid; }
    void                        setRandomConnectGuid( VxGUID& guid )    { m_RandomConnectGuid = guid; }
    VxGUID&                     getRandomConnectGuid( void )            { return m_RandomConnectGuid; }

    void						setPrimaryLanguage( uint16_t language ) { m_PrimaryLanguage = language; }
    uint16_t					getPrimaryLanguage( void )              { return m_PrimaryLanguage; }

    void						setPreferredContent( uint8_t contentType ) { m_ContentType = contentType; }
    uint8_t					    getPreferredContent( void )             { return m_ContentType; }

    void						setAgeType( EAgeType age )              { m_u8Age = (uint8_t)age; }
    EAgeType					getAgeType( void )                      { return (EAgeType)m_u8Age; }

    void						setGender( uint8_t gender )             { m_u8Gender = gender; }
    uint8_t					    getGender( void )                       { return m_u8Gender; }

	//=== vars ===//
private:
	char						m_OnlineName[ MAX_ONLINE_NAME_LEN ];	// users online name
	char						m_OnlineDesc[ MAX_ONLINE_DESC_LEN ];    // users online description
    int64_t	    				m_TimeLastContactMs{ 0 };
    uint16_t					m_PrimaryLanguage{ 0 };     // primary language user speaks
    uint8_t					    m_ContentType{ 0 };         // preferred content type
    uint8_t						m_u8Age{ 0 };
    uint8_t						m_u8Gender{ 0 };
	uint8_t					    m_IdentRes1{ 0 };
    VxGUID                      m_AvatarGuid;

    VxGUID                      m_NetHostGuid;
    VxGUID                      m_ChatRoomHostGuid;
    VxGUID                      m_GroupHostGuid;
    VxGUID                      m_RandomConnectGuid;
    char						m_NetHostUrl[ MAX_NET_HOST_URL_LEN ];
    char						m_ChatRoomHostUrl[ MAX_NET_HOST_URL_LEN ]; 
    char						m_GroupHostUrl[ MAX_NET_HOST_URL_LEN ];
    char						m_RandomConnectUrl[ MAX_NET_HOST_URL_LEN ];
};

//     8 bytes m_s64TimeTcpLastContactMs
// +   8 bytes last connect attempt
// =  16 bytes total
// + 496 bytes VxConnectIdent
// = 512 bytes total
class VxConnectInfo : public VxConnectIdent
{
public:
    VxConnectInfo() = default;
	VxConnectInfo( const VxConnectInfo& rhs );
	VxConnectInfo&              operator =( const VxConnectInfo& rhs );
    bool                        addToBlob( PktBlobEntry& blob );
    bool                        extractFromBlob( PktBlobEntry& blob );

	void						setTimeLastConnectAttemptMs( int64_t timeLastAttempt );
	int64_t 					getTimeLastConnectAttemptMs( void );
	bool						isTooSoonToAttemptConnectAgain( void );

	void						setTimeLastTcpContactMs( int64_t time );
	int64_t	    				getTimeLastTcpContactMs( void );

	int64_t		    			getElapsedMsAnyContact( void );	
	int64_t			    		getElapsedMsTcpLastContact( void );

	//=== vars ===//
private:
    int64_t					    m_s64TimeLastConnectAttemptMs{0};
    int64_t					    m_s64TimeTcpLastContactMs{0};	// time of last contact via tcp
};

#pragma pack(pop)
