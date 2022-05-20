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

#include "VxCommon.h"
#include "PktBlobEntry.h"

#include <NetLib/VxSktBase.h>
#include <NetLib/VxSktUtil.h>
#include <CoreLib/VxParse.h>
#include <CoreLib/VxDebug.h>
#include <CoreLib/VxGlobals.h>

#include <memory.h>
#include <stdio.h>
#include <stdarg.h>

//============================================================================
//! generate connection key from network identity
RCODE GenerateConnectionKey( VxKey *				poRetKey,		// set this key
                             VxConnectId *			poConnectId,	// network identity
                             uint16_t				cryptoPort,
                             const char *			networkName )
{
    std::string strNetName = networkName;
    std::string strIPv4;
    poConnectId->getIPv4( strIPv4 );
    uint16_t u16Port = poConnectId->getPort();

    return GenerateConnectionKey( poRetKey, strIPv4, u16Port, poConnectId->getOnlineId(), cryptoPort, strNetName );
}

//============================================================================
RCODE GenerateConnectionKey( VxKey *					poRetKey,		// set this key
                             std::string&               ipAddr,
                             uint16_t                   port,
                             VxGUID&                    onlineId,
                             uint16_t					cryptoPort,
                             std::string&				networkName )
{
    uint64_t u64IdLowPart = onlineId.getVxGUIDLoPart();
    //vx_assert( u64IdLowPart );
    uint64_t u64IdHiPart = onlineId.getVxGUIDHiPart();
    //vx_assert( u64IdHiPart );
    std::string strIPv4 = ipAddr;
    vx_assert( port );

    std::string strPwd;
    StdStringFormat( strPwd, "%d%llx%llx%s%s%d",
        port,
        u64IdLowPart,
        u64IdHiPart,
        networkName.c_str(),
        strIPv4.c_str(),
        cryptoPort
    );

    // LogModule( eLogTcpData, LOG_VERBOSE, "GenerateConnectionKey: setting Key %s for %s:%d %d", strPwd.c_str(), strIPv4.c_str(), port, cryptoPort );
    poRetKey->setKeyFromPassword( strPwd.c_str(), (int)strPwd.size() );
    return 0;
}

//============================================================================
PluginPermission::PluginPermission()
{
	memset( m_au8Permissions, 0, sizeof( m_au8Permissions ) );
}

//============================================================================
bool PluginPermission::addToBlob( PktBlobEntry& blob )
{
    return blob.setValue( m_au8Permissions, (int)sizeof( m_au8Permissions ) );
}

//============================================================================
bool PluginPermission::extractFromBlob( PktBlobEntry& blob )
{
    int iBufLen = sizeof( m_au8Permissions );
    return blob.getValue(  (void *)m_au8Permissions, iBufLen );
}

//============================================================================
PluginPermission& PluginPermission::operator = ( const PluginPermission& rhs )
{
    if( this != &rhs )
    {
        memcpy( m_au8Permissions, rhs.m_au8Permissions, sizeof( m_au8Permissions ) );
    }

    return *this;
}

//============================================================================
bool PluginPermission::isPluginEnabled( EPluginType ePlugin )		
{ 
	return (eFriendStateIgnore == getPluginPermission(ePlugin))?0:1; 
}

//============================================================================
//! get type of permission user has set for givin plugin
EFriendState PluginPermission::getPluginPermission( EPluginType ePluginType ) 
{ 
	if(( ePluginType > 0 ) && ( ePluginType < eMaxPluginType ) )
	{
		int pluginNum = (int)(ePluginType - 1);
		int byteIdx = pluginNum >> 1;
		int byteShift = pluginNum & 0x01 ? 4 : 0;
		uint8_t byteWithPerm = m_au8Permissions[ byteIdx ];

		return  (EFriendState)( ( byteWithPerm >> byteShift ) & 0xf ); 
	}

	return eFriendStateIgnore;
}

