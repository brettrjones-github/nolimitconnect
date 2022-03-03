//============================================================================
// Copyright (C) 2010 Brett R. Jones
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

#include "PluginMgr.h"
#include <GuiInterface/IToGui.h>

#include <ptop_src/ptop_engine_src/MediaProcessor/MediaProcessor.h>

#include <ptop_src/ptop_engine_src/Plugins/PluginAboutMePageClient.h>
#include <ptop_src/ptop_engine_src/Plugins/PluginAboutMePageServer.h>

#include <ptop_src/ptop_engine_src/Plugins/PluginChatRoomClient.h>
#include <ptop_src/ptop_engine_src/Plugins/PluginFileXfer.h>
#include <ptop_src/ptop_engine_src/Plugins/PluginInvalid.h>
#include <ptop_src/ptop_engine_src/Plugins/PluginMessenger.h>
#include <ptop_src/ptop_engine_src/Plugins/PluginPeerUserClient.h>
#include <ptop_src/ptop_engine_src/Plugins/PluginConnectionTestClient.h>
#include <ptop_src/ptop_engine_src/Plugins/PluginConnectionTestHost.h>
#include <ptop_src/ptop_engine_src/Plugins/PluginPeerUserHost.h>
#include <ptop_src/ptop_engine_src/Plugins/PluginServiceFileShare.h>
#include <ptop_src/ptop_engine_src/Plugins/PluginChatRoomClient.h>
#include <ptop_src/ptop_engine_src/Plugins/PluginChatRoomHost.h>
#include <ptop_src/ptop_engine_src/Plugins/PluginGroupClient.h>
#include <ptop_src/ptop_engine_src/Plugins/PluginGroupHost.h>
#include <ptop_src/ptop_engine_src/Plugins/PluginNetworkSearchList.h>
#include <ptop_src/ptop_engine_src/Plugins/PluginNetworkHost.h>
#include <ptop_src/ptop_engine_src/Plugins/PluginPushToTalk.h>
#include <ptop_src/ptop_engine_src/Plugins/PluginRandomConnectClient.h>
#include <ptop_src/ptop_engine_src/Plugins/PluginRandomConnectHost.h>
#include <ptop_src/ptop_engine_src/Plugins/PluginServiceRelay.h>
#include <ptop_src/ptop_engine_src/Plugins/PluginServiceStoryboard.h>
#include <ptop_src/ptop_engine_src/Plugins/PluginServiceWebCam.h>
#include <ptop_src/ptop_engine_src/Plugins/PluginStoryboardClient.h>
#include <ptop_src/ptop_engine_src/Plugins/PluginTruthOrDare.h>
#include <ptop_src/ptop_engine_src/Plugins/PluginVideoPhone.h>
#include <ptop_src/ptop_engine_src/Plugins/PluginVoicePhone.h>

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

#include <ptop_src/ptop_engine_src/BigListLib/BigListInfo.h>
#include <ptop_src/ptop_engine_src/NetServices/NetServiceHdr.h>
#include <NetLib/VxPeerMgr.h>
#include <NetLib/VxSktBase.h>

#include <string.h>
#include <stdarg.h>
#include <stdio.h>

//============================================================================
PluginMgr::PluginMgr( P2PEngine& engine )
: m_Engine( engine )
, m_BigListMgr( engine.getBigListMgr() )
, m_PktAnn( engine.getMyPktAnnounce() )
, m_PluginMgrInitialized( false )
, m_NetServiceUtils( engine )
{
}

//============================================================================
IToGui&	PluginMgr::getToGui( void )
{ 
    return m_Engine.getToGui(); 
}

//============================================================================
PluginMgr::~PluginMgr()
{
	std::vector<PluginBase *>::iterator iter;
	for( iter = m_aoPlugins.begin(); iter != m_aoPlugins.end(); ++iter )
	{
		PluginBase * poPlugin = *(iter);
		delete poPlugin;		
	}

	m_aoPlugins.clear();
}

