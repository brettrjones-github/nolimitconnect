#pragma once
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

#include "NetworkTestBase.h"

#include <CoreLib/VxUrl.h>

class UrlActionInfo;

class UrlActionResultInterface
{
public:
    virtual void                callbackActionStatus( UrlActionInfo& actionInfo, ERunTestStatus eStatus, std::string statusMsg ) = 0;
    virtual void                callbackActionFailed( UrlActionInfo& actionInfo, ERunTestStatus eStatus, ENetCmdError netCmdError = eNetCmdErrorUnknown ) = 0;

    virtual void                callbackPingSuccess( UrlActionInfo& actionInfo, std::string myIp ) = 0;
    virtual void                callbackConnectionTestSuccess( UrlActionInfo& actionInfo, bool canDirectConnect, std::string myIp ) = 0;
    virtual void                callbackQueryIdSuccess( UrlActionInfo& actionInfo, VxGUID onlineId ) = 0;
};

class UrlActionInfo
{
public:
    UrlActionInfo();
    UrlActionInfo(P2PEngine& engine, EHostType hostType, ENetCmdType testType, const char * ptopUrl, const char * myUrl, UrlActionResultInterface* cbInterface);
    UrlActionInfo( const UrlActionInfo& rhs );
    UrlActionInfo&              operator = ( const UrlActionInfo& rhs );
    bool                        operator == ( const UrlActionInfo& rhs ) const;
    bool                        operator != ( const UrlActionInfo& rhs ) const;

    P2PEngine&					getEngine( void )               { return m_Engine; }
    EHostType                   getHostType( void )             { return m_HostType; }
    VxUrl&                      getMyVxUrl( void )              { return m_MyUrl; }
    std::string&                getMyUrl( void )                { return m_MyUrl.getUrl(); }
    ENetCmdType                 getNetCmdType( void )           { return m_TestType; }
    VxUrl&                      getRemoteVxUrl( void )          { return m_RemoteUrl; }
    std::string&                getRemoteUrl( void )            { return m_RemoteUrl.getUrl(); }
    UrlActionResultInterface*   getResultInterface( void )      { return m_CallbackInterface; }

    std::string                 getTestName( void );

protected:
    //=== vars ===//
    P2PEngine&					m_Engine;
    EHostType                   m_HostType{ eHostTypeUnknown };
    UrlActionResultInterface*   m_CallbackInterface{ nullptr };
    ENetCmdType                 m_TestType{ eNetCmdUnknown };
    VxUrl                       m_MyUrl;
    VxUrl                       m_RemoteUrl;
};


class RunUrlAction
{
public:
    RunUrlAction( P2PEngine& engine, EngineSettings& engineSettings, NetServicesMgr& netServicesMgr, NetServiceUtils& netServiceUtils );
	virtual ~RunUrlAction() = default;
    RunUrlAction() = delete; // don't allow default constructor
    RunUrlAction(const RunUrlAction&) = delete; // don't allow copy constructor

    P2PEngine&					getEngine() { return m_Engine; }

	void				        runUrlAction( ENetCmdType netCmdType, 
                                              const char * ptopUrl, 
                                              const char * myUrl = nullptr, 
                                              EHostType hostType = eHostTypeUnknown, 
                                              UrlActionResultInterface* cbInterface = nullptr );
	void						runTestShutdown( void );

	void						threadFuncRunUrlAction( void );

private:
    void                        startUrlActionThread( void );
    bool                        isThreadRunningActions( void );
    ERunTestStatus			    doUrlAction( UrlActionInfo& urlInfo );
    ERunTestStatus			    doRunTestFailed( UrlActionInfo& urlAction, std::string& urlActionName, ERunTestStatus eTestStatus );
    ERunTestStatus			    doRunTestSuccess( UrlActionInfo& urlAction, std::string& urlActionName );

    void						sendRunTestStatus( UrlActionInfo& urlAction, std::string& urlActionName, ERunTestStatus eTestStatus, const char * msg, ... );
    void						sendTestLog( UrlActionInfo& urlAction, std::string& urlActionName, const char * msg, ... );

    //=== vars ===//
    P2PEngine&					m_Engine;
    EngineSettings&				m_EngineSettings;
    NetServicesMgr&				m_NetServicesMgr;
    NetServiceUtils&			m_NetServiceUtils;

    std::vector<UrlActionInfo>  m_UrlActionList;
    VxMutex                     m_ActionListMutex;
    bool                        m_ThreadIsRunningActions{ false };

    VxThread					m_RunActionThread;
};

