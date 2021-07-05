#pragma once
//============================================================================
// Copyright (C) 2020 Brett R. Jones 
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

#include <GuiInterface/IToGui.h>

#include <CoreLib/VxThread.h>
#include <CoreLib/VxMutex.h>

class P2PEngine;
class EngineSettings;
class VxSktConnectSimple;
class NetworkMgr;
class NetServicesMgr;
class NetServiceUtils;

class NetworkTestBase
{
public:
    NetworkTestBase( P2PEngine& engine, EngineSettings& engineSettings, NetServicesMgr& netServicesMgr, NetServiceUtils& netServiceUtils );
    virtual ~NetworkTestBase() = default;

    P2PEngine&					getEngine( void ) { return m_Engine; }

    void                        setIsHost( bool isHost ) { m_IsHost = isHost; }
    bool                        getIsHost( void ) { return m_IsHost; }

    // sets both client and host test to same name
    void                        setTestName( std::string name ) { setClientTestName( name ); setHostTestName( name ); }
    // get test name depending on if m_IsHost is set or not
    std::string                 getTestName( void ) { return m_IsHost ? getHostTestName() : getClientTestName(); }

    void                        setClientTestName( std::string name ) { m_TestMutex.lock(); m_ClientTestName = name; m_TestMutex.unlock(); }
    std::string                 getClientTestName( void ) { std::string testName;  m_TestMutex.lock(); testName = m_ClientTestName; m_TestMutex.unlock(); return testName; }

    void                        setHostTestName( std::string name ) { m_TestMutex.lock(); m_HostTestName = name; m_TestMutex.unlock(); }
    std::string                 getHostTestName( void ) { std::string testName;  m_TestMutex.lock(); testName = m_HostTestName; m_TestMutex.unlock(); return testName; }


    virtual void				startNetworkTest( void );
    virtual void				stopNetworkTest( void );

    /// called from thread to run test
    virtual void				threadRunNetworkTest( void ) = 0;
    /// called from thread just before thread exit
    virtual void				networkTestComplete( void ){};

    void						sendRunTestStatus( ERunTestStatus eStatus, const char * msg, ... );
    void						sendTestLog( const char * msg, ... );

protected:
    //=== vars ===//
    P2PEngine&					m_Engine;
    EngineSettings&				m_EngineSettings;
    NetServicesMgr&				m_NetServicesMgr;
    NetServiceUtils&			m_NetServiceUtils;
    VxMutex                     m_TestMutex;
    VxThread					m_RunTestThread;
    bool                        m_TestIsRunning{ false };
    bool                        m_IsHost{ false };
    std::string                 m_ClientTestName;
    std::string                 m_HostTestName;

private:
    NetworkTestBase() = delete; // don't allow default constructor
    NetworkTestBase( const NetworkTestBase& ) = delete; // don't allow copy constructor
};