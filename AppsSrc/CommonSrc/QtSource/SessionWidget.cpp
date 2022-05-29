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
// http://www.nolimitconnect.org
//============================================================================

#include "AppCommon.h"
#include "SessionWidget.h"
#include "ui_SessionWidget.h"

#include <QDebug>

//============================================================================
SessionWidget::SessionWidget( QWidget *parent, EAssetType inputMode )
: QWidget( parent )
, m_MyApp(GetAppInstance())
, m_InputMode( inputMode )
, m_IsInitialized( false )
{
	qDebug() << "SessionWidget input mode " << inputMode;
	ui.setupUi(this);
	connect( ui.m_ChatEntry, SIGNAL(signalUserInputButtonClicked()), this, SIGNAL(signalUserInputButtonClicked()) );
    connect( &m_MyApp, SIGNAL( signalStatusMsg( QString ) ), this, SLOT( slotStatusMsg( QString ) ) );

	setEntryMode( m_InputMode );
}

//============================================================================
void SessionWidget::slotStatusMsg( QString msg )
{
    if( !msg.isEmpty() )
    {
        ui.m_StatusLabel->setText( msg );
    }
}

//============================================================================
void SessionWidget::setIdents( GuiUser * myIdent, GuiUser * hisIdent )
{
	ui.m_ChatEntry->setIdents( myIdent, hisIdent );
	ui.m_HistoryList->setIdents( myIdent, hisIdent );
	m_IsInitialized = true;
}

//============================================================================
void SessionWidget::setEntryMode( EAssetType inputMode )
{
	ui.m_ChatEntry->setEntryMode( inputMode );
}

//============================================================================
void SessionWidget::setIsPersonalRecorder( bool isPersonal )
{
	ui.m_ChatEntry->setIsPersonalRecorder( isPersonal );
}

//============================================================================
void SessionWidget::setCanSend( bool canSend )
{
	ui.m_ChatEntry->setCanSend( canSend );
}

//============================================================================
void SessionWidget::playVideoFrame( VxGUID& onlineId, unsigned char * pu8Jpg, unsigned long u32JpgLen, int motion0To100000 )
{
	if( m_IsInitialized )
	{
		ui.m_ChatEntry->playVideoFrame( onlineId, pu8Jpg, u32JpgLen, motion0To100000 );
	}
}

//============================================================================
void SessionWidget::onActivityStop( void )
{
	if( m_IsInitialized )
	{
		ui.m_HistoryList->onActivityStop();
	}
}
