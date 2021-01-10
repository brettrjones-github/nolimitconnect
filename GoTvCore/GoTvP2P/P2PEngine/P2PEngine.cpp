//============================================================================
// Copyright (C) 2009 Brett R. Jones 
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

#include "P2PEngine.h"
#include "P2PConnectList.h"
#include <GoTvInterface/IToGui.h>
#include "Application.h"
#include <GoTvCore/GoTvP2P/PluginSettings/PluginSettingMgr.h>

#include <GoTvCore/GoTvP2P/Network/NetworkMgr.h>
#include <GoTvCore/GoTvP2P/Network/NetworkStateMachine.h>
#include <GoTvCore/GoTvP2P/Network/NetConnector.h>
#include <GoTvCore/GoTvP2P/NetworkMonitor/NetworkMonitor.h>
#include <GoTvCore/GoTvP2P/NetServices/NetServicesMgr.h>

#include <GoTvCore/GoTvP2P/Plugins/PluginServiceRelay.h>
#include <GoTvCore/GoTvP2P/Plugins/PluginServiceFileShare.h>
#include <GoTvCore/GoTvP2P/Plugins/PluginNetServices.h>

#include <GoTvCore/GoTvP2P/Search/RcScan.h>
//#include <GoTvCore/GoTvP2P/WebRelay/RcWebRelaysMgr.h>
#include <GoTvCore/GoTvP2P/MediaProcessor/MediaProcessor.h>

#include <GoTvCore/GoTvP2P/NetworkTest/IsPortOpenTest.h>
#include <GoTvCore/GoTvP2P/NetworkTest/RunUrlAction.h>

#include <GoTvCore/GoTvP2P/AssetMgr/AssetMgr.h>
#include <GoTvCore/GoTvP2P/HostListMgr/HostListMgr.h>
#include <GoTvCore/GoTvP2P/Plugins/PluginMgr.h>

#include <GoTvCore/GoTvP2P/BigListLib/BigListInfo.h>

#include <NetLib/VxSktUtil.h>
#include <NetLib/VxPeerMgr.h>

#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxGlobals.h>
#include <CoreLib/AppErr.h>

#ifdef _MSC_VER
# pragma warning(disable: 4355) //'this' : used in base member initializer list
#endif

namespace
{
	void LogHandler( void * userData, uint32_t u32LogFlags, char * logMsg )
	{
		if( userData )
		{
			((P2PEngine *)userData)->getToGui().toGuiLog( u32LogFlags, logMsg );
		}
	}

	void AppErrHandler( void * userData, EAppErr eAppErr, char * errMsg )
	{
		if( userData )
		{
			((P2PEngine *)userData)->getToGui().toGuiAppErr( eAppErr, errMsg );
		}
	}
}

//============================================================================
P2PEngine::P2PEngine( VxPeerMgr& peerMgr, BigListMgr& bigListMgr )
    : m_PeerMgr( peerMgr )
    , m_BigListMgr( bigListMgr )
    , m_EngineSettings()
    , m_EngineParams()
    , m_NetStatusAccum( *this )
    , m_AssetMgr( *new AssetMgr( *this ) )
    , m_HostListMgr( *new HostListMgr( *this ) )
    , m_ConnectionMgr( *new ConnectionMgr( *this ) )
    , m_ConnectionList( *this )
    , m_MediaProcessor( *( new MediaProcessor( *this ) ) )
    , m_NetworkMgr( *new NetworkMgr( *this, peerMgr, m_BigListMgr, m_ConnectionList ) )
    , m_NetworkMonitor( *new NetworkMonitor( *this ) )
    , m_NetServicesMgr( *new NetServicesMgr( *this ) )
    , m_NetConnector( *new NetConnector( *this ) )
    , m_NetworkStateMachine( *new NetworkStateMachine( *this, m_NetworkMgr ) )
    , m_PluginMgr( *new PluginMgr( *this ) )
    , m_PluginSettingMgr( *this )
    , m_PluginServiceRelay( new PluginServiceRelay( *this, m_PluginMgr, &m_PktAnn ) )
    , m_PluginServiceFileShare( new PluginServiceFileShare( *this, m_PluginMgr, &m_PktAnn ) )
    , m_PluginNetServices( new PluginNetServices( *this, m_PluginMgr, &m_PktAnn ) )
    , m_IsPortOpenTest( *new IsPortOpenTest( *this, m_EngineSettings, m_NetServicesMgr, m_NetServicesMgr.getNetUtils() ) )
    , m_RunUrlAction( *new RunUrlAction( *this, m_EngineSettings, m_NetServicesMgr, m_NetServicesMgr.getNetUtils() ) )
    , m_RcScan( *this, m_ConnectionList )
{
    m_NetStatusAccum.addNetStatusCallback( &m_ConnectionMgr );
}