//============================================================================
//! set type of permission user has set for givin plugin
void PluginPermission::setPluginPermission( EPluginType ePluginType, EFriendState eFriendState ) 
{ 
	if(( ePluginType > 0 ) && ( ePluginType < eMaxUserPluginType ) )
	{
		int pluginNum = (int)(ePluginType - 1);
		int byteIdx = pluginNum >> 1;
		int byteShift = pluginNum & 0x01 ? 4 : 0;
		uint8_t byteWithPerm = m_au8Permissions[ byteIdx ];
		if( byteShift )
		{
			byteWithPerm &= 0x0f;
			byteWithPerm |= (eFriendState << byteShift);
		}
		else
		{
			byteWithPerm &= 0xf0;
			byteWithPerm |= (eFriendState);
		}

		if( ( byteIdx < PERMISSION_ARRAY_SIZE ) 
			&& ( 0 <= byteIdx ) )
		{
			m_au8Permissions[ byteIdx ] = byteWithPerm;
		}
		else
		{
			LogMsg( LOG_ERROR, "setPluginPermission index out of range %d\n", byteIdx );
		}
	}
}

//============================================================================
void PluginPermission::setPluginPermissionsToDefaultValues( void )	
{ 
	memset( m_au8Permissions, 0, sizeof( m_au8Permissions ) );
	setPluginPermission( ePluginTypeAdmin, eFriendStateAdmin );	

    setPluginPermission( ePluginTypeMessenger, eFriendStateGuest );

    setPluginPermission( ePluginTypeAboutMePageServer, eFriendStateGuest );
    setPluginPermission( ePluginTypeStoryboardServer, eFriendStateGuest );
    setPluginPermission( ePluginTypeTruthOrDare, eFriendStateGuest );
    setPluginPermission( ePluginTypeVideoPhone, eFriendStateGuest );
    setPluginPermission( ePluginTypeVoicePhone, eFriendStateGuest );
	setPluginPermission( ePluginTypePushToTalk, eFriendStateFriend );

    setPluginPermission( ePluginTypeCamServer, eFriendStateIgnore );
    setPluginPermission( ePluginTypeFileShareServer, eFriendStateFriend );
    setPluginPermission( ePluginTypePersonFileXfer, eFriendStateFriend );

    setPluginPermission( ePluginTypeClientChatRoom, eFriendStateGuest ); // gives guest permission to whoever is in chat room at the same time
    setPluginPermission( ePluginTypeClientConnectTest, eFriendStateAnonymous );
    setPluginPermission( ePluginTypeClientGroup, eFriendStateIgnore );
    setPluginPermission( ePluginTypeClientNetwork, eFriendStateIgnore );
    setPluginPermission( ePluginTypeClientRandomConnect, eFriendStateGuest );
    setPluginPermission( ePluginTypeClientPeerUser, eFriendStateGuest );

    setPluginPermission( ePluginTypeHostChatRoom, eFriendStateIgnore ); // gives guest permission to whoever is in chat room at the same time
    setPluginPermission( ePluginTypeHostConnectTest, eFriendStateIgnore ); 
    setPluginPermission( ePluginTypeHostGroup, eFriendStateIgnore );
    setPluginPermission( ePluginTypeHostNetwork, eFriendStateIgnore );
    setPluginPermission( ePluginTypeHostRandomConnect, eFriendStateIgnore );
    setPluginPermission( ePluginTypeHostPeerUser, eFriendStateGuest );

    setPluginPermission( ePluginTypeNetworkSearchList, eFriendStateIgnore );
} 

//============================================================================
//============================================================================
//============================================================================
VxNetIdent::VxNetIdent()
: m_u16AppVersion( htons( VxGetAppVersion() ) )	
{
}

//============================================================================
VxNetIdent::VxNetIdent(const VxNetIdent &rhs )
: VxNetIdentBase( rhs )
, PluginPermission( rhs )
, m_u16AppVersion( rhs.m_u16AppVersion )
, m_u16PingTimeMs( rhs.m_u16PingTimeMs )
, m_NetIdentRes1( rhs.m_NetIdentRes1 )
, m_NetIdentRes2( rhs.m_NetIdentRes2 )  
, m_NetIdentRes3( rhs.m_NetIdentRes3 )
, m_LastSessionTimeGmtMs( rhs.m_LastSessionTimeGmtMs )
{
}

//============================================================================
bool VxNetIdent::addToBlob( PktBlobEntry& blob )
{
    uint8_t startMagicNum = 98;
    bool result = blob.setValue( startMagicNum );
    result &= VxNetIdentBase::addToBlob( blob );
    result &= PluginPermission::addToBlob( blob );
    result &= blob.setValue( m_u16AppVersion );
    result &= blob.setValue( m_u16PingTimeMs );
    result &= blob.setValue( m_NetIdentRes1 );
    result &= blob.setValue( m_NetIdentRes2 );
    result &= blob.setValue( m_NetIdentRes3 );
    result &= blob.setValue( m_LastSessionTimeGmtMs );
    uint8_t stopMagicNum = 99;
    result &= blob.setValue( stopMagicNum );
    return result;
}

