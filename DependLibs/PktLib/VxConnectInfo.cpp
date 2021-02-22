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

#include "VxConnectInfo.h"
#include "PktBlobEntry.h"

#include <CoreLib/VxParse.h>
#include <CoreLib/VxGlobals.h>
#include <NetLib/VxSktUtil.h>

#include <memory.h>

namespace
{
    const unsigned int MIN_INTERVAL_CONNECT_REQUEST_MS				= (5 * 60000);
}

//============================================================================
VxConnectBaseInfo::VxConnectBaseInfo( const VxConnectBaseInfo &rhs )
    : P2PEngineVersion( rhs )
    , MyOSVersion( rhs )
    , VxRelayFlags( rhs )
    , FriendMatch( rhs )
    , VxSearchFlags( rhs )
    , m_LanIPv4( rhs.m_LanIPv4 )
    , m_DirectConnectId( rhs.m_DirectConnectId )
    , m_RelayConnectId( rhs.m_RelayConnectId )
{
}

//============================================================================
bool VxConnectBaseInfo::addToBlob( PktBlobEntry& blob )
{
    bool result = P2PEngineVersion::addToBlob( blob );
    result &= MyOSVersion::addToBlob( blob );
    result &= VxRelayFlags::addToBlob( blob );
    result &= FriendMatch::addToBlob( blob );
    result &= VxSearchFlags::addToBlob( blob );
    result &= m_LanIPv4.addToBlob( blob );
    result &= m_DirectConnectId.addToBlob( blob );
    result &= m_RelayConnectId.addToBlob( blob );
    return result;
}

//============================================================================
bool VxConnectBaseInfo::extractFromBlob( PktBlobEntry& blob )
{
    bool result = P2PEngineVersion::extractFromBlob( blob );
    result &= MyOSVersion::extractFromBlob( blob );
    result &= VxRelayFlags::extractFromBlob( blob );
    result &= FriendMatch::extractFromBlob( blob );
    result &= VxSearchFlags::extractFromBlob( blob );
    result &= m_LanIPv4.extractFromBlob( blob );
    result &= m_DirectConnectId.extractFromBlob( blob );
    result &= m_RelayConnectId.extractFromBlob( blob );
    return result;
}

//============================================================================
VxConnectBaseInfo& VxConnectBaseInfo::operator =( const VxConnectBaseInfo &rhs )
{
	if( this != &rhs )
	{
        *((P2PEngineVersion*)this) = *((P2PEngineVersion*)&rhs);
        *((MyOSVersion*)this) = *((MyOSVersion*)&rhs);
        *((VxRelayFlags*)this) = *((VxRelayFlags*)&rhs);
        *((FriendMatch*)this) = *((FriendMatch*)&rhs);
        *((VxSearchFlags*)this) = *((VxSearchFlags*)&rhs);
        m_LanIPv4 = rhs.m_LanIPv4;
        m_DirectConnectId = rhs.m_DirectConnectId;
        m_RelayConnectId = rhs.m_RelayConnectId;
	}

	return *this;
}

//============================================================================
std::string VxConnectBaseInfo::getMyOnlineUrl( void )
{
    std::string strIPv4; 
    m_DirectConnectId.getIPv4(strIPv4);
    std::string strIPv6; 
    m_DirectConnectId.getIPv6(strIPv6);
    std::string myIp = VxIsIpValid( strIPv6 ) ? strIPv6  : strIPv4;

    std::string myUrl;
    StdStringFormat( myUrl, "ptop://%s:%d/%s", myIp.c_str(), m_DirectConnectId.getPort(), getMyOnlineId().toOnlineIdString().c_str() );
    return myUrl;
}

//============================================================================
void VxConnectBaseInfo::setMyOnlineId( uint64_t u64HiPart, uint64_t u64LoPart )
{ 
	m_DirectConnectId.setVxGUID( u64HiPart, u64LoPart );
}

//============================================================================
VxGUID& VxConnectBaseInfo::getMyOnlineId()								
{ 
	return m_DirectConnectId; 
}