//============================================================================
P2PEngine::~P2PEngine()
{
	m_PeerMgr.stopListening();
	m_PluginMgr.pluginMgrShutdown();
}

//============================================================================
/// if skt exists in connection list then lock access to connection list
bool P2PEngine::lockSkt( VxSktBase* sktBase )
{
    return m_PeerMgr.lockSkt( sktBase );
}

//============================================================================
void P2PEngine::unlockSkt( VxSktBase* sktBase )
{
    m_PeerMgr.unlockSkt( sktBase );
}

//============================================================================
IToGui& P2PEngine::getToGui()
{
    return IToGui::getToGui();
}

//============================================================================
IAudioRequests& P2PEngine::getAudioRequest()
{
    return IToGui::getAudioRequests();
}

//============================================================================
void P2PEngine::startupEngine()
{
    iniitializePtoPEngine();

	m_NetworkStateMachine.stateMachineStartup();
	m_PluginMgr.onAppStartup();
}

//============================================================================
void P2PEngine::iniitializePtoPEngine( void )
{
    if( !m_EngineInitialized )
    {
        m_EngineInitialized = true;
        VxSetNetworkLoopbackAllowed( false );
        // not needed if do not need to send log to gui VxSetLogHandler( LogHandler, this );
        VxSetAppErrHandler( AppErrHandler, this );
        VxSocketsStartup();
        m_PluginMgr.pluginMgrStartup();
        m_AssetMgr.addAssetMgrClient( this, true );
   }
}