//============================================================================
void PluginMgr::pluginMgrStartup( void )
{
    uint32_t startTime = (uint32_t)GetApplicationAliveMs();
	LogMsg( LOG_VERBOSE, "pluginMgrStartup start %d ms", startTime );

	PluginBase * poPlugin;
	// invalid
	poPlugin = new PluginInvalid( m_Engine, *this, &this->m_PktAnn, ePluginTypeInvalid );
	m_aoPlugins.push_back( poPlugin );

    LogModule( eLogPlugins, LOG_VERBOSE, "pluginMgrStartup create admin plugin" );
	poPlugin = new PluginInvalid( m_Engine, *this, &this->m_PktAnn, ePluginTypeAdmin );
	poPlugin->setPluginType( ePluginTypeAdmin );
	m_aoPlugins.push_back( poPlugin );

    LogModule( eLogPlugins, LOG_VERBOSE, "pluginMgrStartup create file xfer plugin" );
    poPlugin = new PluginFileXfer( m_Engine, *this, &this->m_PktAnn, ePluginTypeFileXfer );
    m_aoPlugins.push_back( poPlugin );

    LogModule( eLogPlugins, LOG_VERBOSE, "pluginMgrStartup create messenger plugin" );
    poPlugin = new PluginMessenger( m_Engine, *this, &this->m_PktAnn, ePluginTypeMessenger );
    m_aoPlugins.push_back( poPlugin );

    LogModule( eLogPlugins, LOG_VERBOSE, "pluginMgrStartup create about me server plugin" );
	poPlugin = new PluginAboutMePageServer( m_Engine, *this, &this->m_PktAnn, ePluginTypeAboutMePageServer );
	m_aoPlugins.push_back( poPlugin );

    LogModule( eLogPlugins, LOG_VERBOSE, "pluginMgrStartup create client peer user plugin" );
    poPlugin = new PluginPeerUserClient( m_Engine, *this, &this->m_PktAnn, ePluginTypeClientPeerUser );
    m_aoPlugins.push_back( poPlugin );

    LogModule( eLogPlugins, LOG_VERBOSE, "pluginMgrStartup create host peer user plugin" );
    poPlugin = new PluginPeerUserHost( m_Engine, *this, &this->m_PktAnn, ePluginTypeHostPeerUser );
    m_aoPlugins.push_back( poPlugin );

    LogModule( eLogPlugins, LOG_VERBOSE, "pluginMgrStartup create connection test client plugin" );
    poPlugin = new PluginConnectionTestClient( m_Engine, *this, &this->m_PktAnn, ePluginTypeClientConnectTest );
    m_aoPlugins.push_back( poPlugin );

    LogModule( eLogPlugins, LOG_VERBOSE, "pluginMgrStartup create connection test host plugin" );
    poPlugin = new PluginConnectionTestHost( m_Engine, *this, &this->m_PktAnn, ePluginTypeHostConnectTest );
    m_aoPlugins.push_back( poPlugin );

    LogModule( eLogPlugins, LOG_INFO, "pluginMgrStartup create file share plugin" );
    m_aoPlugins.push_back( &m_Engine.getPluginServiceFileShare() );

    LogModule( eLogPlugins, LOG_VERBOSE, "pluginMgrStartup create chat room client plugin" );
    poPlugin = new PluginChatRoomClient( m_Engine, *this, &this->m_PktAnn, ePluginTypeClientChatRoom );
    m_aoPlugins.push_back( poPlugin );

    LogModule( eLogPlugins, LOG_VERBOSE, "pluginMgrStartup create host chat room plugin" );
    poPlugin = new PluginChatRoomHost( m_Engine, *this, &this->m_PktAnn, ePluginTypeHostChatRoom );
    m_aoPlugins.push_back( poPlugin );

    LogModule( eLogPlugins, LOG_VERBOSE, "pluginMgrStartup create host client plugin" );
    poPlugin = new PluginGroupClient( m_Engine, *this, &this->m_PktAnn, ePluginTypeClientGroup );
    m_aoPlugins.push_back( poPlugin );

    LogModule( eLogPlugins, LOG_VERBOSE, "pluginMgrStartup create host group plugin" );
    poPlugin = new PluginGroupHost( m_Engine, *this, &this->m_PktAnn, ePluginTypeHostGroup );
    m_aoPlugins.push_back( poPlugin );

    LogModule( eLogPlugins, LOG_VERBOSE, "pluginMgrStartup create host group listing plugin" );
    poPlugin = new PluginNetworkSearchList( m_Engine, *this, &this->m_PktAnn, ePluginTypeNetworkSearchList );
    m_aoPlugins.push_back( poPlugin );

    LogModule( eLogPlugins, LOG_VERBOSE, "pluginMgrStartup create host network plugin" );
    poPlugin = new PluginNetworkHost( m_Engine, *this, &this->m_PktAnn, ePluginTypeHostNetwork );
    m_aoPlugins.push_back( poPlugin );

    LogModule( eLogPlugins, LOG_VERBOSE, "pluginMgrStartup create random connect client plugin" );
    poPlugin = new PluginRandomConnectClient( m_Engine, *this, &this->m_PktAnn, ePluginTypeClientRandomConnect );
    m_aoPlugins.push_back( poPlugin );

    LogModule( eLogPlugins, LOG_VERBOSE, "pluginMgrStartup create random connect host plugin" );
    poPlugin = new PluginRandomConnectHost( m_Engine, *this, &this->m_PktAnn, ePluginTypeHostRandomConnect );
    m_aoPlugins.push_back( poPlugin );

	// relay pre created by engine
    LogModule( eLogPlugins, LOG_VERBOSE, "pluginMgrStartup create relay plugin" );
    m_aoPlugins.push_back( &m_Engine.getPluginServiceRelay() );

    LogModule( eLogPlugins, LOG_VERBOSE, "pluginMgrStartup create storyboard server plugin" );
    poPlugin = new PluginServiceStoryboard( m_Engine, *this, &this->m_PktAnn, ePluginTypeStoryboardServer );
    m_aoPlugins.push_back( poPlugin );

    LogModule( eLogPlugins, LOG_VERBOSE, "pluginMgrStartup create cam server plugin" );
	poPlugin = new PluginServiceWebCam( m_Engine, *this, &this->m_PktAnn, ePluginTypeCamServer );
	m_aoPlugins.push_back( poPlugin );

    LogModule( eLogPlugins, LOG_INFO, "pluginMgrStartup create voice phone plugin" );
	poPlugin = new PluginVoicePhone( m_Engine, *this, &this->m_PktAnn, ePluginTypeVoicePhone );
	m_aoPlugins.push_back( poPlugin );

	LogModule( eLogPlugins, LOG_INFO, "pluginMgrStartup create push to talk plugin" );
	poPlugin = new PluginPushToTalk( m_Engine, *this, &this->m_PktAnn, ePluginTypePushToTalk );
	m_aoPlugins.push_back( poPlugin );

    LogModule( eLogPlugins, LOG_INFO, "pluginMgrStartup create video phone plugin" );
	poPlugin = new PluginVideoPhone( m_Engine, *this, &this->m_PktAnn, ePluginTypeVideoPhone );
	m_aoPlugins.push_back( poPlugin );

    LogModule( eLogPlugins, LOG_INFO, "pluginMgrStartup create truth or dare plugin" );
	poPlugin = new PluginTruthOrDare( m_Engine, *this, &this->m_PktAnn, ePluginTypeTruthOrDare );
	m_aoPlugins.push_back( poPlugin );

	LogModule( eLogPlugins, LOG_INFO, "pluginMgrStartup create about me viewer plugin" );
	poPlugin = new PluginAboutMePageClient( m_Engine, *this, &this->m_PktAnn, ePluginTypeAboutMePageClient );
	m_aoPlugins.push_back( poPlugin );

	LogModule( eLogPlugins, LOG_INFO, "pluginMgrStartup create about me viewer plugin" );
	poPlugin = new PluginStoryboardClient( m_Engine, *this, &this->m_PktAnn, ePluginTypeStoryboardClient );
	m_aoPlugins.push_back( poPlugin );

    LogModule( eLogPlugins, LOG_INFO, "pluginMgrStartup adding net services" );
	// net services pre created by engine
	m_aoPlugins.push_back( &m_Engine.getPluginNetServices() );

	m_PluginMgrInitialized = true;

    uint32_t endTime = ( uint32_t)GetApplicationAliveMs();
    LogMsg( LOG_INFO, "pluginMgrStartup done in %d ms at %d ms", endTime - startTime, endTime );
}

