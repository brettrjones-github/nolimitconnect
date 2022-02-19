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

#include <GuiInterface/IGoTv.h>
#include <GuiInterface/IDefs.h>

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/NetServices/NetServiceHdr.h>
#include <ptop_src/ptop_engine_src/NetServices/NetServiceUtils.h>
#include <ptop_src/ptop_engine_src/Network/NetworkStateMachine.h>
#include <ptop_src/ptop_engine_src/Network/NetworkMgr.h>
#include <ptop_src/ptop_engine_src/UrlMgr/UrlMgr.h>

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
UrlActionInfo::UrlActionInfo( P2PEngine& engine, EHostType hostType, VxGUID& sessionId, ENetCmdType testType, const char * ptopUrl, const char * myUrl, 
                              UrlActionResultInterface* cbInterface, IConnectRequestCallback* cbConnectReq, EConnectReason connectReason )
    : m_Engine( engine )
    , m_HostType( hostType )
    , m_ResultCbInterface( cbInterface )
    , m_ConnectReqCbInterface( cbConnectReq )
    , m_TestType( testType )
    , m_MyUrl( myUrl )
    , m_RemoteUrl( ptopUrl )
    , m_ConnectReason( connectReason )
    , m_SessionId( sessionId )
{
    if( nullptr == myUrl )
    {
        m_MyUrl.setUrl( m_Engine.getMyOnlineUrl() );
    }
}

//============================================================================
UrlActionInfo::UrlActionInfo( const UrlActionInfo& rhs )
    : m_Engine( rhs.m_Engine )
    , m_HostType( rhs.m_HostType )
    , m_ResultCbInterface( rhs.m_ResultCbInterface )
    , m_ConnectReqCbInterface( rhs.m_ConnectReqCbInterface )
    , m_TestType( rhs.m_TestType )
    , m_MyUrl( rhs.m_MyUrl )
    , m_RemoteUrl( rhs.m_RemoteUrl )
    , m_ConnectReason( rhs.m_ConnectReason )
    , m_SessionId( rhs.m_SessionId )
{
}

//============================================================================
UrlActionInfo& UrlActionInfo::operator = ( const UrlActionInfo& rhs )
{
    if( this != &rhs )   
    {
        m_ResultCbInterface     = rhs.m_ResultCbInterface;
        m_ConnectReqCbInterface = rhs.m_ConnectReqCbInterface;
        m_TestType              = rhs.m_TestType;
        m_HostType              = rhs.m_HostType;
        m_MyUrl                 = rhs.m_MyUrl;
        m_RemoteUrl             = rhs.m_RemoteUrl;
        m_ConnectReason         = rhs.m_ConnectReason;
        m_SessionId             = rhs.m_SessionId;
    }

    return *this;
}

