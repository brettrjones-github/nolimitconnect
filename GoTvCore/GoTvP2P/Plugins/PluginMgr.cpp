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
// brett.jones@engineer.com
// http://www.gotvptop.net
//============================================================================

#include "PluginMgr.h"
#include <GoTvInterface/IToGui.h>

#include <GoTvCore/GoTvP2P/MediaProcessor/MediaProcessor.h>

#include <GoTvCore/GoTvP2P/Plugins/PluginVoicePhone.h>
#include <GoTvCore/GoTvP2P/Plugins/PluginVideoPhone.h>
#include <GoTvCore/GoTvP2P/Plugins/PluginTruthOrDare.h>
#include <GoTvCore/GoTvP2P/Plugins/PluginCamServer.h>
#include <GoTvCore/GoTvP2P/Plugins/PluginRelay.h>
#include <GoTvCore/GoTvP2P/Plugins/PluginFileShare.h>
#include <GoTvCore/GoTvP2P/Plugins/PluginFileOffer.h>
#include <GoTvCore/GoTvP2P/Plugins/PluginWebServer.h>
#include <GoTvCore/GoTvP2P/Plugins/PluginStoryBoard.h>
#include <GoTvCore/GoTvP2P/Plugins/PluginNetServices.h>
#include <GoTvCore/GoTvP2P/Plugins/PluginMultiSession.h>

#include <GoTvCore/GoTvP2P/Plugins/PluginInvalid.h>

#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>

#include <GoTvCore/GoTvP2P/BigListLib/BigListInfo.h>
#include <GoTvCore/GoTvP2P/NetServices/NetServiceHdr.h>
#include <NetLib/VxPeerMgr.h>

#include <string.h>
#include <stdarg.h>
#include <stdio.h>