//============================================================================
void PluginMgr::pluginMgrShutdown( void )
{
	//SetRelayPluginInstance( NULL );
	if( m_PluginMgrInitialized )
	{
		m_PluginMgrInitialized = false;
		std::vector<PluginBase *>::iterator iter;
		for( iter = m_aoPlugins.begin(); iter != m_aoPlugins.end(); ++iter )
		{
			PluginBase * poPlugin = *(iter);
			poPlugin->pluginShutdown();
		}
	}
}

//============================================================================
void PluginMgr::fromGuiNetworkAvailable( void )
{
	std::vector<PluginBase * >::iterator iter;
	for( iter = m_aoPlugins.begin(); iter != m_aoPlugins.end(); ++iter )
	{
		(*iter)->fromGuiAppResume();
	}
}

//============================================================================
//! get plugin state 
EAppState PluginMgr::getPluginState( EPluginType ePluginType )								
{ 
	PluginBase * plugin = getPlugin( ePluginType );
	if( plugin )
	{
		return plugin->getPluginState();
	}

	return eAppStateInvalid;
}

//============================================================================
PluginBase* PluginMgr::getPlugin( EPluginType ePluginType )
{
	std::vector<PluginBase *>::iterator iter;
	for( iter = m_aoPlugins.begin(); iter != m_aoPlugins.end(); ++iter )
	{
		PluginBase * plugin = *iter;
		if( ePluginType == plugin->getPluginType() )
		{
			return *(iter);
		}
	}

	LogMsg( LOG_ERROR, "PluginMgr::getPlugin pluin type %d out of range", ePluginType );
	vx_assert( false );
	return NULL;
}

//============================================================================
//! set plugin state 
void PluginMgr::setPluginState( EPluginType ePluginType, EAppState ePluginState )		
{ 
	PluginBase * plugin = getPlugin( ePluginType );
	if( plugin )
	{
		plugin->setPluginState( ePluginState );
	}
}

//============================================================================
EFriendState PluginMgr::getPluginPermission( EPluginType ePluginType )							
{ 
	PluginBase * plugin = getPlugin( ePluginType );
	if( plugin )
	{
		return plugin->getPluginPermission(); 
	}

	return eFriendStateIgnore;
}

//============================================================================
void PluginMgr::setPluginPermission( EPluginType ePluginType, EFriendState ePluginPermission )	
{ 
	PluginBase * plugin = getPlugin( ePluginType );
	if( plugin )
	{
		plugin->setPluginPermission( ePluginPermission ); 
	}
}

//============================================================================
bool PluginMgr::setPluginSetting( PluginSetting& pluginSetting, int64_t modifiedTimeMs )
{
    bool result = false;
    PluginBase * plugin = getPlugin( pluginSetting.getPluginType() );
    if( plugin )
    {
        result = plugin->setPluginSetting( pluginSetting, modifiedTimeMs );
    }

    return result;
}

//============================================================================
void PluginMgr::onPluginSettingChange( PluginSetting& pluginSetting, int64_t modifiedTimeMs )
{
    PluginBase * plugin = getPlugin( pluginSetting.getPluginType() );
    if( plugin )
    {
        plugin->setPluginSetting( pluginSetting, modifiedTimeMs );
    }
}

//============================================================================
void PluginMgr::pluginApiLog( EPluginType ePluginType, const char * pMsg, ... )
{
	char szBuffer[2048];
	va_list argList;
	va_start(argList, pMsg);
	vsnprintf( szBuffer, 2048, pMsg, argList );
	va_end(argList);
	LogMsg( (ePluginType << 16) | LOG_INFO, "Plugin %d %s", (int)ePluginType, szBuffer );
}

