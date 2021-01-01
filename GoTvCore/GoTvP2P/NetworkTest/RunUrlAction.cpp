//============================================================================
// Copyright (C) 2020 Brett R. Jones 
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

#include "RunUrlAction.h"

#include <GoTvInterface/IGoTv.h>

#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>
#include <GoTvCore/GoTvP2P/NetServices/NetServiceHdr.h>
#include <GoTvCore/GoTvP2P/NetServices/NetServiceUtils.h>
#include <GoTvCore/GoTvP2P/Network/NetworkStateMachine.h>
#include <GoTvCore/GoTvP2P/Network/NetworkMgr.h>

#include <CoreLib/VxDebug.h>
#include <CoreLib/VxParse.h>
#include <CoreLib/VxGlobals.h>

#include <NetLib/VxSktUtil.h>
#include <NetLib/VxPeerMgr.h>
#include <NetLib/VxSktConnectSimple.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _MSC_VER
# pragma warning(disable: 4355) //'this' : used in base member initializer list
#endif //_MSC_VER

namespace
{
    //============================================================================
    void * UrlActionThreadFunc( void * pvContext )
    {
        VxThread * poThread = ( VxThread * )pvContext;
        poThread->setIsThreadRunning( true );
        RunUrlAction * netTest = ( RunUrlAction * )poThread->getThreadUserParam();
        if( netTest && false == poThread->isAborted() )
        {
            netTest->threadFuncRunUrlAction();
        }

        poThread->threadAboutToExit();
        return nullptr;
    }
}

//============================================================================
UrlActionInfo::UrlActionInfo()
    : m_Engine( GetPtoPEngine() )
{
}

//============================================================================
UrlActionInfo::UrlActionInfo( P2PEngine& engine, ENetCmdType testType, const char * ptopUrl, const char * myUrl, UrlActionResultInterface* cbInterface )
    : m_Engine( engine )
    , m_CallbackInterface( cbInterface )
    , m_TestType( testType )
    , m_MyUrl( myUrl )
    , m_RemoteUrl( ptopUrl )
{
    if( nullptr == myUrl )
    {
        m_MyUrl.setUrl( m_Engine.getMyUrl() );
    }

}

//============================================================================
UrlActionInfo::UrlActionInfo( const UrlActionInfo& rhs )
    : m_Engine( rhs.m_Engine )
    , m_CallbackInterface( rhs.m_CallbackInterface )
    , m_TestType( rhs.m_TestType )
    , m_MyUrl( rhs.m_MyUrl )
    , m_RemoteUrl( rhs.m_RemoteUrl )
{
}

//============================================================================
UrlActionInfo& UrlActionInfo::operator = ( const UrlActionInfo& rhs )
{
    if( this != &rhs )   
    {
        m_CallbackInterface     = rhs.m_CallbackInterface;
        m_TestType              = rhs.m_TestType;
        m_MyUrl                 = rhs.m_MyUrl;
        m_RemoteUrl             = rhs.m_RemoteUrl;
    }

    return *this;
}

//============================================================================
bool UrlActionInfo::operator == ( const UrlActionInfo& rhs ) const
{
    return m_CallbackInterface == rhs.m_CallbackInterface &&
        m_TestType == rhs.m_TestType &&
        m_MyUrl == rhs.m_MyUrl &&
        m_RemoteUrl == rhs.m_RemoteUrl;
}

//============================================================================
bool UrlActionInfo::operator != ( const UrlActionInfo& rhs ) const
{
    return !(*this == rhs);
}

//============================================================================
std::string UrlActionInfo::getTestName( void )
{
    return DescribeNetCmdType( getNetCmdType() );
}

//============================================================================
//============================================================================
RunUrlAction::RunUrlAction( P2PEngine& engine, EngineSettings& engineSettings, NetServicesMgr& netServicesMgr, NetServiceUtils& netServiceUtils )
    : m_Engine( engine )
    , m_EngineSettings( engineSettings )
    , m_NetServicesMgr( netServicesMgr )
    , m_NetServiceUtils( netServiceUtils )
{
}

//============================================================================
void RunUrlAction::runTestShutdown( void )
{
    m_RunActionThread.abortThreadRun( true );
}

