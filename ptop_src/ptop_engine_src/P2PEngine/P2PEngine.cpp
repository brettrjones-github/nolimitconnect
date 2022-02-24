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
#include <GuiInterface/IToGui.h>
//#include "Application.h"
#include <ptop_src/ptop_engine_src/PluginSettings/PluginSettingMgr.h>

#include <ptop_src/ptop_engine_src/Network/NetworkMgr.h>
#include <ptop_src/ptop_engine_src/Network/NetworkStateMachine.h>
#include <ptop_src/ptop_engine_src/Network/NetConnector.h>
#include <ptop_src/ptop_engine_src/NetworkMonitor/NetworkMonitor.h>
#include <ptop_src/ptop_engine_src/NetServices/NetServicesMgr.h>

#include <ptop_src/ptop_engine_src/Plugins/PluginServiceRelay.h>
#include <ptop_src/ptop_engine_src/Plugins/PluginServiceFileShare.h>
#include <ptop_src/ptop_engine_src/Plugins/PluginNetServices.h>

#include <ptop_src/ptop_engine_src/Search/RcScan.h>
//#include <ptop_src/ptop_engine_src/WebRelay/RcWebRelaysMgr.h>
#include <ptop_src/ptop_engine_src/MediaProcessor/MediaProcessor.h>

#include <ptop_src/ptop_engine_src/NetworkTest/IsPortOpenTest.h>
#include <ptop_src/ptop_engine_src/NetworkTest/RunUrlAction.h>

#include <ptop_src/ptop_engine_src/AssetMgr/AssetMgr.h>
#include <ptop_src/ptop_engine_src/BlobXferMgr/BlobMgr.h>
#include <ptop_src/ptop_engine_src/Plugins/PluginMgr.h>

#include <ptop_src/ptop_engine_src/BigListLib/BigListInfo.h>

#include <ptop_src/ptop_engine_src/HostJoinMgr/HostJoinMgr.h>
#include <ptop_src/ptop_engine_src/UserJoinMgr/UserJoinMgr.h>
#include <ptop_src/ptop_engine_src/UserOnlineMgr/UserOnlineMgr.h>

#include <ptop_src/ptop_engine_src/UrlMgr/UrlMgr.h>

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
	//void LogHandler( void * userData, uint32_t u32LogFlags, char * logMsg )
	//{
	//	if( userData )
	//	{
	//		((P2PEngine *)userData)->getToGui().toGuiLog( u32LogFlags, logMsg );
	//	}
	//}

	void AppErrHandler( void * userData, EAppErr eAppErr, char * errMsg )
	{
		if( userData )
		{
			((P2PEngine *)userData)->getToGui().toGuiAppErr( eAppErr, errMsg );
		}
	}
}

