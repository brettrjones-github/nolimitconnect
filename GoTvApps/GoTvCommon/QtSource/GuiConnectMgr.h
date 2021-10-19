#pragma once
//============================================================================
// Copyright (C) 2021 Brett R. Jones
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

#include <ptop_src/ptop_engine_src/ConnectMgr/ConnectCallbackInterface.h>

#include <QObject>

class AppCommon;

class GuiConnectMgr : public QObject, public ConnectCallbackInterface
{
    Q_OBJECT
public:
    GuiConnectMgr() = delete;
    GuiConnectMgr( AppCommon& app );
    GuiConnectMgr( const GuiConnectMgr& rhs ) = delete;
	virtual ~GuiConnectMgr() = default;

    virtual void                onAppCommonCreated( void );
    virtual void                onMessengerReady( bool ready ) { }
    virtual bool                isMessengerReady( void );
    virtual void                onSystemReady( bool ready ) { }

protected:
    
    AppCommon&                  m_MyApp;
};