//============================================================================
void RunUrlAction::runUrlAction( ENetCmdType netCmdType, const char * ptopUrl, const char * myUrl, UrlActionResultInterface* cbInterface )
{
    UrlActionInfo urlAction( getEngine(), netCmdType, ptopUrl, myUrl, cbInterface );
    if( !urlAction.getMyVxUrl().validateUrl( true ) )
    {
        // if have callback interface then no need to send testing message
        LogModule( eLogRunTest, LOG_DEBUG, "RunUrlAction::runUrlAction Local URL is invalid" );
        if( !cbInterface )
        {
            sendRunTestStatus( urlAction.getTestName(), eRunTestStatusTestFail, "My URL is invalid" );
        }
    }
    else if( !urlAction.getRemoteVxUrl().validateUrl( false ) )
    {
        LogModule( eLogRunTest, LOG_DEBUG, "RunUrlAction::runUrlAction Remote URL is invalid" );
        if( !cbInterface )
        {
            sendRunTestStatus( urlAction.getTestName(), eRunTestStatusTestFail, "Test URL is invalid" );
        }
    }
    else
    {
        m_ActionListMutex.lock();
        // if already exists in que then do not que again
        bool existsInQue = false;
        for( UrlActionInfo& urlActionInfo : m_UrlActionList )
        {
            if( urlActionInfo == urlAction )
            {
                existsInQue = true;
                break;
            }
        }

        if( existsInQue )
        {
            m_ActionListMutex.unlock();
            if( !cbInterface )
            {
                sendRunTestStatus( urlAction.getTestName(), eRunTestStatusTestFail, "URL action already qued for run" );
            }
        }
        else
        {
            m_UrlActionList.push_back( urlAction );
            m_ActionListMutex.unlock();
            startUrlActionThread();
        }
    }
}

//============================================================================
void RunUrlAction::startUrlActionThread( void )
{
    if( !isThreadRunningActions() && !m_RunActionThread.isAborted() )
    {
        m_ThreadIsRunningActions = true;
        m_RunActionThread.startThread( ( VX_THREAD_FUNCTION_T )UrlActionThreadFunc, this, "UrlActionThread" );
    }
}

//============================================================================
bool RunUrlAction::isThreadRunningActions( void )
{
    return m_ThreadIsRunningActions;
}

//============================================================================
void RunUrlAction::threadFuncRunUrlAction( void )
{
    UrlActionInfo urlAction;
    while( m_ThreadIsRunningActions && !m_RunActionThread.isAborted() )
    {
        m_ActionListMutex.lock();
        size_t listSize = m_UrlActionList.size();
        if( listSize )
        {
            urlAction = m_UrlActionList.front();
            m_UrlActionList.erase( m_UrlActionList.begin() );
            m_ActionListMutex.unlock();
            doUrlAction( urlAction );
        }
        else
        {
            m_ThreadIsRunningActions = false;
            m_ActionListMutex.unlock();
            break;
        }
    }    
}

