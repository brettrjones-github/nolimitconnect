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

#include <app_precompiled_hdr.h>
#include "NetworkTestWidget.h"
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
NetworkTestWidget::NetworkTestWidget( QWidget *		parent )
    : QWidget( parent )
    , m_MyApp( GetAppInstance() )
    , m_Timer(new QTimer(this))
{
	m_Timer->setSingleShot(true);

	ui.setupUi(this);

    connect( &m_MyApp,
             SIGNAL( signalIsPortOpenStatus( EIsPortOpenStatus, QString ) ),
             this,
             SLOT( slotIsPortOpenStatus( EIsPortOpenStatus, QString ) ) );

    connect( ui.m_RunFullTestButton, SIGNAL( clicked() ), this, SLOT( slotRunFullTestButClick() ) );
    connect( ui.m_IsMyPortOpenButton, SIGNAL( clicked() ), this, SLOT( slotIsPortOpenButClick() ) );
}

//============================================================================
void NetworkTestWidget::slotRunFullTestButClick( void )
{
	bool isBusyWithTest = false;
	if( false == isBusyWithTest )
	{
		isBusyWithTest = true;
		getLogWidget()->clear();
		//m_MyApp.getEngine().getFromGuiInterface().fromGuiVerifyNetHostSettings();
		isBusyWithTest = false;
	}
}

//============================================================================
void NetworkTestWidget::slotIsPortOpenButClick( void )
{
    uint16_t tcpPort = m_MyApp.getEngine().getEngineSettings().getTcpIpPort();

    if( !tcpPort )
    {
        QMessageBox::information( this, QObject::tr( "Network Settings Error" ), QObject::tr( "TCP Port cannot be zero." ) );
    }
    else
    {
        getLogWidget()->clear();
        m_MyApp.getEngine().getFromGuiInterface().fromGuiRunIsPortOpenTest( tcpPort );
    }
}


//
////============================================================================
//void NetworkTestWidget::slotHostStatus( EHostTestStatus eHostStatus, QString strMsg )
//{
//	//strMsg.remove(QRegExp("[\\n\\r]"));
//	ui.m_LogEdit->append(strMsg);
//}