bool			VxConnectBaseInfo::getMyOnlineId( std::string& strRetId )		{ return m_DirectConnectId.toHexString( strRetId ); }
uint64_t		VxConnectBaseInfo::getMyOnlineIdLoPart()						{ return m_DirectConnectId.getVxGUIDLoPart(); }
uint64_t		VxConnectBaseInfo::getMyOnlineIdHiPart()						{ return m_DirectConnectId.getVxGUIDHiPart(); }

VxGUID&			VxConnectBaseInfo::getRelayOnlineId()							{ return m_RelayConnectId; }
bool			VxConnectBaseInfo::getRelayOnlineId( std::string& strRetId )	{ return m_RelayConnectId.toHexString( strRetId ); }
uint64_t		VxConnectBaseInfo::getRelayOnlineIdLoPart()						{ return m_RelayConnectId.getVxGUIDLoPart(); }
uint64_t		VxConnectBaseInfo::getRelayOnlineIdHiPart()						{ return m_RelayConnectId.getVxGUIDHiPart(); }
void			VxConnectBaseInfo::setRelayPort( uint16_t port )				{ m_RelayConnectId.setPort( port ); }		
uint16_t		VxConnectBaseInfo::getRelayPort( void )							{ return m_RelayConnectId.getPort(); }

void			VxConnectBaseInfo::getMyOnlineIPv4( std::string& strRetIp )		{ strRetIp = m_DirectConnectId.m_IPv4OnlineIp.toStdString(); }
void			VxConnectBaseInfo::getMyOnlineIPv6( std::string& strRetIp )		{ m_DirectConnectId.getIPv6( strRetIp );}
InetAddrIPv4&	VxConnectBaseInfo::getMyOnlineIPv4( void )						{ return m_DirectConnectId.m_IPv4OnlineIp; }
InetAddress&	VxConnectBaseInfo::getMyOnlineIPv6( void )						{ return m_DirectConnectId.m_IPv6OnlineIp;}
void			VxConnectBaseInfo::setMyOnlinePort( uint16_t port )				{ m_DirectConnectId.setPort( port ); }		
uint16_t		VxConnectBaseInfo::getMyOnlinePort( void )						{ return m_DirectConnectId.getPort(); }

//============================================================================
//! get ip based on if we can connect ipv6 or ipv4 if not
InetAddress	VxConnectBaseInfo::getOnlineIpAddress( void )
{
	return m_DirectConnectId.m_IPv4OnlineIp.toInetAddress();
}

//============================================================================
//! get ip based on if we can connect ipv6 or ipv4 if not
InetAddress	VxConnectBaseInfo::getRelayIpAddress( void )
{
    return m_RelayConnectId.m_IPv4OnlineIp.toInetAddress();
}

//============================================================================
void VxConnectBaseInfo::setOnlineIpAddress( const char * pIp )			
{ 
	m_DirectConnectId.setIpAddress( pIp ); 
};

//============================================================================
void VxConnectBaseInfo::setOnlineIpAddress( InetAddress& oIp )			
{ 
	if( oIp.isIPv4() )
	{
		m_DirectConnectId.m_IPv4OnlineIp.setIp( oIp.getIPv4AddressInNetOrder() );
	}
	else
	{
		m_DirectConnectId.m_IPv6OnlineIp  = oIp;
	}
}

//============================================================================
uint16_t	VxConnectBaseInfo::getOnlinePort( void )							
{ 
	return m_DirectConnectId.getPort(); 
};

//============================================================================
void VxConnectBaseInfo::getOnlinePort( std::string& strRetPort )	
{ 
	StdStringFormat( strRetPort, "%d", m_DirectConnectId.getPort() ); 
}

//============================================================================
void VxConnectBaseInfo::setOnlinePort( uint16_t u16Port )				
{ 
	m_DirectConnectId.setPort( u16Port ); 
}

//============================================================================
bool VxConnectBaseInfo::hasValidRelay( void )
{
	if( 0 == m_RelayConnectId.getVxGUIDHiPart() 
		|| 0 == m_RelayConnectId.getVxGUIDLoPart() 
		|| (0 == m_RelayConnectId.getPort()) )
	{
		return false;
	}

	return true;
}