//============================================================================
bool PluginMgr::handleFirstWebPageConnection( VxSktBase * sktBase )
{
	char *	pSktBuf = (char *)sktBase->getSktReadBuf();
	int	iDataLen =	sktBase->getSktBufDataLen();
	if( iDataLen < (37 + 16 ) )
	{
		// should at least be GET /xxxxxxxxxxxxxxxxxxxxx/ where the x's are the online id + GET/file name
		sktBase->sktBufAmountRead( 0 );
		return false;
	}

	pSktBuf[ iDataLen ] = 0;
	int parseOffset = 0;
	if( 0 == strncmp( pSktBuf, "http://", 7 ) )
	{
		parseOffset = 7;
	}

	if( 0 != strncmp( &pSktBuf[ parseOffset ], "GET /", 5 ) )
	{
		sktBase->sktBufAmountRead( 0 );
		return false;
	}

	parseOffset += 5;
	if( '/' != pSktBuf[ parseOffset + 32 ] )
	{
		sktBase->sktBufAmountRead( 0 );
		return false;
	}

	VxNetIdent * netIdent = NULL;
	VxGUID onlineId;
	if( false == onlineId.fromVxGUIDHexString( &pSktBuf[ parseOffset ] ) )
	{
		sktBase->sktBufAmountRead( 0 );
		return false;
	}

	if( onlineId == m_Engine.getMyPktAnnounce().getMyOnlineId() )
	{
		netIdent = &m_Engine.getMyPktAnnounce();
	}
	else
	{
		netIdent = m_Engine.getBigListMgr().findBigListInfo( onlineId );
	}

	if( NULL == netIdent )
	{
		sktBase->sktBufAmountRead( 0 );
		return false;
	}

	parseOffset += 33;
	sktBase->sktBufAmountRead( 0 );
	PluginBase * poPlugin = getPlugin( ePluginTypeWebServer );
	if( poPlugin )
	{
		sktBase->sktBufAmountRead( 0 );		
		if( 0 == poPlugin->handlePtopConnection( sktBase, netIdent ) )
		{
			return true;
		}
		else
		{
			return true;
		}
	}
	
	sktBase->sktBufAmountRead( 0 );
	return false;
}

//============================================================================
void PluginMgr::handleFirstNetServiceConnection( VxSktBase * sktBase )
{
	int iSktDataLen = sktBase->getSktBufDataLen();
	if( iSktDataLen < NET_SERVICE_HDR_LEN )
	{
		// not even header has arrived so return
		return;
	}
	
	char *	pSktBuf = (char *)sktBase->getSktReadBuf();
	int urlLen = m_NetServiceUtils.getTotalLengthFromNetServiceUrl( pSktBuf, iSktDataLen );
	if( 0 >= urlLen )
	{
		sktBase->sktBufAmountRead( 0 );
		LogMsg( LOG_ERROR, "handleFirstNetServiceConnection: not valid" );
		VxReportHack( eHackerLevelMedium, eHackerReasonNetSrvUrlInvalid, sktBase, "handleFirstNetServiceConnection: invalid url" );
		sktBase->closeSkt( eSktCloseNetSrvUrlInvalid );
		return;
	}

	sktBase->sktBufAmountRead( 0 );
	if( urlLen > iSktDataLen )
	{
		// not all of data here yet
		return;
	}

	bool httpConnectionWasHandled = false;
	sktBase->setIsFirstRxPacket( false );

	NetServiceHdr netServiceHdr;
	EPluginType pluginType = m_NetServiceUtils.parseHttpNetServiceUrl( sktBase, netServiceHdr );
	if( netServiceHdr.m_NetCmdType == eNetCmdHostPingReq )
	{
		if( m_Engine.getHasAnyAnnonymousHostService() )
		{
			std::string content = sktBase->getRemoteIp();
			m_NetServiceUtils.buildAndSendCmd( sktBase, eNetCmdHostPingReply, content );
			// flush then close
			sktBase->closeSkt( eSktCloseNetSrvHostPingReplySent, true );
		}
		else
		{
			LogMsg( LOG_ERROR, "handleFirstNetServiceConnection: connection %s dropped because Host Ping Request Not Allowed", sktBase->getRemoteIp().c_str() );
			// flush then close
			sktBase->closeSkt( eSktCloseNetSrvQueryIdPermission, false );
		}

		return;
	}


    if( ( netServiceHdr.m_NetCmdType == eNetCmdQueryHostOnlineIdReq ) &&
        ( ePluginTypeHostNetwork == pluginType || ePluginTypeHostGroup == pluginType || 
            ePluginTypeHostChatRoom == pluginType || ePluginTypeHostRandomConnect == pluginType || ePluginTypeHostConnectTest == pluginType) )
    {
        // only allowed if Hosting feature is enabled
        PluginBase * poPlugin = getPlugin( pluginType );
        if( poPlugin )
        {
			bool hasAnnonService{ false };
			std::string onlineId;
			if( m_Engine.getHasAnyAnnonymousHostService() )
			{
				// if we have any anonymous hosting we send back valid id but will still set error if plugin is disabled
				onlineId = m_Engine.getMyOnlineId().toHexString();
				hasAnnonService = true;
			}
			else
			{
				VxGUID nullGuid;
				onlineId = nullGuid.toHexString();
			}

            if( eAppStatePermissionErr != poPlugin->getPluginState() )
            {
                std::string onlineId = m_Engine.getMyOnlineId().toHexString();
				m_NetServiceUtils.buildAndSendCmd( sktBase, eNetCmdQueryHostOnlineIdReply, onlineId );
				// flush then close
				sktBase->closeSkt( eSktCloseNetSrvQueryIdSent, true );
            }
            else
            {
				if( !hasAnnonService )
				{
					m_Engine.hackerOffense( eHackerLevelSuspicious, eHackerReasonNetSrvQueryIdPermission, nullptr, sktBase->getRemoteIpBinary(), "Hacker http attack from ip %s query host ID not allowed", sktBase->getRemoteIp().c_str() );
				}
   
                m_NetServiceUtils.buildAndSendCmd( sktBase, eNetCmdQueryHostOnlineIdReply, onlineId, ( eFriendStateIgnore == poPlugin->getPluginPermission() ) ? eNetCmdErrorServiceDisabled : eNetCmdErrorPermissionLevel );
                sktBase->dumpSocketStats();
                // flush then close
                sktBase->closeSkt( eSktCloseNetSrvQueryIdPermission, true );
            }
        }
        else
        {
            m_Engine.hackerOffense( eHackerLevelMedium, eHackerReasonNetSrvUrlInvalid, nullptr, sktBase->getRemoteIpBinary(), "Hacker http attack from ip %s invalid plugin", sktBase->getRemoteIp().c_str() );
            sktBase->closeSkt( eSktCloseNetSrvPluginInvalid, false );
        }

        return;
    }

	if( ePluginTypeInvalid != pluginType )
	{
		VxNetIdent * netIdent = NULL;
		if( netServiceHdr.m_OnlineId == m_Engine.getMyPktAnnounce().getMyOnlineId() )
		{
			netIdent = &m_Engine.getMyPktAnnounce();
			LogMsg( LOG_INFO, "PluginMgr::handleFirstNetServiceConnection: parseOnlineId was myself" );
		}
		else
		{
			netIdent = m_Engine.getBigListMgr().findBigListInfo( netServiceHdr.m_OnlineId );
		}
	
		netServiceHdr.m_Ident = netIdent;
		PluginBase * poPlugin = getPlugin( pluginType );
		if( poPlugin )
		{
			RCODE rc = -1;
			if( ePluginTypeNetServices == poPlugin->getPluginType() || ePluginTypeHostConnectTest == poPlugin->getPluginType() )
			{
				rc = poPlugin->handlePtopConnection( sktBase, netServiceHdr );
			}

			if( 0 == rc )
			{
				// socket was handled
				httpConnectionWasHandled = true;
			}
		}
		else
		{
			LogMsg( LOG_INFO, "PluginMgr::handleFirstNetServiceConnection; unknown plugin type" );
            m_Engine.hackerOffense( eHackerLevelMedium, eHackerReasonNetSrvPluginInvalid, nullptr, sktBase->getRemoteIpBinary(), "Hacker http attack (unknown plugin)from ip %s", sktBase->getRemoteIp().c_str() );
            sktBase->dumpSocketStats();
			sktBase->closeSkt( eSktCloseHttpPluginInvalid );
		}
	}

	if( false == httpConnectionWasHandled )
	{
        m_Engine.hackerOffense( eHackerLevelSevere, eHackerReasonHttpAttack, nullptr, sktBase->getRemoteIpBinary(), "Hacker http attack from ip %s", sktBase->getRemoteIp().c_str() );
        sktBase->dumpSocketStats();
		sktBase->closeSkt( eSktCloseHttpHandleError );
	}
}

