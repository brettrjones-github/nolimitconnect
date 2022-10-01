#pragma once
//============================================================================
// Copyright (C) 2016 Brett R. Jones
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
// http://www.nolimitconnect.org
//============================================================================

#include "ActivityBase.h"
#include <QDialog>
#include "ui_ActivityNetworkStatus.h"

#include <CoreLib/VxDefs.h>
#include <GuiInterface/IToGui.h>

class AppCommon;
class P2PEngine;

class ActivityNetworkState : public ActivityBase
{
	Q_OBJECT
public:
	ActivityNetworkState( AppCommon& app, QWidget* parent = NULL );
    virtual ~ActivityNetworkState() override = default;

    // overrides required for dialogs with there own title bar and bottom bar widgets
    virtual TitleBarWidget *	getTitleBarWidget( void ) override { return ui.m_TitleBarWidget; }
    virtual BottomBarWidget *	getBottomBarWidget( void ) override { return ui.m_BottomBarWidget; }

protected slots:
	void						gotoWebsite( void );
	void						slotNetworkStateChanged( ENetworkStateType eNetworkState );

protected:
	void						setupActivityNetworkState( void );

	void						showNoInternetHelp( void );
	void						showDiscoverNetworkHelp( void );
	void						showWaitForRelayHelp( void );
	void						showDirectConnectMessage( void );
	void						showRelayConnectedHelp( void );
    void						showHostNetworkError( void );
    void						showHostGroupError( void );
    void						showHostGroupListError( void );

	void						clearHelpLines( void );
	void						setHelpLine( int lineIdx, QString helpText );

	//=== vars ===//
	Ui::ActivityNetworkStatusClass	ui;

};
