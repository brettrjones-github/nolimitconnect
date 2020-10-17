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

#include "ActivityQueryHostIdTest.h"
#include "AppCommon.h"
#include "ActivityChooseTestWebsiteUrl.h"

#include <GoTvCore/GoTvP2P/P2PEngine/EngineSettings.h>
#include <NetLib/VxSktUtil.h>
#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>
#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxParse.h>
#include <CoreLib/VxGlobals.h>

#include <QMessageBox>
#include <QTimer>

#include <stdarg.h>
#include <stdio.h>

//============================================================================
ActivityQueryHostIdTest::ActivityQueryHostIdTest( 
				AppCommon&	app,
				uint16_t				port,
				QWidget *		parent )
: ActivityBase( OBJNAME_ACTIVITY_QUERY_HOST_ID_TEST, app, parent, eAppletMessenger, true )
, m_Port( port )
, m_Timer(new QTimer(this))
{
	m_Timer->setSingleShot(true);

	ui.setupUi(this);

    connectBarWidgets();

	connect( ui.m_TitleBarWidget, SIGNAL(signalBackButtonClicked()), this, SLOT(slotExitDialogButtonClick()) );
	connect( ui.m_RunTestButton, SIGNAL(clicked()), this, SLOT(slotRunTestButClick()) );
	connect( this, SIGNAL(signalDialogWasShown()), this, SLOT(slotDialogWasShown()) );

	connect(	&app, 
				SIGNAL(signalIsPortOpenStatus(EIsPortOpenStatus,QString)), 
				this, 
				SLOT(slotIsPortOpenStatus(EIsPortOpenStatus,QString)) );
    fillMyNodeUrl( ui.m_NodeUrlLabel );
}

//============================================================================
void ActivityQueryHostIdTest::slotExitDialogButtonClick( void )
{
	accept();
}

//============================================================================
void ActivityQueryHostIdTest::slotRunTestButClick( void )
{
	ui.m_LogEdit->clear();

	m_Engine.getFromGuiInterface().fromGuiRunQueryHostIdTest();
}

//============================================================================
void ActivityQueryHostIdTest::slotDialogWasShown( void )
{
	slotRunTestButClick();
}

//============================================================================
void ActivityQueryHostIdTest::slotIsPortOpenStatus( EIsPortOpenStatus ePortStatus, QString strMsg )
{
	strMsg.remove(QRegExp("[\\n\\r]"));
	ui.m_LogEdit->append(strMsg);
}