//============================================================================
PluginMgr::PluginMgr( P2PEngine& engine, IToGui& toGui )
: m_Engine( engine )
, m_BigListMgr( engine.getBigListMgr() )
, m_PktAnn( engine.getMyPktAnnounce() )
, m_ToGui( toGui )
, m_PluginMgrInitialized( false )
, m_NetServiceUtils( engine )
{
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
	LogMsg( LOG_INFO, "pluginMgrStartup start\n" );

	PluginBase * poPlugin;
	// invalid
	poPlugin = new PluginInvalid( m_Engine, *this, m_ToGui, &this->m_PktAnn );
	m_aoPlugins.push_back( poPlugin );

	//LogMsg( LOG_INFO, "pluginMgrStartup create admin plugin\n" );
	poPlugin = new PluginInvalid( m_Engine, *this, m_ToGui, &this->m_PktAnn );
	poPlugin->setPluginType( ePluginTypeAdmin );
	m_aoPlugins.push_back( poPlugin );

	//LogMsg( LOG_INFO, "pluginMgrStartup create web server plugin\n" );
	poPlugin = new PluginWebServer( m_Engine, *this, m_ToGui, &this->m_PktAnn );
	m_aoPlugins.push_back( poPlugin );

	// relay pre created by engine
	m_aoPlugins.push_back( &m_Engine.getPluginRelay() );

	//LogMsg( LOG_INFO, "pluginMgrStartup create file share plugin\n" );
	m_aoPlugins.push_back( &m_Engine.getPluginFileShare() );

	//LogMsg( LOG_INFO, "pluginMgrStartup create file offer plugin\n" );
	poPlugin = new PluginFileOffer( m_Engine, *this, m_ToGui, &this->m_PktAnn );
	m_aoPlugins.push_back( poPlugin );

	//LogMsg( LOG_INFO, "pluginMgrStartup create cam server plugin\n" );
	poPlugin = new PluginCamServer( m_Engine, *this, m_ToGui, &this->m_PktAnn);
	m_aoPlugins.push_back( poPlugin );

	//LogMsg( LOG_INFO, "pluginMgrStartup create multisession plugin\n" );
	poPlugin = new PluginMultiSession( m_Engine, *this, m_ToGui, &this->m_PktAnn );
	m_aoPlugins.push_back( poPlugin );

	//LogMsg( LOG_INFO, "pluginMgrStartup create voice phone plugin\n" );
	poPlugin = new PluginVoicePhone( m_Engine, *this, m_ToGui, &this->m_PktAnn );
	m_aoPlugins.push_back( poPlugin );

	//LogMsg( LOG_INFO, "pluginMgrStartup create video phone plugin\n" );
	poPlugin = new PluginVideoPhone( m_Engine, *this, m_ToGui, &this->m_PktAnn );
	m_aoPlugins.push_back( poPlugin );

	//LogMsg( LOG_INFO, "pluginMgrStartup create truth or dare plugin\n" );
	poPlugin = new PluginTruthOrDare( m_Engine, *this, m_ToGui, &this->m_PktAnn );
	m_aoPlugins.push_back( poPlugin );

	//LogMsg( LOG_INFO, "pluginMgrStartup create storyboard plugin\n" );
	poPlugin = new PluginStoryBoard( m_Engine, *this, m_ToGui, &this->m_PktAnn );
	m_aoPlugins.push_back( poPlugin );

	//LogMsg( LOG_INFO, "pluginMgrStartup adding net services\n" );
	// net services pre created by engine
	m_aoPlugins.push_back( &m_Engine.getPluginNetServices() );
	m_PluginMgrInitialized = true;

	//LogMsg( LOG_INFO, "pluginMgrStartup done\n" );
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
PluginBase * PluginMgr::getPlugin( EPluginType ePluginType )
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

	LogMsg( LOG_ERROR, "PluginMgr::getPlugin pluin type %d out of range\n", ePluginType );
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
	sktBase->setIsPluginSpecificSkt( true );
	sktBase->setPluginSpecificNum( (uint8_t)ePluginTypeWebServer );
	PluginBase * poPlugin = getPlugin( ePluginTypeWebServer );
	if( poPlugin )
	{
		sktBase->sktBufAmountRead( 0 );		
		if( 0 == poPlugin->handleHttpConnection( sktBase, netIdent ) )
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
		LogMsg( LOG_ERROR, "handleFirstNetServiceConnection: not valid\n" );
		VxReportHack( sktBase, "handleFirstNetServiceConnection: invalid url\n" );
		sktBase->closeSkt( 636 );
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

	if( ePluginTypeInvalid != pluginType )
	{
		sktBase->setIsPluginSpecificSkt( true );
		sktBase->setPluginSpecificNum( (uint8_t)pluginType );
		
		VxNetIdent * netIdent = NULL;
		if( netServiceHdr.m_OnlineId == m_Engine.getMyPktAnnounce().getMyOnlineId() )
		{
			netIdent = &m_Engine.getMyPktAnnounce();
			LogMsg( LOG_INFO, "PluginMgr::handleFirstNetServiceConnection: parseOnlineId was myself\n" );
		}
		else
		{
			netIdent = m_Engine.getBigListMgr().findBigListInfo( netServiceHdr.m_OnlineId );
		}
	
		netServiceHdr.m_Ident = netIdent;
		PluginBase * poPlugin = getPlugin( pluginType );
		if( poPlugin )
		{
			RCODE rc = 0;
			if( ePluginTypeNetServices == poPlugin->getPluginType() )
			{
				rc = poPlugin->handleHttpConnection( sktBase, netServiceHdr );
			}
			else
			{
				rc = poPlugin->handleHttpConnection( sktBase, netServiceHdr.m_Ident );
			}
			if( 0 == rc )
			{
				// socket was handled
				httpConnectionWasHandled = true;
			}
		}
		else
		{
			LogMsg( LOG_INFO, "PluginMgr::handleFirstNetServiceConnection; unknown plugin type\n" );
			m_Engine.hackerOffense( NULL, 1, sktBase->getRemoteIpBinary(), "Hacker http attack (unknown plugin)from ip %s\n", sktBase->getRemoteIp() );
			sktBase->closeSkt( 657 );
		}
	}

	if( false == httpConnectionWasHandled )
	{
		m_Engine.hackerOffense( NULL, 1, sktBase->getRemoteIpBinary(), "Hacker http attack from ip %s\n", sktBase->getRemoteIp() );
		sktBase->closeSkt( 657 );
	}

}

//============================================================================
void PluginMgr::handlePluginSpecificSkt( VxSktBase * sktBase )
{
	uint8_t u8PluginNum = sktBase->getPluginSpecificNum();
	if( isValidPluginNum( u8PluginNum ) )
	{
		getPlugin( (EPluginType)u8PluginNum )->handlePluginSpecificSkt( sktBase );
	}
	else
	{
		LogMsg( LOG_ERROR, "PluginMgr::handlePluginSpecificSkt invalid plugin num %d\n", u8PluginNum );
	}
}

//============================================================================
//! this is called for all valid packets that are not sys packets
void PluginMgr::handleNonSystemPackets( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	//LogMsg( LOG_INFO, "PluginMgr::handleNonSystemPackets\n" );
	uint8_t u8PluginNum = pktHdr->getPluginNum();
	if( isValidPluginNum( u8PluginNum ) )
	{
		PluginBase * plugin = getPlugin( (EPluginType)u8PluginNum );
		VxNetIdent * netIdent = m_BigListMgr.findBigListInfo( pktHdr->getSrcOnlineId() );
		if( netIdent && plugin )
		{
			plugin->handlePkt( sktBase, pktHdr, netIdent );
		}
		else
		{
			LogMsg( LOG_ERROR, "PluginMgr::handleNonSystemPackets unknown ident 0x%llX 0x%llX\n", 
				pktHdr->getSrcOnlineId().getVxGUIDHiPart(),
				pktHdr->getSrcOnlineId().getVxGUIDLoPart() );
		}
	}
	else
	{
		LogMsg( LOG_ERROR, "PluginMgr::handleNonSystemPackets invalid plugin num %d\n", u8PluginNum );
	}
}

//============================================================================
bool PluginMgr::isValidPluginNum( uint8_t u8PluginNum )
{
	return ((ePluginTypeInvalid < u8PluginNum ) && (eMaxPluginType > u8PluginNum )) ? true : false;
}

//============================================================================
//! get permission/access state for remote user
EPluginAccessState PluginMgr::pluginApiGetPluginAccessState( EPluginType ePluginType, VxNetIdent * netIdent )
{
	PluginBase * plugin = getPlugin( ePluginType );
	if( plugin )
	{
		EPluginAccessState eAccess = m_PktAnn.getHisAccessPermissionFromMe( ePluginType );
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
	// set all plugin permissions
	std::vector<PluginBase * >::iterator iter;
	for( iter = m_aoPlugins.begin(); iter != m_aoPlugins.end(); ++iter )
	{
		(*iter)->setPluginPermission( m_PktAnn.getPluginPermission( (*iter)->getPluginType() ) );
	}

	// now tell plugins we are logged on
	for( iter = m_aoPlugins.begin(); iter != m_aoPlugins.end(); ++iter )
	{
		PluginBase * pluginBase = ( *iter );
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
void PluginMgr::fromGuiSendAsset( AssetInfo& assetInfo )
{
	std::vector<PluginBase * >::iterator iter;
	for( iter = m_aoPlugins.begin(); iter != m_aoPlugins.end(); ++iter )
	{
		(*iter)->fromGuiSendAsset( assetInfo );
	}
}

//============================================================================
PluginBase * PluginMgr::findPlugin( EPluginType ePluginType )
{
	std::vector<PluginBase * >::iterator iter;
	for( iter = m_aoPlugins.begin(); iter != m_aoPlugins.end(); ++iter )
	{
		if( ePluginType == (*iter)->getPluginType() )
		{
			return (*iter);
		}
	}

	return 0;
}

//============================================================================
bool PluginMgr::fromGuiMultiSessionAction( EMSessionAction mSessionAction, VxGUID& onlineId, int pos0to100000, VxGUID lclSessionId )
{
	bool result = false;
	PluginBase * plugin = findPlugin( ePluginTypeMultiSession );
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
bool PluginMgr::pluginApiTxPacket(	EPluginType			ePluginType, 
									VxNetIdentBase *	netIdent, 
									VxSktBase *			sktBase, 
									VxPktHdr *			poPkt, 
									bool				bDisconnectAfterSend )

{
	poPkt->setPluginNum( (uint8_t)ePluginType );
	poPkt->setSrcOnlineId( m_Engine.getMyPktAnnounce().getMyOnlineId() );

	return m_Engine.getPeerMgr().txPacket( sktBase, netIdent->getMyOnlineId(), poPkt, bDisconnectAfterSend );
}

//============================================================================
void PluginMgr::pluginApiPlayVideoFrame( EPluginType ePluginType, uint8_t * pu8VidData, uint32_t u32VidDataLen, VxNetIdent * netIdent, int motion0to100000 )
{
	//LogMsg( LOG_INFO, "PluginMgr::pluginApiPlayVideoFrame\n" );
	m_ToGui.toGuiPlayVideoFrame( netIdent->getMyOnlineId(), pu8VidData, u32VidDataLen, motion0to100000 );
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
void PluginMgr::fromGuiStopPluginSession( EPluginType ePluginType, VxGUID& oOnlineId, int pvUserData, VxGUID lclSessionId )
{
	PluginBase * plugin = getPlugin( ePluginType );
	if( plugin )
	{
		BigListInfo * poInfo = m_BigListMgr.findBigListInfo( oOnlineId );
		if( poInfo )
		{
			plugin->fromGuiStopPluginSession( poInfo, pvUserData, lclSessionId );	
		}
		else if( oOnlineId == m_PktAnn.getMyOnlineId() )
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
bool PluginMgr::fromGuiSetGameValueVar(	EPluginType	ePluginType, 
											VxGUID&	oOnlineId, 
											int32_t			s32VarId, 
											int32_t			s32VarValue )
{
	bool bResult = false;
	PluginBase * plugin = getPlugin( ePluginType );
	if( plugin )
	{
		BigListInfo * poInfo = m_BigListMgr.findBigListInfo( oOnlineId );
		if( poInfo )
		{
			bResult = plugin->fromGuiSetGameValueVar( poInfo, s32VarId, s32VarValue );	
		}
		else if( oOnlineId == m_PktAnn.getMyOnlineId() )
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
bool PluginMgr::fromGuiSetGameActionVar(	EPluginType	ePluginType, 
											VxGUID&		oOnlineId,
											int32_t			s32VarId, 
											int32_t			s32VarValue )
{
	bool bResult = false;
	PluginBase * plugin = getPlugin( ePluginType );
	if( plugin )
	{
		BigListInfo * poInfo = m_BigListMgr.findBigListInfo( oOnlineId );
		if( poInfo )
		{
			bResult = plugin->fromGuiSetGameActionVar( poInfo, s32VarId, s32VarValue );	
		}
		else if( oOnlineId == m_PktAnn.getMyOnlineId() )
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
EPluginAccessState PluginMgr::canAcceptNewSession( EPluginType ePluginType, VxNetIdent * netIdent )
{
	EPluginAccessState canAcceptSession = ePluginAccessDisabled;
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
