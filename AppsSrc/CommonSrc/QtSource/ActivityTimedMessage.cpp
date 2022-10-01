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

#include "ActivityTimedMessage.h"

//============================================================================
ActivityTimedMessage::ActivityTimedMessage( QString strTitle, QString strMessage, int iTimeoutSeconds, QWidget* parent )
	: QDialog( parent, Qt::SubWindow )
{
	ui.setupUi(this);
	QRect oRect = parent->geometry();
	oRect.setRight(oRect.right() - oRect.left());
	oRect.setLeft(0);
	oRect.setBottom(oRect.bottom() - oRect.top());
	oRect.setTop(0);
	this->setGeometry(oRect);


	connect( ui.m_TitleBarWidget, SIGNAL(signalBackButtonClicked()), this, SLOT(reject()) );

	this->ui.m_TitleBarWidget->setTitleBarText( strTitle );
	this->ui.LogEdit->append(strMessage);
	if( iTimeoutSeconds )
	{
		connect( &m_CloseDlgTimer, SIGNAL(timeout()), this, SLOT(accept()) );
		m_CloseDlgTimer.setSingleShot( true );
		m_CloseDlgTimer.start( iTimeoutSeconds * 1000 );
	}
}