//============================================================================
// VxConnectIdent
//============================================================================
VxConnectIdent::VxConnectIdent()
{
	m_OnlineName[ 0 ] = 0;
	m_OnlineDesc[ 0 ] = 0;
    m_NetHostUrl[ 0 ] = 0;
    m_ChatRoomHostUrl[ 0 ] = 0;
    m_GroupHostUrl[ 0 ] = 0;
    m_RandomConnectUrl[ 0 ] = 0;
}

//============================================================================
VxConnectIdent::VxConnectIdent( const VxConnectIdent& rhs )
: VxConnectBaseInfo( rhs )
, m_TimeLastContactMs( rhs.m_TimeLastContactMs )
, m_PrimaryLanguage( rhs.m_PrimaryLanguage )
, m_ContentType( rhs.m_ContentType )
, m_u8Age( rhs.m_u8Age )
, m_u8Gender( rhs.m_u8Gender )
, m_IdentRes1( rhs.m_IdentRes1 )
, m_AvatarGuid( rhs.m_AvatarGuid )
, m_NetHostGuid( rhs.m_NetHostGuid )
, m_ChatRoomHostGuid( rhs.m_ChatRoomHostGuid )
, m_GroupHostGuid( rhs.m_GroupHostGuid )
, m_RandomConnectGuid( rhs.m_RandomConnectGuid )
{
    SafeStrCopy( m_OnlineName, rhs.m_OnlineName, MAX_ONLINE_NAME_LEN );
    SafeStrCopy( m_OnlineDesc, rhs.m_OnlineDesc, MAX_ONLINE_DESC_LEN );

    SafeStrCopy( m_NetHostUrl, rhs.m_NetHostUrl, MAX_NET_HOST_URL_LEN );
    SafeStrCopy( m_ChatRoomHostUrl, rhs.m_ChatRoomHostUrl, MAX_NET_HOST_URL_LEN );
    SafeStrCopy( m_GroupHostUrl, rhs.m_GroupHostUrl, MAX_NET_HOST_URL_LEN );
    SafeStrCopy( m_RandomConnectUrl, rhs.m_RandomConnectUrl, MAX_NET_HOST_URL_LEN );
}

//============================================================================
bool VxConnectIdent::addToBlob( PktBlobEntry& blob )
{
    bool result = VxConnectBaseInfo::addToBlob( blob );
    result &= blob.setValue( m_OnlineName, MAX_ONLINE_NAME_LEN );
    result &= blob.setValue( m_OnlineDesc, MAX_ONLINE_DESC_LEN );
    result &= blob.setValue( m_TimeLastContactMs );
    result &= blob.setValue( m_PrimaryLanguage );
    result &= blob.setValue( m_ContentType );
    result &= blob.setValue( m_u8Age );
    result &= blob.setValue( m_u8Gender );
    result &= blob.setValue( m_IdentRes1 );

    result &= blob.setValue( m_AvatarGuid );
    result &= blob.setValue( m_NetHostGuid );
    result &= blob.setValue( m_ChatRoomHostGuid );
    result &= blob.setValue( m_GroupHostGuid );
    result &= blob.setValue( m_RandomConnectGuid );

    result &= blob.setValue( m_NetHostUrl, MAX_NET_HOST_URL_LEN );
    result &= blob.setValue( m_ChatRoomHostUrl, MAX_NET_HOST_URL_LEN );
    result &= blob.setValue( m_GroupHostUrl, MAX_NET_HOST_URL_LEN );
    result &= blob.setValue( m_RandomConnectUrl, MAX_NET_HOST_URL_LEN );
    return result;
}

