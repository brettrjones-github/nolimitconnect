#include "P2PEngine.h"

#include <ptop_src/ptop_engine_src/Network/NetworkStateMachine.h>

#include <ptop_src/ptop_engine_src/BigListLib/BigListInfo.h>
#include <ptop_src/ptop_engine_src/Plugins/PluginNetServices.h>
#include <ptop_src/ptop_engine_src/Plugins/PluginMgr.h>

#include <ptop_src/ptop_engine_src/HostJoinMgr/HostJoinMgr.h>
#include <ptop_src/ptop_engine_src/UserJoinMgr/UserJoinMgr.h>
#include <ptop_src/ptop_engine_src/UserOnlineMgr/UserOnlineMgr.h>

#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxFileShredder.h>
#include <CoreLib/VxParse.h>

namespace
{
    //============================================================================
    void* P2PEngineAfterLogOnThreadFunc( void* pvContext )
    {
        VxThread* poThread = ( VxThread* )pvContext;
        poThread->setIsThreadRunning( true );
        P2PEngine* p2pEngine = ( P2PEngine* )poThread->getThreadUserParam();
        if( p2pEngine && false == poThread->isAborted() )
        {
            p2pEngine->executeAfterLogOnThreadFunctions();
        }

        poThread->threadAboutToExit();
        return nullptr;
    }
}

//============================================================================
void P2PEngine::fromGuiAppStartup( const char* assetsDir, const char* rootDataDir )
{
	VxSetAppIsShuttingDown( false );
	enableTimerThread( true );

	LogMsg( LOG_INFO, "P2PEngine::fromGuiAppStartup %s %s", assetsDir, rootDataDir );
}

//============================================================================
void P2PEngine::fromGuiSetUserXferDir( const char* userXferDir )
{
	LogMsg( LOG_INFO, "P2PEngine::fromGuiSetUserXferDir %s", userXferDir );
	VxSetUserXferDirectory( userXferDir );
	std::string incompleteDir = VxGetIncompleteDirectory();
	// delete all incomplete files from previous usage
	std::vector<std::string>	fileList;
	VxFileUtil::listFilesInDirectory( incompleteDir.c_str(), fileList );
	std::vector<std::string>::iterator iter;
	for( iter = fileList.begin(); iter != fileList.end(); ++iter )
	{
		GetVxFileShredder().shredFile( *iter );
	}
}

//============================================================================
void P2PEngine::fromGuiSetUserSpecificDir( const char* userSpecificDir )
{
	LogMsg( LOG_INFO, "P2PEngine::fromGuiSetUserSpecificDir %s", userSpecificDir );
	VxSetUserSpecificDataDirectory( userSpecificDir );

	std::string strDbFileName = VxGetSettingsDirectory();
	strDbFileName += "biglist.db3";
	RCODE rc = m_BigListMgr.bigListMgrStartup( strDbFileName.c_str() );
	if( rc )
	{
		LogMsg( LOG_ERROR, "P2PEngine::startupEngine error %d bigListMgrStartup", rc );
	}

	strDbFileName = VxGetSettingsDirectory();
	strDbFileName += "HostUrlList.db3";
	getHostUrlListMgr().hostUrlListMgrStartup( strDbFileName );

	strDbFileName = VxGetSettingsDirectory();
	strDbFileName += "EngineSettings.db3";
	getEngineSettings().engineSettingsStartup( strDbFileName );

	strDbFileName = VxGetSettingsDirectory();
	strDbFileName += "EngineParams.db3";
	getEngineParams().engineParamsStartup( strDbFileName );

    strDbFileName = VxGetSettingsDirectory();
    strDbFileName += "IgnoredHostsList.db3";
    getIgnoreListMgr().ignoredHostsListMgrStartup( strDbFileName );

	strDbFileName = VxGetSettingsDirectory();
	strDbFileName += "HostedList.db3";
	getHostedListMgr().hostedListMgrStartup( strDbFileName );

	m_IsUserSpecificDirSet = true;
	m_AppStartupCalled = true;
}

//============================================================================
void P2PEngine::fromGuiUserLoggedOn( VxNetIdent* netIdent )
{
    //assureUserSpecificDirIsSet( "P2PEngine::fromGuiUserLoggedOn" );
    LogMsg( LOG_INFO, "P2PEngine fromGuiUserLoggedOn" );
    memcpy( ( VxNetIdent* )&m_PktAnn, netIdent, sizeof( VxNetIdent ) );
    m_PktAnn.setSrcOnlineId( netIdent->getMyOnlineId() );
    m_MyOnlineId = netIdent->getMyOnlineId();

    m_AssetMgr.fromGuiUserLoggedOn();
    // m_BlobMgr.fromGuiUserLoggedOn();
    m_ThumbMgr.fromGuiUserLoggedOn();
    m_HostJoinMgr.fromGuiUserLoggedOn();
    m_UserJoinMgr.fromGuiUserLoggedOn();
    m_UserOnlineMgr.fromGuiUserLoggedOn();

    // set network settings from saved settings
    startupEngine();
    updateFromEngineSettings( getEngineSettings() );
    m_PluginMgr.fromGuiUserLoggedOn();
    m_NetworkStateMachine.fromGuiUserLoggedOn();
    LogMsg( LOG_INFO, "P2PEngine fromGuiUserLoggedOn done" );
    m_IsEngineReady = true;
    enableAfterLogOnThread( true );
}

//============================================================================
void P2PEngine::enableAfterLogOnThread( bool enable )
{
    if( enable && !VxIsAppShuttingDown() )
    {
        static int threadStartCnt = 0;
        threadStartCnt++;
        std::string timerThreadName;
        StdStringFormat( timerThreadName, "AfterLogOnThread_%d", threadStartCnt );
        m_AfterLogOnThread.killThread();
        m_AfterLogOnThread.startThread( ( VX_THREAD_FUNCTION_T )P2PEngineAfterLogOnThreadFunc, this, timerThreadName.c_str() );

        LogModule( eLogThread, LOG_VERBOSE, "after log on thread %d started", threadStartCnt );
    }
    else
    {
        m_AfterLogOnThread.killThread();
    }
}

//============================================================================
void P2PEngine::executeAfterLogOnThreadFunctions( void )
{
    if( !m_AfterLogOnThread.isAborted() && !VxIsAppShuttingDown() )
    {
        m_PluginMgr.onAfterUserLogOnThreaded();
    }
}
