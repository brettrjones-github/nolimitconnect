//============================================================================
// Copyright (C) 2003 Brett R. Jones 
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

#include "PktAnnounce.h"
#include "PktTypes.h"

#include <CoreLib/VxParse.h>
#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxMutex.h>
#include <CoreLib/VxTime.h>

#include <memory.h>

namespace
{
    const int64_t           g_u64NearbyUdpTimeoutMs       = 60000;		// if haven't received udp broadcast in these milli seconds then user is no longer nearby
    const int64_t           g_u64OnlineStatusTimeoutMs    = 600000;	// if haven't had udp or tcp activity in these milli seconds then user is no longer online
}

//============================================================================
P2PEngineVersion::P2PEngineVersion()
    : m_u8P2PEngineVersion( P2P_ENGINE_VERSION )
{
}

//============================================================================
uint8_t P2PEngineVersion::getP2PEngineVersion( void )
{ 
    return m_u8P2PEngineVersion;
};

//============================================================================
void P2PEngineVersion::getP2PEngineVersion( std::string& strRetP2PEngineVersion )
{ 
    return StdStringFormat( strRetP2PEngineVersion, "%d.%d", (m_u8P2PEngineVersion >> 4)&0x0f,  m_u8P2PEngineVersion & 0x0f );
}

//============================================================================
MyOSVersion::MyOSVersion()
{
#if defined( TARGET_OS_WINDOWS )
	m_u8OSVersion = 1;
#elif defined( TARGET_OS_ANDROID )
	m_u8OSVersion = 2;
#elif defined( TARGET_OS_LINUX )
    m_u8OSVersion = 3;
#elif defined( TARGET_OS_APPLE )
    m_u8OSVersion = 4;
#elif defined( TARGET_OS_RASBERRY_PI )
    m_u8OSVersion = 5;
#else
    m_u8OSVersion = 0;
#endif
}

//============================================================================
uint8_t MyOSVersion::getOSVersion( void )
{
	return m_u8OSVersion;
}

//============================================================================
void MyOSVersion::getOSVersion( std::string& strRetOSVersion )
{
	switch( m_u8OSVersion )
	{
	case 1:
		strRetOSVersion = "Windows";
		break;
	case 2:
		strRetOSVersion = "Android";
		break;
	case 3:
		strRetOSVersion = "Linux";
		break;
    case 4:
        strRetOSVersion = "Apple";
        break;
    case 5:
        strRetOSVersion = "RasberryPi";
        break;
	default:
		strRetOSVersion = "Unknown OS";
	}
}

//============================================================================
PktAnnActionData::PktAnnActionData()
	: m_u8TimeToLive( 0 )
	, m_u8RequestFlags(0)
	, m_u32ActionRes1(0)
	, m_u32ActionRes2(0)								
	, m_u32ActionRes3(0)
	, m_u32ActionRes4(0)
	, m_u32ActionRes5(0)	
	, m_u32ActionRes6(0)	
	, m_u32ActionRes7(0)
	, m_u32ActionRes8(0)
{
}

//============================================================================
void PktAnnActionData::setIsTopTenRequested( bool enable )
{
	if( enable )(m_u8RequestFlags |= FLAG_PKT_ANN_REQ_TOP_TEN); else m_u8RequestFlags &= (~FLAG_PKT_ANN_REQ_TOP_TEN); 
}

//============================================================================
bool PktAnnActionData::getIsTopTenRequested( void )
{
	return (m_u8RequestFlags & FLAG_PKT_ANN_REQ_TOP_TEN) ? true : false;
}

//============================================================================
void		PktAnnActionData::setIsPktAnnReplyRequested( bool bReqReply )		{ if( bReqReply )(m_u8RequestFlags |= FLAG_PKT_ANN_REQ_REPLY); else m_u8RequestFlags &= (~FLAG_PKT_ANN_REQ_REPLY); }
bool		PktAnnActionData::getIsPktAnnReplyRequested( void )					{ return (m_u8RequestFlags & FLAG_PKT_ANN_REQ_REPLY)?true:false; }
void		PktAnnActionData::setIsPktAnnRevConnectRequested( bool bReqConnect ){ if( bReqConnect )(m_u8RequestFlags |= FLAG_PKT_ANN_REQ_REV_CONNECT); else m_u8RequestFlags &= (~FLAG_PKT_ANN_REQ_REV_CONNECT); }
bool		PktAnnActionData::getIsPktAnnRevConnectRequested( void )			{ return (m_u8RequestFlags & FLAG_PKT_ANN_REQ_REV_CONNECT)?true:false; }
void		PktAnnActionData::setIsPktAnnStunRequested( bool bReqStun )			{ if( bReqStun )(m_u8RequestFlags |= FLAG_PKT_ANN_REQ_STUN); else m_u8RequestFlags &= (~FLAG_PKT_ANN_REQ_STUN); }
bool		PktAnnActionData::getIsPktAnnStunRequested( void )					{ return (m_u8RequestFlags & FLAG_PKT_ANN_REQ_STUN)?true:false; }

