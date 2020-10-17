#pragma once
//============================================================================
// Copyright (C) 2013 Brett R. Jones
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

#include "ActivityBase.h"
#include "ui_ActivityNetworkHostTest.h"

#include <GoTvInterface/IToGui.h>
#include <NetLib/InetAddress.h>

#include <QDialog>

class VxNetIdent;
class NetSettings;

class ActivityNetworkHostTest : public ActivityBase
{
	Q_OBJECT
public:
	ActivityNetworkHostTest(	AppCommon&	app,
							uint16_t	port,
							QWidget *	parent );

	virtual ~ActivityNetworkHostTest() override = default;

    // overrides required for dialogs with there own title bar and bottom bar widgets
    virtual TitleBarWidget *	getTitleBarWidget( void ) override { return ui.m_TitleBarWidget; }
    virtual BottomBarWidget *	getBottomBarWidget( void ) override { return ui.m_BottomBarWidget; }

public slots:
	void						slotIsPortOpenStatus( EIsPortOpenStatus eIsPortOpenStatus, QString strMsg );

	void						slotExitDialogButtonClick( void );
	void						slotRunTestButClick( void );
	void						slotDialogWasShown( void );

protected:
 
	//=== vars ===//
	Ui::NetworkHostTestUi		ui; 
	uint16_t					m_Port;
	std::vector<InetAddress>	m_aoIpAddress;
	QTimer *					m_Timer;

	std::string					m_strMyExternalIPv4;
	std::string					m_strMyLclIPv4;
};