//============================================================================
void P2PEngine::shutdownEngine( void )
{
	VxSetAppIsShuttingDown( true );
	LogMsg( LOG_INFO, "P2PEngine::shutdownEngine: stop listening\n" );
	m_PeerMgr.stopListening();
	LogMsg( LOG_INFO, "P2PEngine::shutdownEngine: remove asset client\n" );
	m_AssetMgr.addAssetMgrClient( this, false );
	LogMsg( LOG_INFO, "P2PEngine::shutdownEngine: shutdown media processor\n" );
	m_MediaProcessor.shutdownMediaProcessor();
	LogMsg( LOG_INFO, "P2PEngine::shutdownEngine: shutdown IsPortOpen\n" );
	m_IsPortOpenTest.isPortOpenShutdown();
	//VxUpdateSystemTime();
	//m_NetworkMgr.networkMgrShutdown();
	LogMsg( LOG_INFO, "P2PEngine::shutdownEngine: m_PeerMgr.sktMgrShutdown\n" );
	m_PeerMgr.sktMgrShutdown();

	LogMsg( LOG_INFO, "P2PEngine::shutdownEngine: m_PluginMgr.onAppShutdown\n" );
	m_PluginMgr.onAppShutdown();
	LogMsg( LOG_INFO, "P2PEngine::shutdownEngine: m_NetworkStateMachine.stateMachineShutdown\n" );
	m_NetworkStateMachine.stateMachineShutdown();
	LogMsg( LOG_INFO, "P2PEngine::shutdownEngine: m_PluginMgr.pluginMgrShutdown\n" );
	m_PluginMgr.pluginMgrShutdown();
	//m_RcScan.scanShutdown();
	LogMsg( LOG_INFO, "P2PEngine::shutdownEngine: databases shutdown\n" );
	getEngineSettings().engineSettingsShutdown();
	getEngineParams().engineParamsShutdown();
	m_BigListMgr.bigListMgrShutdown();
	m_AssetMgr.assetInfoMgrShutdown();

	//g_oHttpConnection.httpConnectionShutdown();
	LogMsg( LOG_INFO, "P2PEngine::shutdownEngine: waiting threads exit\n" );
	//delete (IsPortOpenTest *)&m_IsPortOpenTest;
	//delete (PluginNetServices *)&m_PluginNetServices;
	//delete (PluginServiceFileShare *)&m_PluginServiceFileShare;
	//delete &m_PluginServiceRelay;
	//delete &m_MediaProcessor;
	//delete &m_PluginMgr;
	//delete &m_NetConnector;
	//delete &m_HostTest;
	//delete &m_NetworkStateMachine;
	//delete &m_NetServicesMgr;
	//delete &m_NetworkMonitor;
	//delete &m_NetworkMgr;
	//delete &m_AssetMgr;
	
	VxSleep( 1000 );
	for ( int i = 0; i < 8; i++ )
	{
		if ( 0 == VxThread::getThreadsRunningCount() )
		{
			break;
		}
		
		VxThread::dumpRunningThreads();
		VxSleep( 1000 );
	}
	
	m_PluginServiceRelay			= 0;
	m_PluginServiceFileShare		= 0;
	m_PluginNetServices		= 0;
	LogMsg( LOG_INFO, "P2PEngine::shutdownEngine: done\n" );
}

//============================================================================
void P2PEngine::fromGuiKickWatchdog( void )
{
	m_PeerMgr.fromGuiKickWatchdog();
}

//============================================================================
void P2PEngine::onSessionStart( EPluginType ePluginType, VxNetIdent * netIdent )
{
	bool shouldUpdateLastSessionTime = false;
	switch( ePluginType )
	{
	case ePluginTypeMessenger:
	case ePluginTypeVoicePhone:
	case ePluginTypeVideoPhone:
	case ePluginTypeTruthOrDare:
		shouldUpdateLastSessionTime = true;
	default:
		break;
	}

	if( shouldUpdateLastSessionTime )
	{
		int64_t sysTimeMs = GetGmtTimeMs();
		
		netIdent->setLastSessionTimeMs( sysTimeMs );
		m_BigListMgr.dbUpdateSessionTime( netIdent->getMyOnlineId(), sysTimeMs, getNetworkMgr().getNetworkKey() );
		IToGui::getToGui().toGuiContactLastSessionTimeChange( netIdent );
	}
}

//============================================================================
bool P2PEngine::shouldInfoBeInDatabase( BigListInfo * poInfo )
{
	if( poInfo->getMyOnlineId() == m_PktAnn.getMyOnlineId() )
	{
		return false;
	}

	EFriendState friendState =	poInfo->getMyFriendshipToHim();
	if( poInfo->isMyRelay() 
		|| poInfo->isMyPreferedRelay()
		|| ( eFriendStateAnonymous != friendState ) )
	{
		LogMsg( LOG_INFO, "%s belongs in database. is my proxy %d preferred proxy %d\n", 
						poInfo->getOnlineName(), 
						poInfo->isMyRelay(), 
						poInfo->isMyPreferedRelay() );
		return true;
	}

	//LogMsg( LOG_INFO, "%s does not belong in database\n", poInfo->getOnlineName() );
	return false;
}