//============================================================================
ERunTestStatus RunUrlAction::doUrlAction( UrlActionInfo& urlInfo )
{
    std::string actionName = urlInfo.getTestName();
    std::string nodeUrl = urlInfo.getRemoteUrl();
    ENetCmdType netCmdType = urlInfo.getNetCmdType();

	VxSktConnectSimple netServConn;
	std::string strHost;
	std::string strFile;
	uint16_t u16Port;
	VxTimer testTimer;
	double connectTime = 0;
	double sendTime= 0;
	double reponseTime= 0;

    switch( netCmdType )
    {
    case eNetCmdPing:
    case eNetCmdIsMyPortOpenReq:
    case eNetCmdQueryHostOnlineIdReq:
        break;
    default:
        LogModule( eLogRunTest, LOG_INFO, "RunUrlAction: unsupport net cmd %s", DescribeNetCmdType( netCmdType ) );
        sendRunTestStatus( actionName, eRunTestStatusTestBadParam, ": Unsupport net cmd %s", DescribeNetCmdType( netCmdType ) );
        doRunTestFailed( actionName );
        return eRunTestStatusConnectFail;
    }

    LogModule( eLogRunTest, LOG_INFO, "RunUrlAction: sec %3.3f : connecting to %s thread 0x%x", 
               testTimer.elapsedSec(), nodeUrl.c_str(), VxGetCurrentThreadId() );
	if( false == netServConn.connectToWebsite(	nodeUrl.c_str(), 
		strHost, 
		strFile, 
		u16Port, 
		NETSERVICE_CONNECT_TIMEOUT ) )
	{
        sendRunTestStatus( actionName, eRunTestStatusConnectFail, "Could not connected to %s..Please check settings thread 0x%x",
            nodeUrl.c_str(), VxGetCurrentThreadId() );

		doRunTestFailed( actionName );

		return eRunTestStatusConnectFail;
	}

	netServConn.dumpConnectionInfo();
	std::string strNetActionUrl;
    uint16_t myPort = 0;
    int rxCmdHeaderTimeout = PING_TEST_RX_HDR_TIMEOUT;
    int rxCmdDataTimeout = PING_TEST_RX_DATA_TIMEOUT;

    switch( netCmdType )
    {
    case eNetCmdPing:
        rxCmdHeaderTimeout = PING_TEST_RX_HDR_TIMEOUT;
        rxCmdDataTimeout = PING_TEST_RX_DATA_TIMEOUT;
        m_NetServiceUtils.buildPingTestUrl( &netServConn, strNetActionUrl );
        break;

    case eNetCmdIsMyPortOpenReq:
        myPort = urlInfo.getMyVxUrl().getPort();
        if( 0 == myPort )
        {
            LogModule( eLogRunTest, LOG_INFO, "RunUrlAction: Invalid listen port %d", myPort );
            sendRunTestStatus( actionName, eRunTestStatusTestBadParam, ": Invalid listen port %d", myPort );
            netServConn.closeSkt();
            doRunTestFailed( actionName );
            return eRunTestStatusTestBadParam;
        }

        rxCmdHeaderTimeout = IS_PORT_OPEN_RX_HDR_TIMEOUT;
        rxCmdDataTimeout = IS_PORT_OPEN_RX_DATA_TIMEOUT;
        m_NetServiceUtils.buildIsMyPortOpenUrl( &netServConn, strNetActionUrl, myPort );
        break;

    case eNetCmdQueryHostOnlineIdReq:
        rxCmdHeaderTimeout = QUERY_HOST_ID_RX_HDR_TIMEOUT;
        rxCmdDataTimeout = QUERY_HOST_ID_RX_DATA_TIMEOUT;
        m_NetServiceUtils.buildQueryHostIdUrl( &netServConn, strNetActionUrl );
        break;

    default:
        netServConn.closeSkt();
        doRunTestFailed( actionName );
        return eRunTestStatusConnectFail;
    }

    connectTime = testTimer.elapsedSec();
    LogModule( eLogRunTest, LOG_INFO, "RunUrlAction: action url %s", strNetActionUrl.c_str() );

	RCODE rc = netServConn.sendData( strNetActionUrl.c_str(), (int)strNetActionUrl.length() );
	if( rc )
	{
        LogModule( eLogRunTest, LOG_ERROR, "RunUrlAction: sendData error %d", rc );
        sendRunTestStatus( actionName, eRunTestStatusConnectionDropped,
			"Connected to %s but connection was dropped (wrong network key ?) %s",nodeUrl.c_str(), m_Engine.getNetworkMgr().getNetworkKey() );
        netServConn.closeSkt();
		return doRunTestFailed( actionName );
	}

	sendTime = testTimer.elapsedSec();
    VxSleep( 100 ); // wait a bit for the response

	char rxBuf[ 513 ];
    rxBuf[ 0 ] = 0;
	NetServiceHdr netServiceHdr;
	if( false == m_NetServiceUtils.rxNetServiceCmd( &netServConn, 
													rxBuf, 
													sizeof( rxBuf ) - 1, 
													netServiceHdr, 
                                                    rxCmdHeaderTimeout,
                                                    rxCmdDataTimeout ) )
	{
		sendRunTestStatus( actionName, eRunTestStatusConnectionDropped,
			"%s Connected to %s but failed to respond (wrong network key ?) thread 0x%x", actionName.c_str(), nodeUrl.c_str(), VxGetCurrentThreadId() );
        netServConn.closeSkt();
		return doRunTestFailed( actionName );
	}

    rxBuf[ sizeof( rxBuf ) - 1 ] = 0;
	std::string content = rxBuf;
	reponseTime = testTimer.elapsedSec();
    LogModule( eLogRunTest, LOG_INFO, "RunUrlAction: response len %d total time %3.3fsec connect %3.3fsec send %3.3fsec response %3.3f sec thread 0x%x",
		content.length(),
		reponseTime, connectTime, sendTime - connectTime, reponseTime - sendTime, VxGetCurrentThreadId() );
	if( 0 == content.length() )
	{
        LogModule( eLogRunTest, LOG_ERROR, "RunUrlAction: no content in response" );
		sendRunTestStatus( actionName, eRunTestStatusInvalidResponse, "Invalid response content %s\n", content.c_str() );
        netServConn.closeSkt();
		return doRunTestFailed( actionName );
	}

	const char * contentBuf = content.c_str();
    if( '/' != contentBuf[content.length() -1] )
    {
        LogModule( eLogRunTest, LOG_ERROR, "%s no trailing / in content thread 0x%x", actionName.c_str(), VxGetCurrentThreadId() );
        sendRunTestStatus( actionName, eRunTestStatusInvalidResponse, "Invalid response content %s\n", content.c_str() );
        return doRunTestFailed( actionName );
    }

    ((char *)contentBuf)[content.length() -1] = 0;

    std::string strPayload = content;
    if( content.empty() )
    {
        LogModule( eLogRunTest, LOG_ERROR, "%s No content thread 0x%x", actionName.c_str(), VxGetCurrentThreadId() );
        sendRunTestStatus( actionName, eRunTestStatusInvalidResponse, "Invalid host id %s\n", content.c_str() );
        netServConn.closeSkt();
        return doRunTestFailed( actionName );
    }

    if( eNetCmdPing == netCmdType )
    {
        if( strPayload.empty() || strPayload.length() < 5  )
        {
            LogModule( eLogRunTest, LOG_ERROR, "RunUrlAction no content" );
            sendRunTestStatus( actionName, eRunTestStatusInvalidResponse, "%s No PONG content\n", actionName.c_str() );
            netServConn.closeSkt();
            return doRunTestFailed( actionName );
        }

        sendTestLog( actionName, "%s PONG Content (%s)", actionName.c_str(), strPayload.c_str() );
        std::string strPongSig = strPayload.substr( 0, 5 );
        if( strPongSig != "PONG-" )
        {
            sendRunTestStatus( actionName, eRunTestStatusInvalidResponse, "Content did not contain PONG- %s\n", content.c_str() );
            netServConn.closeSkt();
            return doRunTestFailed( actionName );
        }
        else
        {
            std::string strMyIP = strPayload.substr( 5, strPayload.length() - 5 );
            LogModule( eLogRunTest, LOG_VERBOSE, "%s PING test success .. My IP is %s", actionName.c_str(), strMyIP.c_str() );
            sendTestLog( actionName, "Success. PONG returned My IP (%s)", strMyIP.c_str() );
            sendTestLog( actionName, "Elapsed Seconds Connect %3.3f sec Send %3.3f sec Respond %3.3f sec", connectTime, sendTime - connectTime, reponseTime - sendTime );
            netServConn.closeSkt();
            return doRunTestSuccess( actionName );
        }
    }
    else if( eNetCmdQueryHostOnlineIdReq == netCmdType )
    {
        VxGUID hostId;
        hostId.fromVxGUIDHexString( strPayload.c_str() );
        if( !hostId.isVxGUIDValid() )
        {
            LogMsg( LOG_ERROR, "Query Host Online Id %s Invalid Content (%3.3f sec) thread 0x%x", content.c_str(), testTimer.elapsedSec(), VxGetCurrentThreadId() );
            sendRunTestStatus( actionName, eRunTestStatusInvalidResponse, "Invalid host id %s\n", content.c_str() );
            netServConn.closeSkt();
            return doRunTestFailed( actionName );
        }

        std::string hostIdStr = hostId.toHexString();
        LogModule( eLogRunTest, LOG_VERBOSE, "test success %s host id %s thread 0x%x", actionName.c_str(), hostIdStr.c_str(), VxGetCurrentThreadId() );
        sendTestLog( actionName, "Action complete with Id %s Elapsed Seconds Connect %3.3fsec Send %3.3fsec Respond %3.3f sec", hostIdStr.c_str(), connectTime, sendTime - connectTime, reponseTime - sendTime );
    }
    else if( eNetCmdIsMyPortOpenReq  == netCmdType )
    {
        std::vector<std::string> contentParts;
        StdStringSplit( content, '-', contentParts );
        if( 2 != contentParts.size() )
        {
            LogMsg( LOG_ERROR, "NetActionIsMyPortOpen::doAction: not enough parts to content thread 0x%x", VxGetCurrentThreadId() );
            sendRunTestStatus( actionName, eRunTestStatusInvalidResponse, "Invalid response content %s\n", content.c_str() );
            return doRunTestFailed( actionName );
        }

        std::string retMyExternalIp = contentParts[1];
        std::string strPayload = contentParts[0];
        int iIsOpen = atoi( contentParts[0].c_str() );
        // m_Engine.getNetStatusAccum().setIpAddress( retMyExternalIp );

        LogModule( eLogIsPortOpenTest, LOG_VERBOSE, "NetActionIsMyPortOpen::doAction: direct connect %s my ip %s result %d thread 0x%x", strPayload.c_str(), retMyExternalIp.c_str(), iIsOpen, VxGetCurrentThreadId() );
        if( iIsOpen )
        {
            sendRunTestStatus( actionName, eRunTestStatusMyPortIsOpen, "My ip %s port %d is open\n", retMyExternalIp.c_str(), myPort );
        }
        else
        {
            sendRunTestStatus( actionName, eRunTestStatusMyPortIsClosed, "My ip %s port %d is NOT open (Relay will be required)\n", retMyExternalIp.c_str(), myPort );
            if( m_Engine.getNetworkStateMachine().getLocalNetworkIp().length()
                && m_Engine.getNetworkStateMachine().isCellularNetwork() )
            {
                sendTestLog( actionName, "Cellular data network. Cell phone providers block all ports. Consider using a VPN with port forward feature");
            }
        }

        sendTestLog( actionName, "Elapsed Seconds Connect %3.3f sec Send %3.3f sec Respond %3.3f sec", connectTime, sendTime - connectTime, reponseTime - sendTime );    
    }

    netServConn.closeSkt();
    return doRunTestSuccess( actionName );
}