//============================================================================
//! this is called for all valid packets that are not sys packets
void PluginMgr::handleNonSystemPackets( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	//LogMsg( LOG_INFO, "PluginMgr::handleNonSystemPackets" );
	uint8_t u8PluginNum = pktHdr->getPluginNum();
	if( isValidPluginNum( u8PluginNum ) )
	{
		PluginBase * plugin = getPlugin( (EPluginType)u8PluginNum );
		VxNetIdent * netIdent = pktHdr->getSrcOnlineId() == m_Engine.getMyOnlineId() ? m_Engine.getMyNetIdent() : m_BigListMgr.findBigListInfo( pktHdr->getSrcOnlineId() );
		if( netIdent && plugin )
		{
			plugin->handlePkt( sktBase, pktHdr, netIdent );
		}
		else // TODO BRJ handle case of valid netIdent not needed?
		{
			LogMsg( LOG_ERROR, "PluginMgr::handleNonSystemPackets unknown ident 0x%llX 0x%llX", 
				pktHdr->getSrcOnlineId().getVxGUIDHiPart(),
				pktHdr->getSrcOnlineId().getVxGUIDLoPart() );
		}
	}
	else
	{
		LogMsg( LOG_ERROR, "PluginMgr::handleNonSystemPackets invalid plugin num %d", u8PluginNum );
	}
}

//============================================================================
bool PluginMgr::isValidPluginNum( uint8_t u8PluginNum )
{
	return ((ePluginTypeInvalid < u8PluginNum ) && (eMaxPluginType > u8PluginNum )) ? true : false;
}

//============================================================================
//! get permission/access state for remote user
EPluginAccess PluginMgr::pluginApiGetPluginAccessState( EPluginType ePluginType, VxNetIdent * netIdent )
{
	PluginBase * plugin = getPlugin( ePluginType );
	if( plugin )
	{
		EPluginAccess eAccess = m_PktAnn.getHisAccessPermissionFromMe( ePluginType );
		if( ePluginAccessOk == eAccess )
		{
			eAccess = plugin->canAcceptNewSession( netIdent );
		}

		return eAccess;
	}

	return ePluginAccessDisabled;
}

//============================================================================
VxNetIdent * PluginMgr::pluginApiGetMyIdentity( void )
{
	return &m_PktAnn;
}

//============================================================================
VxNetIdent * PluginMgr::pluginApiFindUser( const char * pUserName )
{
	return m_BigListMgr.findBigListInfo( pUserName );
}

//============================================================================
void PluginMgr::fromGuiUserLoggedOn( void )
{
    vx_assert( m_PluginMgrInitialized );
	// set all plugin permissions
	std::vector<PluginBase * >::iterator iter;
	for( PluginBase * pluginBase : m_aoPlugins )
	{
        vx_assert( pluginBase );
        pluginBase->setPluginPermission( m_PktAnn.getPluginPermission( pluginBase->getPluginType() ) );
	}

	// now tell plugins we are logged on
	for( iter = m_aoPlugins.begin(); iter != m_aoPlugins.end(); ++iter )
	{
		PluginBase * pluginBase = ( *iter );
        vx_assert( pluginBase );
		pluginBase->fromGuiUserLoggedOn();
	}

	// tell all plugins to startup
	onAppStateChange( eAppStateStartup );
}