//============================================================================
bool VxNetIdent::extractFromBlob( PktBlobEntry& blob )
{
    uint8_t startMagicNum;
    bool result = blob.getValue( startMagicNum );
    if( !result || startMagicNum != 98 )
    {
        LogMsg( LOG_ERROR, "VxNetIdent::extractFromBlob startMagicNum not valid" );
        return false;
    }

    result &= VxNetIdentBase::extractFromBlob( blob );
    result &= PluginPermission::extractFromBlob( blob );
    result &= blob.getValue( m_u16AppVersion );
    result &= blob.getValue( m_u16PingTimeMs );
    result &= blob.getValue( m_NetIdentRes1 );
    result &= blob.getValue( m_NetIdentRes2 );
    result &= blob.getValue( m_NetIdentRes3 );
    result &= blob.getValue( m_LastSessionTimeGmtMs );

    uint8_t stopMagicNum;
    result &= blob.getValue( stopMagicNum );
    if( !result || stopMagicNum != 99 )
    {
        LogMsg( LOG_ERROR, "VxNetIdent::extractFromBlob stopMagicNum not valid" );
        return false;
    }

    return result;
}

//============================================================================
//! copy operator
VxNetIdent& VxNetIdent::operator =( const VxNetIdent& rhs  )
{
    if( this != &rhs )
    {
        *( (VxNetIdentBase*)this ) = *( (VxNetIdentBase*)&rhs );
        *( (PluginPermission*)this ) = *( (PluginPermission*)&rhs );
        m_u16AppVersion = rhs.m_u16AppVersion;
        m_u16PingTimeMs = rhs.m_u16PingTimeMs;
        m_NetIdentRes1 = rhs.m_NetIdentRes1;
        m_NetIdentRes2 = rhs.m_NetIdentRes2;
        m_NetIdentRes3 = rhs.m_NetIdentRes3;
        m_LastSessionTimeGmtMs = rhs.m_LastSessionTimeGmtMs;
    }

    return *this;
}

//============================================================================
bool VxNetIdent::isValidNetIdent()
{
	bool result = getMyOnlineId().isVxGUIDValid();
	if( result )
	{
		result &= isOnlineNameValid();
	}

	return result;
} 

//============================================================================
bool VxNetIdent::isOnlineNameValid( void )
{
    bool result = !(m_OnlineName[0] == 0);
	if( result )
	{
		int asciiCnt = 0;
		bool foundTerminator{ false };
		bool invalidChar{ false };
		for( int i = 0; i < MAX_ONLINE_NAME_LEN; i++ )
		{
			if( 0 == m_OnlineName[i] )
			{
				foundTerminator = true;
				break;
			}
			else if( isascii( m_OnlineName[i] ) )
			{
				asciiCnt++;
			}
			else
			{
				// invalid char
				invalidChar = true;
				break;
			}
		}

		result &= !invalidChar && foundTerminator && asciiCnt >= 3;
	}

	return result;
}

//============================================================================
void VxNetIdent::setPingTimeMs( uint16_t pingTime )
{
	m_u16PingTimeMs = htons( pingTime );
}

//============================================================================
uint16_t VxNetIdent::getPingTimeMs( void )
{
	return ntohs( m_u16PingTimeMs );
}

//============================================================================
EPluginAccess	VxNetIdent::getHisAccessPermissionFromMe( EPluginType ePluginType, bool inGroup )
{
	EFriendState friendState = getMyFriendshipToHim();
	if( inGroup && friendState == eFriendStateAnonymous )
	{
		friendState = eFriendStateGuest;
	}

	return getPluginAccessState( ePluginType, friendState );
}