//============================================================================
PktAnnBase::PktAnnBase()
{
}

//============================================================================
bool PktAnnBase::hasFriendDataChanged( PktAnnBase * poOther ) 
{ 
	return ( 0 == memcmp( this, poOther, sizeof( PktAnnBase ) ))?0:1;
}

//============================================================================
//============================================================================
PktAnnounce::PktAnnounce()	
{ 
#if defined(DEBUG)
static bool firstTime = true;
    if( firstTime )
    {
        firstTime = false;
        size_t hdr = sizeof( VxPktHdr );
        size_t connectBaseInfo = sizeof( VxConnectBaseInfo );
        size_t connectIdent = sizeof( VxConnectIdent );
        size_t connectInfo = sizeof( VxConnectInfo );

        size_t identBase = sizeof( VxNetIdentBase );
        size_t netIdent = sizeof( VxNetIdent );
        size_t action = sizeof( PktAnnActionData );
        size_t base = sizeof( PktAnnBase );
        size_t pktAnn = sizeof( PktAnnounce );
        uint16_t remainder = sizeof( PktAnnounce ) & 0x0f;
        if( remainder )
        {
            LogMsg( LOG_ERROR, "ERROR Invalid PktAnn len %d hdr %d baseinfo %d connectinfo %d ident base %d ident %d action %d annbase %d pktann %d remainder %d\n",
                    sizeof( PktAnnounce ), hdr, connectBaseInfo, connectInfo, identBase, netIdent, action, base, pktAnn, remainder );
        }
        else
        {
            LogMsg( LOG_DEBUG, "OK PktAnn len %d hdr %d baseinfo %d connectinfo %d ident base %d ident %d action %d annbase %d pktann %d remainder %d\n",
                    sizeof( PktAnnounce ), hdr, connectBaseInfo, connectInfo, identBase, netIdent, action, base, remainder );
        }
    }
#endif // defined(DEBUG)

	setPktLength( sizeof( PktAnnounce ) );
	setPktType(  PKT_TYPE_ANNOUNCE );
	vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
PktAnnounce *	PktAnnounce::makeAnnCopy( void )
{
	vx_assert( sizeof( PktAnnounce ) ==  getPktLength() );
	vx_assert( PKT_TYPE_ANNOUNCE == getPktType() );
	char * pTemp = new char[ getPktLength() ];
	vx_assert( pTemp );
	memcpy( pTemp, this, getPktLength() );
	return ( PktAnnounce *)pTemp;
}

//============================================================================
PktAnnounce *	PktAnnounce::makeAnnReverseCopy( void )
{
	PktAnnounce * pTemp = makeAnnCopy();
	pTemp->reversePermissions();
	return pTemp;
}

//============================================================================
bool PktAnnounce::isValidPktAnn( void )
{
	return ( ( getPktLength() == sizeof( PktAnnounce ) ) &&
			 ( getPktType() == PKT_TYPE_ANNOUNCE ) );
}

//============================================================================
//! online status expires if no activity for specified time 
bool PktAnnounce::isOnlineStatusExpired( void )
{
	if( isIgnored() )
	{
		return true;
	}
	if( false == isNearbyStatusExpired() )
	{
		return false;
	}
	if( g_u64OnlineStatusTimeoutMs >= ( GetGmtTimeMs() - getTimeLastTcpContactMs() ) )
	{
		return false;
	}
	return true;
}

//============================================================================
//! determine if udp timeout has expired ( used for guest permissions and nearby status )
bool PktAnnounce::isNearbyStatusExpired( void )
{
	if( g_u64NearbyUdpTimeoutMs <= ( GetGmtTimeMs() - getTimeLastTcpContactMs() ) )
	{
		return true;
	}
	return false;
}
//============================================================================
//! if nearby then make permissions at least guest
void PktAnnounce::updateNearbyPermissions( void )
{
	if( false == isNearbyStatusExpired() )
	{
		if( false == isIgnored() )
		{
			if( eFriendStateGuest > getMyFriendshipToHim() )
			{
				setMyFriendshipToHim(eFriendStateGuest);
			}
			if( eFriendStateGuest > getHisFriendshipToMe() )
			{
				setHisFriendshipToMe(eFriendStateGuest);
			}
		}
	}
}

//============================================================================
//! dump contents of pkt announce for debug
void PktAnnounce::DebugDump( void )
{
	std::string strName;
	std::string strDesc;
	std::string strIp;
	std::string strId;

	this->getMyOnlineId( strId );
	this->getMyOnlineIPv4( strIp );
	uint16_t u16Port = this->getOnlinePort();
	strName = this->getOnlineName();
	strDesc = this->getOnlineDescription();
	//strNetwork = this->getNetworkKey();

	LogMsg( LOG_INFO, "PktAnnounce Len %d Version #%d name %s Ip %s Port %d desc %s\n",
			getPktLength(),		// packet length
			getPktVersionNum(),		// version of program
			strName.c_str(),		
			strIp.c_str(),		// IP of announcer
			u16Port,			// Port announcer listens on
			strDesc.c_str()
			);		
}

