//============================================================================
// Copyright (C) 2015 Brett R. Jones
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

#include "InputTextWidget.h"
#include "AppCommon.h"
#include "GuiParams.h"

#include <time.h>

//============================================================================
InputTextWidget::InputTextWidget( QWidget * parent )
: InputBaseWidget( GetAppInstance(), parent )
{
	m_AssetInfo.setAssetType( eAssetTypeChatText );

	ui.setupUi( this );
    ui.m_CancelTextButton->setSquareButtonSize( eButtonSizeTiny );
    ui.m_SendTextButton->setSquareButtonSize( eButtonSizeTiny );

	ui.m_CancelTextButton->setIcons( eMyIconSendArrowCancel );
	ui.m_SendTextButton->setIcons( eMyIconSendArrowNormal );

	connect( ui.m_CancelTextButton, SIGNAL(clicked()), this, SLOT(slotCancelButtonClicked()) );
	connect( ui.m_SendTextButton, SIGNAL(clicked()), this, SLOT(slotSendButtonClicked()) );
	ui.m_ChatTextEdit->setFocus();
}

//============================================================================
void InputTextWidget::slotCancelButtonClicked( void )
{
	emit signalInputCompleted();
}

//============================================================================
void InputTextWidget::slotSendButtonClicked( void )
{
	// What did they want to say (minus white space around the string):
	QString message = ui.m_ChatTextEdit->toPlainText().trimmed();

	// Only send the text if it's not empty
	if(!message.isEmpty())
	{
		message += "\n";
		m_AssetInfo.setAssetName( message.toUtf8().constData() );
		m_AssetInfo.generateNewUniqueId();
		m_AssetInfo.setAssetLength( message.length() );
		m_TimeRecCurrent	= GetTimeStampMs();
		m_AssetInfo.setCreationTime( m_TimeRecCurrent );
		m_MyApp.getEngine().fromGuiAssetAction( m_IsPersonalRecorder ? eAssetActionAddToAssetMgr : eAssetActionAddAssetAndSend,  m_AssetInfo );
	}

	ui.m_ChatTextEdit->clear();

	// Put the focus back into the input box so they can type again:
	ui.m_ChatTextEdit->setFocus();
	emit signalInputCompleted();
}
