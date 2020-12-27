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

class RunTestUrlTest : public NetworkTestBase
{
public:
    RunTestUrlTest( P2PEngine& engine, EngineSettings& engineSettings, NetServicesMgr& netServicesMgr, NetServiceUtils& netServiceUtils );
	virtual ~RunTestUrlTest() = default;


	virtual void				fromGuiRunQueryHostIdTest( const char * ptopUrl, int testType );
	void						runTestShutdown( void );

	void						threadRunNetworkTest( void ) override;

private:
    ERunTestStatus			    doRunTest( std::string& nodeUrl );
    ERunTestStatus			    doRunTestFailed( void );
    ERunTestStatus			    doRunTestSuccess( void );

	//=== vars ===//
    bool                        m_TestIsRunning{ false };

private:
	RunTestUrlTest() = delete; // don't allow default constructor
	RunTestUrlTest(const RunTestUrlTest&) = delete; // don't allow copy constructor
};

