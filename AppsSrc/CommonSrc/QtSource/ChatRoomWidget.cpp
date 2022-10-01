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


#include "ChatRoomWidget.h"

#include <QDebug>

//============================================================================
ChatRoomWidget::ChatRoomWidget( QWidget*parent, EAssetType inputMode )
: QWidget( parent )
, m_InputMode( inputMode )
, m_IsInitialized( false )
{
	qDebug() << "ChatRoomWidget input mode " << inputMode;
	ui.setupUi(this);
	connect( getSessionChatEntry(), SIGNAL(signalUserInputButtonClicked()), this, SIGNAL(signalUserInputButtonClicked()) );

	setEntryMode( m_InputMode );
}

//============================================================================
void ChatRoomWidget::setIdents( GuiUser * myIdent, GuiUser * hisIdent )
{
    getSessionChatEntry()->setIdents( myIdent, hisIdent );
    getSessionHistoryList()->setIdents( myIdent, hisIdent );
	m_IsInitialized = true;
}

//============================================================================
void ChatRoomWidget::setEntryMode( EAssetType inputMode )
{
    getSessionChatEntry()->setEntryMode( inputMode );
}

//============================================================================
void ChatRoomWidget::setIsPersonalRecorder( bool isPersonal )
{
    getSessionChatEntry()->setIsPersonalRecorder( isPersonal );
}

//============================================================================
void ChatRoomWidget::setCanSend( bool canSend )
{
    getSessionChatEntry()->setCanSend( canSend );
}

//============================================================================
void ChatRoomWidget::setAppModule( EAppModule appModule )
{
	getSessionChatEntry()->setAppModule( appModule );
}

//============================================================================
void ChatRoomWidget::callbackGuiPlayMotionVideoFrame( VxGUID& feedOnlineId, QImage& vidFrame, int motion0To100000 )
{
	if( m_IsInitialized )
	{
        getSessionChatEntry()->callbackGuiPlayMotionVideoFrame( feedOnlineId, vidFrame, motion0To100000 );
	}
}

//============================================================================
void ChatRoomWidget::onActivityStop( void )
{
	if( m_IsInitialized )
	{
        getSessionHistoryList()->onActivityStop();
	}
}