//============================================================================
void PluginMgr::onAppStateChange( EAppState eAppState )
{
	switch( eAppState )
	{
	case eAppStateStartup:
		onAppStartup();
		break;
	case eAppStateShutdown:
		onAppShutdown();
		break;
	case eAppStatePause:
		fromGuiAppPause();
		break;
	case eAppStateResume:
		fromGuiAppResume();
		break;
	default:
		break;
	}
}

//============================================================================
void PluginMgr::fromGuiAppIdle( void )
{
	std::vector<PluginBase * >::iterator iter;
	for( iter = m_aoPlugins.begin(); iter != m_aoPlugins.end(); ++iter )
	{
		(*iter)->fromGuiAppIdle();
	}
}

//============================================================================
void PluginMgr::onAppStartup( void )
{
	std::vector<PluginBase * >::iterator iter;
	int pluginIdx = 0;
	for( iter = m_aoPlugins.begin(); iter != m_aoPlugins.end(); ++iter )
	{
        //LogMsg( LOG_INFO, "pluginMgr::onAppStartup idx %d\n", pluginIdx );
		PluginBase * pluginBase = (*iter);
		pluginBase->onAppStartup();
		pluginIdx++;
	}
}

//============================================================================
void PluginMgr::onAppShutdown( void )
{
	std::vector<PluginBase * >::iterator iter;
	for( iter = m_aoPlugins.begin(); iter != m_aoPlugins.end(); ++iter )
	{
		(*iter)->onAppShutdown();
	}
}

//============================================================================
void PluginMgr::fromGuiAppPause( void )
{
	std::vector<PluginBase * >::iterator iter;
	for( iter = m_aoPlugins.begin(); iter != m_aoPlugins.end(); ++iter )
	{
		(*iter)->fromGuiAppPause();
	}
}

//============================================================================
void PluginMgr::fromGuiAppResume( void )
{
	std::vector<PluginBase * >::iterator iter;
	for( iter = m_aoPlugins.begin(); iter != m_aoPlugins.end(); ++iter )
	{
		(*iter)->fromGuiAppResume();
	}
}

//============================================================================
void PluginMgr::fromGuiListAction( EListAction listAction )
{
	std::vector<PluginBase* >::iterator iter;
	for( iter = m_aoPlugins.begin(); iter != m_aoPlugins.end(); ++iter )
	{
		( *iter )->fromGuiListAction( listAction );
	}
}

//============================================================================
void PluginMgr::onMyOnlineUrlIsValid( bool iValid )
{
	std::vector<PluginBase* >::iterator iter;
	for( iter = m_aoPlugins.begin(); iter != m_aoPlugins.end(); ++iter )
	{
		( *iter )->onMyOnlineUrlIsValid( iValid );
	}
}

//============================================================================
void PluginMgr::onOncePerSecond( void )
{
	//NOTE: TODO ?
	//std::vector<PluginBase * >::iterator iter;
	//for( iter = m_aoPlugins.begin(); iter != m_aoPlugins.end(); ++iter )
	//{
	//	(*iter)->onOncePerSecond();
	//}
}

//============================================================================
void PluginMgr::onThreadOncePer15Minutes( void )
{
    std::vector<PluginBase * >::iterator iter;
    for( iter = m_aoPlugins.begin(); iter != m_aoPlugins.end(); ++iter )
    {
		if( !VxIsAppShuttingDown() )
		{
			( *iter )->onThreadOncePer15Minutes();
		}
		else
		{
			break;
		}
    }
}

//============================================================================
void PluginMgr::onAfterUserLogOnThreaded( void )
{
	std::vector<PluginBase* >::iterator iter;
	for( iter = m_aoPlugins.begin(); iter != m_aoPlugins.end(); ++iter )
	{
		if( !VxIsAppShuttingDown() )
		{
			( *iter )->onAfterUserLogOnThreaded();
		}
		else
		{
			break;
		}
	}
}

//============================================================================
void PluginMgr::onContactWentOnline( VxNetIdent * netIdent, VxSktBase * sktBase )
{
	std::vector<PluginBase * >::iterator iter;
	for( iter = m_aoPlugins.begin(); iter != m_aoPlugins.end(); ++iter )
	{
		(*iter)->onContactWentOnline( netIdent, sktBase );
	}
}

//============================================================================
void PluginMgr::onContactWentOffline( VxNetIdent * netIdent, VxSktBase * sktBase )
{
	std::vector<PluginBase * >::iterator iter;
	for( iter = m_aoPlugins.begin(); iter != m_aoPlugins.end(); ++iter )
	{
		(*iter)->onContactWentOffline( netIdent, sktBase );
	}
}

//============================================================================
void PluginMgr::onConnectionLost( VxSktBase * sktBase )
{
	std::vector<PluginBase * >::iterator iter;
	for( iter = m_aoPlugins.begin(); iter != m_aoPlugins.end(); ++iter )
	{
		(*iter)->onConnectionLost( sktBase );
	}
}

//============================================================================
void PluginMgr::fromGuiRelayPermissionCount( int userPermittedCount, int anonymousCount )
{
	std::vector<PluginBase * >::iterator iter;
	for( iter = m_aoPlugins.begin(); iter != m_aoPlugins.end(); ++iter )
	{
		(*iter)->fromGuiRelayPermissionCount( userPermittedCount, anonymousCount );
	}
}

//============================================================================
void PluginMgr::fromGuiSendAsset( AssetBaseInfo& assetInfo )
{
	std::vector<PluginBase * >::iterator iter;
	for( iter = m_aoPlugins.begin(); iter != m_aoPlugins.end(); ++iter )
	{
		(*iter)->fromGuiSendAsset( assetInfo );
	}
}