//============================================================================
P2PEngine::P2PEngine( VxPeerMgr& peerMgr )
	: m_PeerMgr( peerMgr )
	, m_ConnectIdListMgr( *this )
	, m_IgnoreListMgr( *this )
	, m_FriendListMgr( *this )
	, m_GroupieListMgr( *this )
	, m_HostUrlListMgr( *this )
	, m_HostedListMgr( *this )
	, m_NearbyListMgr( *this )
	, m_BigListMgr( *this )
	, m_EngineSettings()
	, m_EngineParams()
	, m_NetStatusAccum( *this )
	, m_AssetMgr( *new AssetMgr( *this, "AssetMgrDb.db3", "AssetStateDb.db3" ) )
	, m_BlobMgr( *new BlobMgr( *this, "BlobAssetDb.db3", "BlobStateDb.db3" ) )
	, m_OfferClientMgr( *new OfferClientMgr( *this, "OfferClientDb.db3", "OfferClientStateDb.db3" ) )
	, m_OfferHostMgr( *new OfferHostMgr( *this, "OfferHostDb.db3", "OfferHostStateDb.db3" ) )
	, m_ThumbMgr( *new ThumbMgr( *this, "ThumbAssetDb.db3", "ThumbStateDb.db3" ) )
	, m_ConnectionMgr( *new ConnectionMgr( *this ) )
	, m_ConnectMgr( *new ConnectMgr( *this, "ConnectMgrDb.db3", "ConnectStateDb.db3" ) )
	, m_ConnectionList( *this )
	, m_MediaProcessor( *( new MediaProcessor( *this ) ) )
	, m_MembershipAvailableMgr( *this )
    , m_MembershipHostedMgr( *this )
    , m_NetworkMgr( *new NetworkMgr( *this, peerMgr, m_BigListMgr, m_ConnectionList ) )
    , m_NetworkMonitor( *new NetworkMonitor( *this ) )
    , m_NetServicesMgr( *new NetServicesMgr( *this ) )
    , m_NetConnector( *new NetConnector( *this ) )
    , m_NetworkStateMachine( *new NetworkStateMachine( *this, m_NetworkMgr ) )
    , m_PluginMgr( *new PluginMgr( *this ) )
    , m_PluginSettingMgr( *this )
    , m_PluginServiceRelay( new PluginServiceRelay( *this, m_PluginMgr, &m_PktAnn, ePluginTypeRelay ) )
    , m_PluginServiceFileShare( new PluginServiceFileShare( *this, m_PluginMgr, &m_PktAnn, ePluginTypeFileServer ) )
    , m_PluginNetServices( new PluginNetServices( *this, m_PluginMgr, &m_PktAnn, ePluginTypeNetServices ) )
    , m_IsPortOpenTest( *new IsPortOpenTest( *this, m_EngineSettings, m_NetServicesMgr, m_NetServicesMgr.getNetUtils() ) )
    , m_RunUrlAction( *new RunUrlAction( *this, m_EngineSettings, m_NetServicesMgr, m_NetServicesMgr.getNetUtils() ) )
    , m_HostJoinMgr( *new HostJoinMgr( *this, "HostJoinMgrDb.db3", "HostJoinedLastDb.db3" ) )
    , m_UserJoinMgr( *new UserJoinMgr( *this, "UserJoinMgrDb.db3", "UserJoinedLastDb.db3" ) )
    , m_UserOnlineMgr( *new UserOnlineMgr( *this, "UserOnlineMgrDb.db3", "UserOnlineStateDb.db3" ) )
    , m_SktLoopback( *this )
    , m_RcScan( *this, m_ConnectionList )
{
    m_PeerMgr.setSktLoopback( &m_SktLoopback );
    m_NetStatusAccum.addNetStatusCallback( &m_ConnectionMgr );
	m_IsEngineCreated = true;
}

//============================================================================
P2PEngine::~P2PEngine()
{
	m_PeerMgr.stopListening();
	m_PluginMgr.pluginMgrShutdown();
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
OfferBaseMgr& P2PEngine::getOfferMgr( EOfferMgrType mgrType )
{
    switch( mgrType )
    {
    case eOfferMgrTypeOfferClient:
        return getOfferClientMgr();
    case eOfferMgrTypeOfferHost:
        return getOfferHostMgr();
    default:
        vx_assert( false );
        return getOfferClientMgr();
    }
}

//============================================================================
UrlMgr& P2PEngine::getUrlMgr()
{
	return GetUrlMgrInstance();
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
        //m_AssetMgr.addAssetMgrClient( this, true );
        //m_BlobMgr.addBlobMgrClient( this, true );
        //m_ThumbMgr.addThumbMgrClient( this, true );
   }
}

