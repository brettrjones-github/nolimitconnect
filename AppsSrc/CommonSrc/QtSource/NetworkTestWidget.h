#pragma once
//============================================================================
// Copyright (C) 2019 Brett R. Jones
//
// You may use, copy, modify, merge, publish, distribute, sub-license, and/or sell this software
// provided this Copyright is not modified or removed and is included all copies or substantial portions of the Software
//
// This code is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// bjones.engineer@gmail.com
// http://www.nolimitconnect.org
//============================================================================

#include "ActivityBase.h"
#include <QDialog>
#include "ui_NetworkTestWidget.h"

#include <CoreLib/VxDefs.h>
#include <GuiInterface/IToGui.h>

class AppCommon;
class P2PEngine;

class NetworkTestWidget : public QWidget
{
	Q_OBJECT
public:
	NetworkTestWidget( QWidget* parent = nullptr );
    virtual ~NetworkTestWidget() override = default;

protected slots:
	//void						gotoWebsite( void );
	//void						slotNetworkStateChanged( ENetworkStateType eNetworkState );
    void                        slotRunFullTestButClick( void );
    void                        slotIsPortOpenButClick( void );

protected:
    LogWidget*                  getLogWidget( void ) { return ui.m_LogWidget; }
	//void						setupNetworkTestWidget( void );

	//void						showNoInternetHelp( void );
	//void						showDiscoverNetworkHelp( void );
	//void						showRelaySearchHelp( void );
	//void						showDirectConnectMessage( void );
	//void						showRelayConnectedHelp( void );
	//void						showHostError( void );

	//void						clearHelpLines( void );
	//void						setHelpLine( int lineIdx, QString helpText );

	//=== vars ===//
	Ui::NetworkTestWidget	    ui;
    AppCommon&                  m_MyApp;
    QTimer *                    m_Timer;

};