//============================================================================
PluginBase* PluginMgr::findPlugin( EPluginType ePluginType )
{
	std::vector<PluginBase * >::iterator iter;
	for( iter = m_aoPlugins.begin(); iter != m_aoPlugins.end(); ++iter )
	{
		if( ePluginType == (*iter)->getPluginType() )
		{
			return (*iter);
		}
	}

	return nullptr;
}

//============================================================================
PluginBase* PluginMgr::findHostClientPlugin( EHostType hostType )
{
    return  hostClientToPlugin( hostType );
}

//============================================================================
PluginBase* PluginMgr::findHostServicePlugin( EHostType hostType )
{
    return hostServiceToPlugin( hostType );
}

//============================================================================
PluginBase* PluginMgr::hostClientToPlugin( EHostType hostType )
{
    switch( hostType )
    {
    case eHostTypeChatRoom:
        return findPlugin( ePluginTypeClientChatRoom );
    case eHostTypeConnectTest:
        return findPlugin( ePluginTypeClientConnectTest );
    case eHostTypeGroup:
        return findPlugin( ePluginTypeClientGroup );
    case eHostTypeNetwork:
        return findPlugin( ePluginTypeClientNetwork );
    case eHostTypeRandomConnect:
        return findPlugin( ePluginTypeClientRandomConnect );
    default:
        return nullptr;
    }
}

//============================================================================
PluginBase* PluginMgr::hostServiceToPlugin( EHostType hostType )
{
    switch( hostType )
    {
    case eHostTypeChatRoom:
        return findPlugin( ePluginTypeHostChatRoom );
    case eHostTypeConnectTest:
        return findPlugin( ePluginTypeHostConnectTest );
    case eHostTypeGroup:
        return findPlugin( ePluginTypeHostGroup );
    case eHostTypeNetwork:
        return findPlugin( ePluginTypeHostNetwork );
    case eHostTypeRandomConnect:
        return findPlugin( ePluginTypeHostRandomConnect );
    default:
        return nullptr;
    }
}

//============================================================================
bool PluginMgr::fromGuiMultiSessionAction( EMSessionAction mSessionAction, VxGUID& onlineId, int pos0to100000, VxGUID lclSessionId )
{
	bool result = false;
	PluginBase * plugin = findPlugin( ePluginTypeMessenger );
	if( plugin )
	{
		BigListInfo * bigInfo = m_BigListMgr.findBigListInfo( onlineId );
		if( bigInfo )
		{
			result = plugin->fromGuiMultiSessionAction( bigInfo, mSessionAction, pos0to100000, lclSessionId );
		}
	}

	return result;
}

//============================================================================
//! return true if access ok
bool PluginMgr::canAccessPlugin( EPluginType ePluginType, VxNetIdent * netIdent )
{
	LogMsg( LOG_INFO, "PluginMgr::canAccessPlugin\n" );
	EFriendState eHisFriendshipToMe = netIdent->getHisFriendshipToMe();
	EFriendState ePluginPermission = netIdent->getPluginPermission(ePluginType);
	if( ( ePluginPermission != eFriendStateIgnore ) &&
		( ePluginPermission <= eHisFriendshipToMe ) )
	{
		return true;
	}

	return false;
}

//============================================================================
void PluginMgr::pluginApiPlayVideoFrame( EPluginType ePluginType, uint8_t * pu8VidData, uint32_t u32VidDataLen, VxNetIdent * netIdent, int motion0to100000 )
{
	//LogMsg( LOG_INFO, "PluginMgr::pluginApiPlayVideoFrame\n" );
	IToGui::getToGui().toGuiPlayVideoFrame( netIdent->getMyOnlineId(), pu8VidData, u32VidDataLen, motion0to100000 );
}

//============================================================================
void PluginMgr::pluginApiWantAppIdle( EPluginType ePluginType, bool bWantAppIdle )
{
	m_Engine.getMediaProcesser().wantAppIdle( ePluginType, bWantAppIdle );
}

//============================================================================
void PluginMgr::pluginApiWantMediaInput( EPluginType ePluginType, EMediaInputType mediaType, bool wantInput, void * userData )
{
	PluginBase * plugin = getPlugin( ePluginType );
	if( plugin )
	{
		m_Engine.getMediaProcesser().wantMediaInput( mediaType, plugin, userData, wantInput );
	}
}

//============================================================================
//! called to start service or session with remote friend
void PluginMgr::fromGuiStartPluginSession( EPluginType ePluginType,  VxGUID& oOnlineId, int pvUserData, VxGUID lclSessionId )
{
	PluginBase * plugin = getPlugin( ePluginType );
	if( plugin )
	{
		BigListInfo * poInfo = m_BigListMgr.findBigListInfo( oOnlineId );
		if( poInfo )
		{
			plugin->fromGuiStartPluginSession( poInfo, pvUserData, lclSessionId );	
		}
		else if( oOnlineId == m_PktAnn.getMyOnlineId() )
		{
			plugin->fromGuiStartPluginSession( &m_PktAnn, pvUserData, lclSessionId );	
		}
		else
		{
			LogMsg( LOG_ERROR, "PluginMgr::fromGuiStartPluginSession: id not found NOT FOUND %s my id %s\n", 
				oOnlineId.describeVxGUID().c_str(), 
				m_PktAnn.getMyOnlineId().describeVxGUID().c_str() );
		}
	}
	else
	{
		LogMsg( LOG_ERROR, "PluginMgr::fromGuiStartPluginSession: invalid plugin type %d\n", ePluginType );
	}	
}