//============================================================================
bool VxConnectIdent::extractFromBlob( PktBlobEntry& blob )
{
    bool result = VxConnectBaseInfo::extractFromBlob( blob );
    int onlineNameLen = MAX_ONLINE_NAME_LEN;
    result &= blob.getValue( m_OnlineName, onlineNameLen );
    int onlineDescLen = MAX_ONLINE_NAME_LEN;
    result &= blob.getValue( m_OnlineDesc, onlineDescLen );
    result &= blob.getValue( m_TimeLastContactMs );
    result &= blob.getValue( m_PrimaryLanguage );
    result &= blob.getValue( m_ContentType );
    result &= blob.getValue( m_u8Age );
    result &= blob.getValue( m_u8Gender );
    result &= blob.getValue( m_IdentRes1 );

    result &= blob.getValue( m_AvatarGuid );
    result &= blob.getValue( m_NetHostGuid );
    result &= blob.getValue( m_ChatRoomHostGuid );
    result &= blob.getValue( m_GroupHostGuid );
    result &= blob.getValue( m_RandomConnectGuid );

    int len = MAX_NET_HOST_URL_LEN;
    result &= blob.getValue( m_NetHostUrl, len );
    result &= blob.getValue( m_ChatRoomHostUrl, len );
    result &= blob.getValue( m_GroupHostUrl, len );
    result &= blob.getValue( m_RandomConnectUrl, len );
    return result;
}

//============================================================================
VxConnectIdent& VxConnectIdent::operator =( const VxConnectIdent& rhs )
{
    if( this != &rhs )
    {
        SafeStrCopy( m_OnlineName, rhs.m_OnlineName, MAX_ONLINE_NAME_LEN );
        SafeStrCopy( m_OnlineDesc, rhs.m_OnlineDesc, MAX_ONLINE_DESC_LEN );

        m_TimeLastContactMs = rhs.m_TimeLastContactMs;
        m_PrimaryLanguage = rhs.m_PrimaryLanguage;
        m_ContentType = rhs.m_ContentType;
        m_u8Age = rhs.m_u8Age;
        m_u8Gender = rhs.m_u8Gender;
        m_IdentRes1 = rhs.m_IdentRes1;
        m_AvatarGuid = rhs.m_AvatarGuid;

        m_NetHostGuid = rhs.m_NetHostGuid;
        m_ChatRoomHostGuid = rhs.m_ChatRoomHostGuid;
        m_GroupHostGuid = rhs.m_GroupHostGuid;
        m_RandomConnectGuid = rhs.m_RandomConnectGuid;

        SafeStrCopy( m_NetHostUrl, rhs.m_NetHostUrl, MAX_NET_HOST_URL_LEN );
        SafeStrCopy( m_ChatRoomHostUrl, rhs.m_ChatRoomHostUrl, MAX_NET_HOST_URL_LEN );
        SafeStrCopy( m_GroupHostUrl, rhs.m_GroupHostUrl, MAX_NET_HOST_URL_LEN );
        SafeStrCopy( m_RandomConnectUrl, rhs.m_RandomConnectUrl, MAX_NET_HOST_URL_LEN );
    }

    return *this;
}

//============================================================================
void VxConnectIdent::setOnlineName( const char * pUserName )			
{ 
    if( !pUserName )
    {
        m_OnlineName[0] = 0;
    }
    else
    {
        SafeStrCopy( m_OnlineName, pUserName, sizeof( m_OnlineName ));
    }
}

//============================================================================
//! set users online description
void VxConnectIdent::setOnlineDescription( const char * pUserDesc )	
{ 
    if( !pUserDesc )
    {
        m_OnlineName[0] = 0;
    }
    else
    {
        SafeStrCopy( m_OnlineDesc, pUserDesc, sizeof( m_OnlineDesc ) );
    }
}

//============================================================================
void VxConnectIdent::setNetHostUrl( const char * netHostUrl )
{
    if( !netHostUrl )
    {
        m_NetHostUrl[ 0 ] = 0;
    }
    else
    {
        SafeStrCopy( m_NetHostUrl, netHostUrl, sizeof( m_NetHostUrl ) );
    }
}

//============================================================================
void VxConnectIdent::setChatRoomHostUrl( const char * groupListUrl )
{
    if( !groupListUrl )
    {
        m_ChatRoomHostUrl[ 0 ] = 0;
    }
    else
    {
        SafeStrCopy( m_ChatRoomHostUrl, groupListUrl, sizeof( m_ChatRoomHostUrl ) );
    }
}

