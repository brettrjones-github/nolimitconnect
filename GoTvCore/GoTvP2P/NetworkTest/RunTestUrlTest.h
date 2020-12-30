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

class RunTestUrlTest : public NetworkTestBase
{
public:
    RunTestUrlTest( P2PEngine& engine, EngineSettings& engineSettings, NetServicesMgr& netServicesMgr, NetServiceUtils& netServiceUtils );
	virtual ~RunTestUrlTest() = default;
    RunTestUrlTest() = delete; // don't allow default constructor
    RunTestUrlTest(const RunTestUrlTest&) = delete; // don't allow copy constructor

	virtual void				fromGuiRunTestUrlTest( const char * myUrl, const char * ptopUrl, ENetCmdType testType );
	void						runTestShutdown( void );

	void						threadRunNetworkTest( void ) override;

private:
    ERunTestStatus			    doRunTest( std::string& nodeUrl );
    ERunTestStatus			    doRunTestFailed( void );
    ERunTestStatus			    doRunTestSuccess( void );

	//=== vars ===//
    bool                        m_TestIsRunning{ false };
    VxUrl                       m_MyUrl;
    VxUrl                       m_TestUrl;
};

