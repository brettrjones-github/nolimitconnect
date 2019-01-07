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
// brett.jones@engineer.com
// http://www.gotvptop.net
//============================================================================

#include "ActivityBase.h"
#include "ui_ActivityNetServiceTest.h"

#include <GoTvInterface/IToGui.h>
#include <NetLib/InetAddress.h>

#include <QDialog>

class VxNetIdent;
class NetSettings;

class ActivityNetServiceTest : public ActivityBase
{
	Q_OBJECT
public:
	ActivityNetServiceTest(	AppCommon&	app,
							QWidget *		parent );

	virtual ~ActivityNetServiceTest();


public slots:
	void						slotAnchorStatus( EAnchorTestStatus eAnchorStatus, QString strMsg );

	void						slotExitDialogButtonClick( void );
	void						slotRunTestButClick( void );
	void						slotDialogWasShown( void );

protected:

	//=== vars ===//
	Ui::NetServiceTestDlg		ui; 
	std::vector<InetAddress>	m_aoIpAddress;
	QTimer *					m_Timer;

	std::string					m_strMyExternalIPv4;
	std::string					m_strMyLclIPv4;
};