//============================================================================
void VxConnectIdent::setGroupHostUrl( const char * groupHostUrl )
{
    if( !groupHostUrl )
    {
        m_GroupHostUrl[ 0 ] = 0;
    }
    else
    {
        SafeStrCopy( m_GroupHostUrl, groupHostUrl, sizeof( m_GroupHostUrl ) );
    }
}

//============================================================================
void VxConnectIdent::setRandomConnectUrl( const char * randomConnecttUrl )
{
    if( !randomConnecttUrl )
    {
        m_RandomConnectUrl[ 0 ] = 0;
    }
    else
    {
        SafeStrCopy( m_RandomConnectUrl, randomConnecttUrl, sizeof( m_RandomConnectUrl ) );
    }
}

//============================================================================
/// @brief return indenty unique folder name in the form of OnlineName_GuidHexString
std::string VxConnectIdent::getIdentFolderName( void )
{
    std::string folderName = getOnlineName();
    folderName += '_';
    folderName += getMyOnlineIdHexString();
    return folderName;
}

//============================================================================
VxConnectInfo::VxConnectInfo( const VxConnectInfo &rhs )
    : VxConnectIdent( rhs )
    , m_s64TimeLastConnectAttemptMs( rhs.m_s64TimeLastConnectAttemptMs )
    , m_s64TimeTcpLastContactMs( rhs.m_s64TimeTcpLastContactMs )
{
}

//============================================================================
VxConnectInfo& VxConnectInfo::operator =( const VxConnectInfo &rhs )
{
	if( this != &rhs )
	{
        *((VxConnectIdent*)this) = *((VxConnectIdent*)&rhs);
        m_s64TimeLastConnectAttemptMs = rhs.m_s64TimeLastConnectAttemptMs;
        m_s64TimeTcpLastContactMs = rhs.m_s64TimeLastConnectAttemptMs;
    }

	return *this;
}

//============================================================================
bool VxConnectInfo::addToBlob( PktBlobEntry& blob )
{
    bool result = VxConnectIdent::addToBlob( blob );
    result &= blob.setValue( m_s64TimeLastConnectAttemptMs );
    result &= blob.setValue( m_s64TimeTcpLastContactMs );
    return result;
}

//============================================================================
bool VxConnectInfo::extractFromBlob( PktBlobEntry& blob )
{
    bool result = VxConnectIdent::extractFromBlob( blob );
    result &= blob.getValue( m_s64TimeLastConnectAttemptMs );
    result &= blob.getValue( m_s64TimeTcpLastContactMs );
    return result;
}

//============================================================================
void VxConnectInfo::setTimeLastConnectAttemptMs( int64_t timeLastAttemptGmtMs )
{
	m_s64TimeLastConnectAttemptMs = timeLastAttemptGmtMs;
}

//============================================================================
int64_t VxConnectInfo::getTimeLastConnectAttemptMs( void )
{
	return m_s64TimeLastConnectAttemptMs;
}

//============================================================================
bool VxConnectInfo::isTooSoonToAttemptConnectAgain( void )
{
	return ( MIN_INTERVAL_CONNECT_REQUEST_MS > ( GetGmtTimeMs() - m_s64TimeLastConnectAttemptMs ) );
}

//============================================================================
void VxConnectInfo::setTimeLastTcpContactMs( int64_t timeGmtMs )
{
	m_s64TimeTcpLastContactMs = htonll( timeGmtMs );
}

//============================================================================
int64_t	VxConnectInfo::getTimeLastTcpContactMs( void )
{
	return ntohll( m_s64TimeTcpLastContactMs );
}

//============================================================================
//! get seconds since any last contact
int64_t VxConnectInfo::getElapsedMsAnyContact( void )	
{ 
    return ( GetGmtTimeMs() -  getTimeLastTcpContactMs() );
}

//============================================================================
//! get milli seconds since last tcp contact
int64_t VxConnectInfo::getElapsedMsTcpLastContact( void )	
{ 
	return ( GetGmtTimeMs() - getTimeLastTcpContactMs() ); 
}