//============================================================================
void P2PEngine::onBigListInfoRestored( BigListInfo * poInfo )
{
	//LogMsg( LOG_INFO, "onBigListInfoRestored\n");
	//poInfo->debugDumpIdent();
	if( poInfo->isMyRelay() )
	{
		m_ConnectionList.getPreferredRelayList().addContactInfo( poInfo->getConnectInfo() );
	}
	else if( false == poInfo->requiresRelay() )
	{
		m_ConnectionList.getPossibleRelayList().addContactInfo( poInfo->getConnectInfo() );
	}

	if( isP2POnline() )
	{
		m_NetConnector.addConnectRequestToQue( poInfo->getConnectInfo() );
	}
}

//============================================================================
void P2PEngine::onBigListInfoDelete( BigListInfo * poInfo )
{
	LogMsg( LOG_INFO, "onBigListInfoDelete\n");
	poInfo->debugDumpIdent();
	if( poInfo->isMyRelay() )
	{
		m_ConnectionList.removeContactInfo( poInfo->getConnectInfo() );
	}

	//m_RcScan.onIdentDelete( poInfo );
	//NOTE: TODO.. there are many more places we should check for references to BigListInfo  or VxNetIdent
}

//============================================================================
//! called by big list when all friends are loaded
void P2PEngine::onBigListLoadComplete( RCODE rc )
{
	LogMsg( LOG_INFO, "onBigListLoadComplete\n");
}

//============================================================================
//! handle app state change
void P2PEngine::doAppStateChange( EAppState eAppState )
{
	m_PluginMgr.onAppStateChange( eAppState );
}

//============================================================================
bool P2PEngine::setPluginSetting( PluginSetting& pluginSetting )
{
    if( ( ePluginTypeInvalid < pluginSetting.getPluginType() ) && ( eMaxPluginType > pluginSetting.getPluginType() ) )
    {
        return getPluginSettingMgr().setPluginSetting( pluginSetting );
    }

    return false;
}

//============================================================================
bool P2PEngine::getPluginSetting( EPluginType pluginType, PluginSetting& pluginSetting )
{
    if( ( ePluginTypeInvalid < pluginType ) && ( eMaxPluginType > pluginType ) )
    {
        return getPluginSettingMgr().getPluginSetting( pluginType, pluginSetting );
    }

    return false;
}

//============================================================================
EFriendState P2PEngine::getPluginPermission( int iPluginType )
{
    EFriendState iPermission = eFriendStateIgnore;
	EPluginType ePluginType = (EPluginType)iPluginType;
	if( ( ePluginTypeInvalid < ePluginType ) && 
		( eMaxPluginType > ePluginType ) )
	{
		iPermission = m_PluginMgr.getPluginPermission(ePluginType);
	}

	return iPermission;
}

//============================================================================
void P2PEngine::setPluginPermission( EPluginType ePluginType, int iPluginPermission )
{
	if( ( ePluginTypeInvalid < ePluginType ) && 
		( eMaxPluginType > ePluginType ) )
	{
		if( ( iPluginPermission != m_PluginMgr.getPluginPermission( ePluginType ) )
			|| ( iPluginPermission != m_PktAnn.getPluginPermission( ePluginType ) ) )
		{
			m_PluginMgr.setPluginPermission( ePluginType, (EFriendState)iPluginPermission );
			m_PktAnn.setPluginPermission( ePluginType, (EFriendState)iPluginPermission );
			doPktAnnHasChanged( false );
		}
	}
}

//============================================================================
//! if bHasPicture then user has updated profile picture		
void P2PEngine::setHasPicture( int bHasPicture )
{
	m_PktAnn.setHasProfilePicture( bHasPicture ? true : false );
	// let others handle the changed announcement packet
	doPktAnnHasChanged( false );
}

//============================================================================
//! if bHasShaeredWebCam then user has has started web cam server		
void P2PEngine::setHasSharedWebCam( int hasSharedWebCam )
{
	bool sharedCam = hasSharedWebCam ? true : false;
	if( sharedCam != m_PktAnn.hasSharedWebCam() )
	{
		m_PktAnn.setHasSharedWebCam( sharedCam );
		// let others handle the changed announcement packet
		doPktAnnHasChanged( false );
	}
}