//============================================================================
ERunTestStatus RunUrlAction::doRunTestFailed( std::string& urlActionName )
{
	sendRunTestStatus( urlActionName, eRunTestStatusTestCompleteFail,
		"\n" );
	return eRunTestStatusTestCompleteFail;
}

//============================================================================
ERunTestStatus RunUrlAction::doRunTestSuccess( std::string& urlActionName )
{
	sendRunTestStatus( urlActionName, eRunTestStatusTestCompleteSuccess,
		"\n" );
	return eRunTestStatusTestCompleteSuccess;
}

//============================================================================
void RunUrlAction::sendRunTestStatus( std::string& urlActionName, ERunTestStatus eStatus, const char * msg, ... )
{
    char as8Buf[ 1024 ];
    va_list argList;
    va_start( argList, msg );
    vsnprintf( as8Buf, sizeof( as8Buf ), msg, argList );
    as8Buf[ sizeof( as8Buf ) - 1 ] = 0;
    va_end( argList );
    IToGui::getToGui().toGuiRunTestStatus( urlActionName.c_str(), eStatus, as8Buf );
}

//============================================================================
void RunUrlAction::sendTestLog( std::string& urlActionName, const char * msg, ... )
{
    char as8Buf[ 1024 ];
    va_list argList;
    va_start( argList, msg );
    vsnprintf( as8Buf, sizeof( as8Buf ), msg, argList );
    as8Buf[ sizeof( as8Buf ) - 1 ] = 0;
    va_end( argList );
    IToGui::getToGui().toGuiRunTestStatus( urlActionName.c_str(), eRunTestStatusLogMsg, as8Buf );
}