//============================================================================
bool UrlActionInfo::operator == ( const UrlActionInfo& rhs ) const
{
    return m_ResultCbInterface == rhs.m_ResultCbInterface &&
        m_ConnectReqCbInterface == rhs.m_ConnectReqCbInterface &&
        m_TestType == rhs.m_TestType &&
        m_HostType == rhs.m_HostType &&
        m_MyUrl == rhs.m_MyUrl &&
        m_RemoteUrl == rhs.m_RemoteUrl && m_ConnectReason == rhs.m_ConnectReason;
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
void RunUrlAction::runUrlAction( VxGUID& sessionId, ENetCmdType netCmdType, const char * ptopUrl, const char * myUrl, UrlActionResultInterface* cbInterface, 
                                 IConnectRequestCallback* cbConnectRequest, EHostType hostType, EConnectReason connectReason )
{
    UrlActionInfo urlAction( getEngine(), hostType, sessionId, netCmdType, ptopUrl, myUrl, cbInterface, cbConnectRequest, connectReason );
    std::string actionName = urlAction.getTestName();
    if( !urlAction.getMyVxUrl().validateUrl( true ) )
    {
        // if have callback interface then no need to send testing message
        LogModule( eLogRunTest, LOG_DEBUG, "RunUrlAction::runUrlAction Local URL is invalid" );
        sendRunTestStatus( urlAction, actionName, eRunTestStatusTestFail, eNetCmdErrorBadParameter, "My URL is invalid" );
        doRunTestFailed( urlAction, actionName, eRunTestStatusTestBadParam, eNetCmdErrorBadParameter );
    }
    else if( !urlAction.getRemoteVxUrl().validateUrl( false ) )
    {
        LogModule( eLogRunTest, LOG_DEBUG, "RunUrlAction::runUrlAction Remote URL is invalid" );
        sendRunTestStatus( urlAction, actionName, eRunTestStatusTestFail, eNetCmdErrorBadParameter, "Test URL is invalid" );
        doRunTestFailed( urlAction, actionName, eRunTestStatusTestBadParam, eNetCmdErrorBadParameter );
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
            sendRunTestStatus( urlAction, actionName, eRunTestStatusTestFail, eNetCmdErrorTxFailed, "URL action already qued for run" );
            doRunTestFailed( urlAction, actionName, eRunTestStatusAlreadyQueued, eNetCmdErrorTxFailed );
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
ERunTestStatus RunUrlAction::doUrlAction( UrlActionInfo& urlAction )
{
    std::string actionName = urlAction.getTestName();
    std::string nodeUrl = urlAction.getRemoteUrl();
    ENetCmdType netCmdType = urlAction.getNetCmdType();
    if( eNetCmdQueryHostOnlineIdReq == netCmdType && urlAction.getResultInterface() )
    {
        // if we are quering the host id a previous query for another service may have already gotten it for the given ip/port
        VxGUID hostId;
        if( m_Engine.getUrlMgr().lookupOnlineId( nodeUrl, hostId ) )
        {
            urlAction.getResultInterface()->callbackQueryIdSuccess( urlAction, hostId );
            return eRunTestStatusTestSuccess;
        }
    }

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
    case eNetCmdHostPingReq:
    case eNetCmdIsMyPortOpenReq:
    case eNetCmdQueryHostOnlineIdReq:
        break;
    default:
        LogModule( eLogRunTest, LOG_INFO, "RunUrlAction: unsupport net cmd %s", DescribeNetCmdType( netCmdType ) );
        sendRunTestStatus( urlAction, actionName, eRunTestStatusTestBadParam, eNetCmdErrorBadParameter, ": Unsupport net cmd %s", DescribeNetCmdType( netCmdType ) );
        doRunTestFailed( urlAction, actionName, eRunTestStatusTestBadParam, eNetCmdErrorBadParameter );
        return eRunTestStatusConnectFail;
    }

    LogModule( eLogRunTest, LOG_INFO, "RunUrlAction: sec %3.3f : connecting to %s thread 0x%x", 
               testTimer.elapsedSec(), nodeUrl.c_str(), VxGetCurrentThreadId() );
    std::string resolveIp("");
	if( false == netServConn.connectToWebsite(	nodeUrl.c_str(), 
		                                        strHost, 
		                                        strFile, 
		                                        u16Port, 
                                                resolveIp,
		                                        NETSERVICE_CONNECT_TIMEOUT ) )
	{
        sendRunTestStatus( urlAction, actionName, eRunTestStatusConnectFail, eNetCmdErrorConnectFailed, "Could not connected to %s IP %s..Please check settings",
                            nodeUrl.c_str(), resolveIp.c_str() );

		doRunTestFailed( urlAction, actionName, eRunTestStatusConnectFail, eNetCmdErrorConnectFailed );

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

    case eNetCmdHostPingReq:
        rxCmdHeaderTimeout = PING_TEST_RX_HDR_TIMEOUT;
        rxCmdDataTimeout = PING_TEST_RX_DATA_TIMEOUT;
        m_NetServiceUtils.buildHostPingReqUrl( &netServConn, strNetActionUrl );
        break;

    case eNetCmdIsMyPortOpenReq:
        myPort = urlAction.getMyVxUrl().getPort();
        if( 0 == myPort )
        {
            LogModule( eLogRunTest, LOG_INFO, "RunUrlAction: Invalid listen port %d", myPort );
            sendRunTestStatus( urlAction, actionName, eRunTestStatusTestBadParam, eNetCmdErrorBadParameter, ": Invalid listen port %d", myPort );
            netServConn.closeSkt();
            doRunTestFailed( urlAction, actionName, eRunTestStatusTestBadParam, eNetCmdErrorBadParameter );
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
        doRunTestFailed( urlAction, actionName, eRunTestStatusConnectFail, eNetCmdErrorConnectFailed );
        return eRunTestStatusConnectFail;
    }

    connectTime = testTimer.elapsedSec();
    LogModule( eLogRunTest, LOG_INFO, "RunUrlAction: action url %s", strNetActionUrl.c_str() );

	RCODE rc = netServConn.sendData( strNetActionUrl.c_str(), (int)strNetActionUrl.length() );
	if( rc )
	{
        netServConn.closeSkt();
        LogModule( eLogRunTest, LOG_ERROR, "RunUrlAction: sendData error %d", rc );
        sendRunTestStatus( urlAction, actionName, eRunTestStatusConnectionDropped, eNetCmdErrorTxFailed,
			"Connected to %s but connection was dropped (wrong network key ?) %s", nodeUrl.c_str(), m_Engine.getNetworkMgr().getNetworkKey() );
		return doRunTestFailed( urlAction, actionName, eRunTestStatusConnectionDropped, eNetCmdErrorTxFailed );
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
        netServConn.closeSkt();
		sendRunTestStatus( urlAction, actionName, eRunTestStatusConnectionDropped, eNetCmdErrorResponseTimedOut,
			"%s Connected to %s but failed to respond (wrong network key ?) thread 0x%x", actionName.c_str(), nodeUrl.c_str(), VxGetCurrentThreadId() );
		return doRunTestFailed( urlAction, actionName, eRunTestStatusConnectionDropped, eNetCmdErrorResponseTimedOut );
	}

    netServConn.closeSkt();

    rxBuf[ sizeof( rxBuf ) - 1 ] = 0;
	std::string content = rxBuf;
	reponseTime = testTimer.elapsedSec();
    LogModule( eLogRunTest, LOG_INFO, "RunUrlAction: response len %d total time %3.3fsec connect %3.3fsec send %3.3fsec response %3.3f sec thread 0x%x",
		content.length(),
		reponseTime, connectTime, sendTime - connectTime, reponseTime - sendTime, VxGetCurrentThreadId() );
	if( 0 == content.length() )
	{
        LogModule( eLogRunTest, LOG_ERROR, "RunUrlAction: no content in response" );
		sendRunTestStatus( urlAction, actionName, eRunTestStatusInvalidResponse, netServiceHdr.getError(), "Invalid response content %s", content.c_str() );
		return doRunTestFailed( urlAction, actionName, eRunTestStatusInvalidResponse, netServiceHdr.getError() );
	}

	const char * contentBuf = content.c_str();
    if( '/' != contentBuf[content.length() -1] )
    {
        LogModule( eLogRunTest, LOG_ERROR, "%s no trailing / in content thread 0x%x", actionName.c_str(), VxGetCurrentThreadId() );
        sendRunTestStatus( urlAction, actionName, eRunTestStatusInvalidResponse, netServiceHdr.getError(), "Invalid response content %s", content.c_str() );
        return doRunTestFailed( urlAction, actionName, eRunTestStatusInvalidResponse, netServiceHdr.getError() );
    }

    ((char *)contentBuf)[content.length() -1] = 0;

    std::string strPayload = content;
    if( content.empty() )
    {
        LogModule( eLogRunTest, LOG_ERROR, "%s No content thread 0x%x", actionName.c_str(), VxGetCurrentThreadId() );
        sendRunTestStatus( urlAction, actionName, eRunTestStatusInvalidResponse, netServiceHdr.getError(), "Invalid host id %s", content.c_str() );
        return doRunTestFailed( urlAction, actionName, eRunTestStatusInvalidResponse, netServiceHdr.getError() );
    }

    if( eNetCmdPing == netCmdType )
    {
        if( strPayload.empty() || strPayload.length() < 5  )
        {
            LogModule( eLogRunTest, LOG_ERROR, "RunUrlAction no content" );
            sendRunTestStatus( urlAction, actionName, eRunTestStatusInvalidResponse, netServiceHdr.getError(), "%s No PONG content", actionName.c_str() );
            return doRunTestFailed(  urlAction, actionName, eRunTestStatusInvalidResponse, netServiceHdr.getError() );
        }

        sendTestLog( urlAction, actionName, "%s PONG Content (%s)", actionName.c_str(), strPayload.c_str() );
        std::string strPongSig = strPayload.substr( 0, 5 );
        if( strPongSig != "PONG-" )
        {
            sendRunTestStatus( urlAction, actionName, eRunTestStatusInvalidResponse, netServiceHdr.getError(), "Content did not contain PONG- %s", content.c_str() );
            return doRunTestFailed( urlAction, actionName, eRunTestStatusInvalidResponse, netServiceHdr.getError() );
        }
        else
        {
            std::string strMyIP = strPayload.substr( 5, strPayload.length() - 5 );
            LogModule( eLogRunTest, LOG_VERBOSE, "%s PING test success .. My IP is %s", actionName.c_str(), strMyIP.c_str() );
            sendTestLog( urlAction, actionName, "Success. PONG returned My IP (%s)", strMyIP.c_str() );
            sendTestLog( urlAction, actionName, "Elapsed Seconds Connect %3.3f sec Send %3.3f sec Respond %3.3f sec", connectTime, sendTime - connectTime, reponseTime - sendTime );
            if( urlAction.getResultInterface() )
            {
                urlAction.getResultInterface()->callbackPingSuccess( urlAction, strMyIP.c_str() );
            }
        }
    }
    else if( eNetCmdHostPingReq == netCmdType )
    {
        if( strPayload.empty() || strPayload.length() < 5 )
        {
            LogModule( eLogRunTest, LOG_ERROR, "RunUrlAction no content" );
            sendRunTestStatus( urlAction, actionName, eRunTestStatusInvalidResponse, netServiceHdr.getError(), "%s No HOST PING REPLY content", actionName.c_str() );
            return doRunTestFailed( urlAction, actionName, eRunTestStatusInvalidResponse, netServiceHdr.getError() );
        }

        sendTestLog( urlAction, actionName, "%s HOST PING REPLY Content (%s)", actionName.c_str(), strPayload.c_str() );
    }
    else if( eNetCmdQueryHostOnlineIdReq == netCmdType )
    {
        VxGUID hostId;
        hostId.fromVxGUIDHexString( strPayload.c_str() );
        if( !hostId.isVxGUIDValid() )
        {
            LogModule( eLogRunTest, LOG_ERROR, "Query Host Online Id %s Invalid Content (%3.3f sec) thread 0x%x", content.c_str(), testTimer.elapsedSec(), VxGetCurrentThreadId() );
            sendRunTestStatus( urlAction, actionName, eRunTestStatusInvalidResponse, netServiceHdr.getError(), "Invalid host id %s", content.c_str() );
            netServConn.closeSkt();
            return doRunTestFailed( urlAction, actionName, eRunTestStatusInvalidResponse, netServiceHdr.getError() );
        }

        std::string hostIdStr = hostId.toHexString();
        LogModule( eLogRunTest, LOG_VERBOSE, "test success %s host id %s thread 0x%x", actionName.c_str(), hostIdStr.c_str(), VxGetCurrentThreadId() );
        sendTestLog( urlAction, actionName, "Action complete with Id %s Elapsed Seconds Connect %3.3fsec Send %3.3fsec Respond %3.3f sec", hostIdStr.c_str(), connectTime, sendTime - connectTime, reponseTime - sendTime );
        if( urlAction.getResultInterface() )
        {
            urlAction.getResultInterface()->callbackQueryIdSuccess( urlAction, hostId );
        }
    }
    else if( eNetCmdIsMyPortOpenReq  == netCmdType )
    {
        std::vector<std::string> contentParts;
        StdStringSplit( content, '-', contentParts );
        if( 2 != contentParts.size() )
        {
            LogModule( eLogRunTest, LOG_ERROR, "NetActionIsMyPortOpen::doAction: not enough parts to content" );
            sendRunTestStatus( urlAction, actionName, eRunTestStatusInvalidResponse, netServiceHdr.getError(), "Invalid response content %s", content.c_str() );
            return doRunTestFailed( urlAction, actionName, eRunTestStatusInvalidResponse, netServiceHdr.getError() );
        }

        std::string retMyExternalIp = contentParts[1];
        std::string strPayload = contentParts[0];
        int iIsOpen = atoi( contentParts[0].c_str() );
        // m_Engine.getNetStatusAccum().setIpAddress( retMyExternalIp );

        LogModule( eLogRunTest, LOG_VERBOSE, "NetActionIsMyPortOpen::doAction: direct connect %s my ip %s result %d thread 0x%x", strPayload.c_str(), retMyExternalIp.c_str(), iIsOpen, VxGetCurrentThreadId() );
        if( iIsOpen )
        {
            sendRunTestStatus(  urlAction, actionName, eRunTestStatusMyPortIsOpen, eNetCmdErrorNone, "My ip %s port %d is open", retMyExternalIp.c_str(), myPort );
        }
        else
        {
            sendRunTestStatus(  urlAction, actionName, eRunTestStatusMyPortIsClosed, netServiceHdr.getError(), "My ip %s port %d is NOT open (Relay will be required)", retMyExternalIp.c_str(), myPort );
            if( m_Engine.getNetworkStateMachine().getLocalNetworkIp().length()
                && m_Engine.getNetworkStateMachine().isCellularNetwork() )
            {
                sendTestLog(  urlAction, actionName, "Cellular data network. Cell phone providers block all ports. Consider using a VPN with port forward feature");
            }
        }

        if( urlAction.getResultInterface() )
        {
            urlAction.getResultInterface()->callbackConnectionTestSuccess( urlAction, iIsOpen, retMyExternalIp.c_str() );
        }

        sendTestLog(  urlAction, actionName, "Elapsed Seconds Connect %3.3f sec Send %3.3f sec Respond %3.3f sec", connectTime, sendTime - connectTime, reponseTime - sendTime );    
    }


    return doRunTestSuccess(  urlAction, actionName );
}

//============================================================================
ERunTestStatus RunUrlAction::doRunTestFailed( UrlActionInfo& urlAction, std::string& urlActionName, ERunTestStatus eTestStatus, ENetCmdError cmdErr )
{
    if( urlAction.getResultInterface() )
    {
        urlAction.getResultInterface()->callbackActionFailed( urlAction, eTestStatus, cmdErr );
    }

    sendRunTestStatus( urlAction, urlActionName, eTestStatus, cmdErr, "" );
	sendRunTestStatus( urlAction, urlActionName, eRunTestStatusTestCompleteFail, cmdErr, "" );
	return eRunTestStatusTestCompleteFail;
}

//============================================================================
ERunTestStatus RunUrlAction::doRunTestSuccess( UrlActionInfo& urlAction, std::string& urlActionName )
{
	sendRunTestStatus( urlAction, urlActionName, eRunTestStatusTestCompleteSuccess, eNetCmdErrorNone, "" );
	return eRunTestStatusTestCompleteSuccess;
}

//============================================================================
void RunUrlAction::sendRunTestStatus( UrlActionInfo& urlAction, std::string& urlActionName, ERunTestStatus eStatus, ENetCmdError cmdErr, const char * msg, ... )
{
    char as8Buf[ 1024 ];
    va_list argList;
    va_start( argList, msg );
    vsnprintf( as8Buf, sizeof( as8Buf ), msg, argList );
    as8Buf[ sizeof( as8Buf ) - 1 ] = 0;
    va_end( argList );
    std::string cmdMsg = as8Buf;
    cmdMsg += " - ";
    cmdMsg += DescribeNetCmdError( cmdErr );

    if( !urlAction.getResultInterface() )
    {
        IToGui::getToGui().toGuiRunTestStatus( urlActionName.c_str(), eStatus, cmdMsg.c_str() );
    }
    else
    {
        urlAction.getResultInterface()->callbackActionStatus( urlAction, eStatus, cmdErr, cmdMsg.c_str() );
    }
}

//============================================================================
void RunUrlAction::sendTestLog( UrlActionInfo& urlAction, std::string& urlActionName, const char * msg, ... )
{
    if( !urlAction.getResultInterface() )
    {
        char as8Buf[1024];
        va_list argList;
        va_start( argList, msg );
        vsnprintf( as8Buf, sizeof( as8Buf ), msg, argList );
        as8Buf[sizeof( as8Buf ) - 1] = 0;
        va_end( argList );
        IToGui::getToGui().toGuiRunTestStatus( urlActionName.c_str(), eRunTestStatusLogMsg, as8Buf );
    }
}