//============================================================================
void P2PEngine::shutdownEngine( void )
{
	VxSetAppIsShuttingDown( true );
	LogMsg( LOG_VERBOSE, "P2PEngine::shutdownEngine: stop listening" );
	m_PeerMgr.stopListening();
	LogMsg( LOG_VERBOSE, "P2PEngine::shutdownEngine: remove asset client" );
	//m_AssetMgr.addAssetMgrClient( this, false );
	LogMsg( LOG_VERBOSE, "P2PEngine::shutdownEngine: shutdown media processor" );
	m_MediaProcessor.shutdownMediaProcessor();
	LogMsg( LOG_VERBOSE, "P2PEngine::shutdownEngine: shutdown IsPortOpen" );
	m_IsPortOpenTest.isPortOpenShutdown();
	//VxUpdateSystemTime();
	//m_NetworkMgr.networkMgrShutdown();
	LogMsg( LOG_VERBOSE, "P2PEngine::shutdownEngine: m_PeerMgr.sktMgrShutdown" );
	m_PeerMgr.sktMgrShutdown();

	LogMsg( LOG_VERBOSE, "P2PEngine::shutdownEngine: m_PluginMgr.onAppShutdown" );
	m_PluginMgr.onAppShutdown();
	LogMsg( LOG_VERBOSE, "P2PEngine::shutdownEngine: m_NetworkStateMachine.stateMachineShutdown" );
	m_NetworkStateMachine.stateMachineShutdown();
	LogMsg( LOG_VERBOSE, "P2PEngine::shutdownEngine: m_PluginMgr.pluginMgrShutdown" );
	m_PluginMgr.pluginMgrShutdown();
	//m_RcScan.scanShutdown();
	LogMsg( LOG_VERBOSE, "P2PEngine::shutdownEngine: databases shutdown" );
	getEngineSettings().engineSettingsShutdown();
	getEngineParams().engineParamsShutdown();
	m_BigListMgr.bigListMgrShutdown();
	m_AssetMgr.assetInfoMgrShutdown();

	//g_oHttpConnection.httpConnectionShutdown();
	LogMsg( LOG_VERBOSE, "P2PEngine::shutdownEngine: waiting threads exit" );
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
	LogMsg( LOG_VERBOSE, "P2PEngine::shutdownEngine: done" );
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
	updateIdentLists( poInfo, poInfo->getLastSessionTimeMs() );
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
    getToGui().toGuiContactRemoved( poInfo->getConnectIdent().getMyOnlineId() );
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
	LogMsg( LOG_INFO, "onBigListLoadComplete %d", rc );
    getBigListMgr().bigListLock();
    for( auto iter = getBigListMgr().m_BigList.begin(); iter != getBigListMgr().m_BigList.end(); ++iter )
    {
        BigListInfo * info = iter->second;
        if( info && !info->isAnonymous() )
        {
            getToGui().toGuiContactAdded( info );
        }
        else
        {
            if( info )
            {
                LogMsg( LOG_ERROR, "onBigListLoadComplete anonymouse info %s %s", info->getOnlineName(), info->getMyOnlineIdHexString().c_str() );
            }
            else
            {
                LogMsg( LOG_ERROR, "onBigListLoadComplete null info" );
            }
        }
    }

    getBigListMgr().bigListUnlock();
}

//============================================================================
//! handle app state change
void P2PEngine::doAppStateChange( EAppState eAppState )
{
	m_PluginMgr.onAppStateChange( eAppState );
}

//============================================================================
bool P2PEngine::addMyIdentToBlob( PktBlobEntry& blobEntry )
{ 
    m_AnnouncePktMutex.lock(); 
    bool result = (*((VxNetIdent *)&m_PktAnn)).addToBlob( blobEntry ); 
    m_AnnouncePktMutex.unlock(); 
    return result;
}

//============================================================================
// true if have open port and ready to recieve
bool P2PEngine::isDirectConnectReady( void )
{
    bool directConnectReady = false;
    if( eFirewallTestAssumeNoFirewall == m_EngineSettings.getFirewallTestSetting() )
    {
        directConnectReady = m_NetStatusAccum.isInternetAvailable();
    }
    else
    {
        directConnectReady = m_NetStatusAccum.isInternetAvailable() && m_NetStatusAccum.isDirectConnectTested() && !m_NetStatusAccum.requiresRelay();
    }

    return directConnectReady;
}