//============================================================================
EPluginAccess VxNetIdent::getMyAccessPermissionFromHim( EPluginType ePluginType, bool inGroup )
{
	EFriendState friendState = getHisFriendshipToMe();
	if( inGroup && friendState == eFriendStateAnonymous )
	{
		friendState = eFriendStateGuest;
	}

	EPluginAccess accessState = getPluginAccessState( ePluginType, friendState );
	if( ePluginAccessOk == accessState )
	{
		if( ( ePluginTypeFileShareServer == ePluginType ) 
			&& ( false == hasSharedFiles() ) )
		{
			// no files shared
			return ePluginAccessInactive;
		}

		if( ( ePluginTypeCamServer == ePluginType ) 
			&& ( false == hasSharedWebCam() ) )
		{
			// no shared web cam
			return ePluginAccessInactive;
		}

		if( ( ePluginTypeAboutMePageServer == ePluginType )
			|| ( ePluginTypeStoryboardServer == ePluginType ) )
		{
			if( false == isOnline() )
			{
				accessState = ePluginAccessRequiresOnline;
			}

			if( requiresRelay() )
			{
				accessState = ePluginAccessRequiresDirectConnect;
			}
		}
		else if( ePluginTypeMessenger != ePluginType )
		{
			if( false == isOnline() )
			{
				accessState = ePluginAccessRequiresOnline;
			}
		}		
	}

	return accessState;
}

//============================================================================
bool VxNetIdent::isHisAccessAllowedFromMe( EPluginType ePluginType, bool inGroup )
{
	EFriendState friendState = this->getMyFriendshipToHim();
	if( eFriendStateAnonymous == friendState && inGroup )
	{
		friendState = eFriendStateGuest;
	}

	return ( ePluginAccessOk == getPluginAccessState( ePluginType, friendState ) );
}

//============================================================================
bool VxNetIdent::isMyAccessAllowedFromHim( EPluginType ePluginType, bool inGroup )
{
	EFriendState friendState = this->getHisFriendshipToMe();
	if( eFriendStateAnonymous == friendState && inGroup )
	{
		friendState = eFriendStateGuest;
	}

	return ( ePluginAccessOk == getPluginAccessState( ePluginType, friendState ) );
}

//============================================================================
EPluginAccess VxNetIdent::getPluginAccessState( EPluginType ePluginType, EFriendState eHisPermissionToMe )
{
	if( eFriendStateIgnore == eHisPermissionToMe )
	{
		return ePluginAccessIgnored;
	}

	EFriendState ePermissionLevel = this->getPluginPermission( ePluginType );
	if( eFriendStateIgnore == ePermissionLevel )
	{
		return ePluginAccessDisabled;
	}

	if( ePermissionLevel > eHisPermissionToMe )
	{
		return ePluginAccessLocked;
	}
	if( (ePluginTypeFileShareServer == ePluginType) && 
		(false == hasSharedFiles()) )
	{
		// no files shared
		return ePluginAccessInactive;
	}

	if( (ePluginTypeCamServer == ePluginType) && 
		(false == hasSharedWebCam()) )
	{
		// no files shared
		return ePluginAccessInactive;
	}

	return ePluginAccessOk;
}

//============================================================================
bool VxNetIdent::canRequestJoin( EHostType hostType )
{
	if( requiresRelay() )
	{
		return false;
	}

	EPluginType pluginType = HostTypeToHostPlugin( hostType );
	EFriendState pluginPermissionLevel = getPluginPermission( pluginType );
	return pluginPermissionLevel != eFriendStateIgnore && getHisFriendshipToMe() != eFriendStateIgnore &&
		getMyFriendshipToHim() != eFriendStateIgnore;
}

//============================================================================
bool VxNetIdent::canJoinImmediate( EHostType hostType ) // request to join will be granted immediate because have sufficient permission
{
	if( requiresRelay() )
	{
		return false;
	}

	EPluginType pluginType = HostTypeToHostPlugin( hostType );
	EFriendState pluginPermissionLevel = getPluginPermission( pluginType );
	return pluginPermissionLevel != eFriendStateIgnore && getHisFriendshipToMe() != eFriendStateIgnore &&
		getMyFriendshipToHim() != eFriendStateIgnore && getHisFriendshipToMe() >= pluginPermissionLevel;
}

//============================================================================
//! dump identity
void VxNetIdent::debugDumpIdent( void )
{
	std::string strIPv4; 
	m_DirectConnectId.getIPv4(strIPv4);
	std::string strIPv6; 
	m_DirectConnectId.getIPv6(strIPv6);

	LogMsg( LOG_INFO, "Ident %s id %s my friendship %s his friendship %s search 0x%x ipv4 %s ipv6 %s port %d proxy flags 0x%x ",
		getOnlineName(),
		getMyOnlineId().describeVxGUID().c_str(),
		DescribeFriendState(getMyFriendshipToHim()),
		DescribeFriendState(getHisFriendshipToMe()),
		getSearchFlags(),
		strIPv4.c_str(),
		strIPv6.c_str(),
		m_DirectConnectId.getPort(),
		m_u8RelayFlags
		);
}