//============================================================================
//! called to stop service or session with remote friend
void PluginMgr::fromGuiStopPluginSession( EPluginType ePluginType, VxGUID& onlineId, int pvUserData, VxGUID lclSessionId )
{
	PluginBase * plugin = getPlugin( ePluginType );
	if( plugin )
	{
		BigListInfo * poInfo = m_BigListMgr.findBigListInfo( onlineId );
		if( poInfo )
		{
			plugin->fromGuiStopPluginSession( poInfo, pvUserData, lclSessionId );	
		}
		else if( onlineId == m_PktAnn.getMyOnlineId() )
		{
			plugin->fromGuiStopPluginSession( &m_PktAnn, pvUserData, lclSessionId );	
		}
		else
		{
			LogMsg( LOG_ERROR, "PluginMgr::fromGuiStopPluginSession: id not found NOT FOUND\n");
		}
	}
	else
	{
		LogMsg( LOG_ERROR, "PluginMgr::fromGuiStopPluginSession: invalid plugin type %d\n", ePluginType );
	}
}

//============================================================================
//! return true if is plugin session
bool PluginMgr::fromGuiIsPluginInSession( EPluginType ePluginType, VxNetIdent * netIdent, int pvUserData, VxGUID lclSessionId )
{
	bool inSession = false;
	PluginBase * plugin = getPlugin( ePluginType );
	if( plugin )
	{
		inSession = plugin->fromGuiIsPluginInSession( netIdent, pvUserData, lclSessionId );	
	}

	return inSession;
}

//============================================================================
bool PluginMgr::fromGuiSetGameValueVar(	    EPluginType	    ePluginType, 
											VxGUID&	        onlineId, 
											int32_t			s32VarId, 
											int32_t			s32VarValue )
{
	bool bResult = false;
	PluginBase * plugin = getPlugin( ePluginType );
	if( plugin )
	{
		BigListInfo * poInfo = m_BigListMgr.findBigListInfo( onlineId );
		if( poInfo )
		{
			bResult = plugin->fromGuiSetGameValueVar( poInfo, s32VarId, s32VarValue );	
		}
		else if( onlineId == m_PktAnn.getMyOnlineId() )
		{
			bResult = plugin->fromGuiSetGameValueVar( &m_PktAnn, s32VarId, s32VarValue );	
		}
		else
		{
			LogMsg( LOG_ERROR, "PluginMgr::fromGuiSetGameActionVar: id not found NOT FOUND\n");
		}
	}
	return bResult;
}

//============================================================================
bool PluginMgr::fromGuiSetGameActionVar(	EPluginType	    ePluginType, 
											VxGUID&		    onlineId,
											int32_t			s32VarId, 
											int32_t			s32VarValue )
{
	bool bResult = false;
	PluginBase * plugin = getPlugin( ePluginType );
	if( plugin )
	{
		BigListInfo * poInfo = m_BigListMgr.findBigListInfo( onlineId );
		if( poInfo )
		{
			bResult = plugin->fromGuiSetGameActionVar( poInfo, s32VarId, s32VarValue );	
		}
		else if( onlineId == m_PktAnn.getMyOnlineId() )
		{
			bResult = plugin->fromGuiSetGameActionVar( &m_PktAnn, s32VarId, s32VarValue );	
		}
		else
		{
			LogMsg( LOG_ERROR, "PluginMgr::fromGuiSetGameActionVar: id not found NOT FOUND\n");
		}
	}
	return bResult;
}

//============================================================================
int PluginMgr::fromGuiDeleteFile( const char * fileName, bool shredFile )
{
	std::vector<PluginBase * >::iterator iter;
	for( iter = m_aoPlugins.begin(); iter != m_aoPlugins.end(); ++iter )
	{
		(*iter)->fromGuiDeleteFile( fileName, shredFile );
	}

	return 0;
}

//============================================================================
EPluginAccess PluginMgr::canAcceptNewSession( EPluginType ePluginType, VxNetIdent * netIdent )
{
	EPluginAccess canAcceptSession = ePluginAccessDisabled;
	PluginBase * plugin = getPlugin( ePluginType );
	if( plugin )
	{
		canAcceptSession = plugin->canAcceptNewSession( netIdent );
	}

	return canAcceptSession;
}

//============================================================================
//! get identity from socket connection
VxNetIdent * PluginMgr::pluginApiOnlineIdToIdentity( VxGUID& oOnlineId )
{
	BigListInfo * poInfo = m_BigListMgr.findBigListInfo( oOnlineId );
	if( poInfo )
	{
		return poInfo;
	}
	LogMsg( LOG_ERROR, "PluginMgr::pluginApiSktToIdentity: NOT FOUND\n");
	return NULL;
}

//============================================================================
void PluginMgr::replaceConnection( VxNetIdent * netIdent, VxSktBase * poOldSkt, VxSktBase * poNewSkt )
{
	std::vector<PluginBase *>::iterator iter;
	for( iter = m_aoPlugins.begin(); iter != m_aoPlugins.end(); ++iter )
	{
		PluginBase * poPlugin = *(iter);
		poPlugin->replaceConnection( netIdent, poOldSkt, poNewSkt );
	}
}

//============================================================================
void PluginMgr::leavePreviousHost( GroupieId& groupieId )
{
	PluginBaseHostClient* plugin = dynamic_cast< PluginBaseHostClient* >( findPlugin( HostTypeToClientPlugin( groupieId.getHostType() ) ) );
	if( plugin )
	{
		plugin->sendLeaveHost( groupieId );
	}

	m_Engine.getConnectIdListMgr().disconnectIfIsOnlyUser( groupieId );
}