//============================================================================
// true if netowrk host plugin is enabled
bool P2PEngine::isNetworkHostEnabled( void )
{
    m_AnnouncePktMutex.lock(); 
    bool netHostEnabled = ( eFriendStateIgnore != m_PktAnn.getPluginPermission( ePluginTypeHostNetwork ) ); 
    m_AnnouncePktMutex.unlock();
    return netHostEnabled;
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

//============================================================================
void P2PEngine::updateIdentLists( BigListInfo* bigListInfo, int64_t timestampMs )
{
	int64_t timestamp;
	if( timestampMs )
	{
		timestamp = timestampMs;
	}
	else
	{
		timestamp = GetTimeStampMs();
	}

	if( bigListInfo->isIgnored() )
	{
		m_IgnoreListMgr.updateIdent( bigListInfo->getMyOnlineId(), timestamp );
		m_FriendListMgr.removeIdent( bigListInfo->getMyOnlineId() );
	}
	else
	{
		m_IgnoreListMgr.removeIdent( bigListInfo->getMyOnlineId() );
		m_FriendListMgr.removeIdent( bigListInfo->getMyOnlineId() );
		if( bigListInfo->isAdministrator() || bigListInfo->isFriend() )
		{
			m_FriendListMgr.updateIdent( bigListInfo->getMyOnlineId(), timestamp );
		}
	}
}

//============================================================================
bool validateArrayString( const char* str, int minStrLen, int maxStrLen )
{
	int strLen = 0;
	bool nullTerminatorFound = false;
	for( int i = 0; i < maxStrLen; ++i )
	{
		if( 0 == str[i] )
		{
			nullTerminatorFound = true;
			break;
		}

		if( isascii( str[i] ) )
		{
			strLen++;
		}
		else
		{
			LogMsg( LOG_ERROR, "validateArrayString invalid char 0x%X", str[i] );
			return false;
		}
	}

	return strLen >= minStrLen && nullTerminatorFound;
}

//============================================================================
bool P2PEngine::validateIdent( VxNetIdent* netIdent )
{
	// validate port is reasonable value
	if( 80 > netIdent->getOnlinePort() )
	{
		return false;
	}

	// validate online name
	if( !validateArrayString( netIdent->getOnlineName(), 4, MAX_ONLINE_NAME_LEN ) )
	{
		LogMsg( LOG_ERROR, "validateIdent invalid name");
		return false;
	}

	if( !validateArrayString( netIdent->getOnlineDescription(), 4, MAX_ONLINE_DESC_LEN ) )
	{
		LogMsg( LOG_ERROR, "validateIdent invalid description" );
		return false;
	}

	return netIdent->getMyOnlinePort() > 0;
}

//============================================================================
void P2PEngine::onFirstPktAnnounce( PktAnnounce* pktAnn, VxSktBase* sktBase, BigListInfo* bigListInfo )
{
	if( pktAnn && pktAnn->getMyOnlineId() != getMyOnlineId() && sktBase && bigListInfo )
	{
		if( getHasFixedIpAddress() )
		{
			std::string externAddr = getEngineSettings().getUserSpecifiedExternIpAddr();
			getNetStatusAccum().setDirectConnectTested( true, false, externAddr );
		}

		getHostUrlListMgr().updateHostUrls( bigListInfo->getVxNetIdent(), sktBase->getLastActiveTimeMs() );
		getHostedListMgr().updateHostedList( bigListInfo->getVxNetIdent(), sktBase );

		getThumbMgr().requestThumbs( sktBase, bigListInfo->getVxNetIdent() );

		getToGui().toGuiContactAdded( bigListInfo->getVxNetIdent() );
	}
}