//============================================================================
//! describe plugin local name
const char * DescribePluginLclName( EPluginType ePluginType )
{
	switch( ePluginType )
	{
    case ePluginTypeInvalid:
        return "Invalid Plugin";
    case ePluginTypeAboutMePageServer:
        return "About Me Web Page Server";
	case ePluginTypeAboutMePageClient:
		return "About Me Web Page Viewer";
    case ePluginTypeClientPeerUser:
        return "Client User Avatar Image";
    case ePluginTypeHostPeerUser:
        return "Host User Avatar Image";
    case ePluginTypeCamServer:	// web cam broadcast plugin
        return "Shared Video Broadcast";
    case ePluginTypeFileShareServer:
        return "Shared Files";
    case ePluginTypePersonFileXfer:	// file transfer plugin
        return "Offer A File";
    case ePluginTypeMessenger:
        return "Chat Session";
    case ePluginTypeStoryboardServer:	// User Web Page Storyboard
        return "Story Board Server";
	case ePluginTypeStoryboardClient:	// User Web Page Storyboard
		return "Story Board Viewer";
    case ePluginTypeTruthOrDare:	// Web Cam Truth Or Dare game p2p plugin
        return "Truth Or Dare Video Chat";
    case ePluginTypeVideoPhone:	// Video phone p2p plugin
        return "Video Phone";
    case ePluginTypeVoicePhone:
        return "VOIP Voice Phone";
	case ePluginTypePushToTalk:
		return "Push To Talk Walkie Talkie ";
    case ePluginTypeClientChatRoom:
        return "Chat Room Client";
    case ePluginTypeHostChatRoom:
        return "Chat Room Host Service";
    case ePluginTypeHostConnectTest:
        return "Connection Test Service";
    case ePluginTypeClientConnectTest:
        return "Connection Test Client";
    case ePluginTypeHostGroup:
        return "Group Host Service";
    case ePluginTypeClientGroup:
        return "Group Client";
    case ePluginTypeHostNetwork:
        return "Network Host Sevice";
    case ePluginTypeNetworkSearchList:
        return "Search Listing Service";
    case ePluginTypeHostRandomConnect:
        return "Random Connect Service";
    case ePluginTypeClientRandomConnect:
        return "Random Connect Client";


    case ePluginTypeCameraService:
        return "Camera Feed Service";
    case ePluginTypeMJPEGReader:
        return "MJPEG Reader";
    case ePluginTypeMJPEGWriter:
        return "MJPEG Writer";
    case ePluginTypePersonalRecorder:
        return "Personal Recorder";
    case ePluginTypeNetServices:
        return "Net Service";
    case ePluginTypeSearch:
        return "Search Service";
    case ePluginTypeSndReader:
        return "Audio Playback";
    case ePluginTypeSndWriter:
        return "Audio Recorder";
    case ePluginTypeWebServer:
        return "Web Page Services";
	case ePluginTypeFileShareClient:
		return "Download Shared Files";

	default:		
		return "Unknown Plugin";
	}
};

//============================================================================
RCODE VxReportHack(	EHackerLevel hackerLevel, EHackerReason hackerReason, VxSktBase * sktBase, const char * pDescription, ... )	
{
	char as8Buf[ 2048 ];
	va_list argList;
	va_start( argList, pDescription );
	vsnprintf( as8Buf, sizeof( as8Buf ), pDescription, argList );
	as8Buf[sizeof( as8Buf ) - 1] = 0;
	va_end( argList );
	LogMsg( LOG_ERROR, "%s %s %s %s",  DescribeHackerLevel( hackerLevel ), DescribeHackerReason( hackerReason ),
        sktBase->describeSktConnection().c_str(),
		as8Buf );
	return 0;
}

//============================================================================
RCODE VxReportHack(	EHackerLevel hackerLevel, EHackerReason hackerReason, SOCKET skt, const char * ipAddr, const char * pDescription, ... )
{
	char as8Buf[ 2048 ];
	va_list argList;
	va_start( argList, pDescription );
	vsnprintf( as8Buf, sizeof( as8Buf ), pDescription, argList );
	as8Buf[sizeof( as8Buf ) - 1] = 0;
	va_end( argList );
	LogMsg( LOG_ERROR, "%s %s handle %d ip %s %s", DescribeHackerLevel( hackerLevel ), DescribeHackerReason( hackerReason ),
		skt, 
		ipAddr, 
		as8Buf );
	return 0;
}
