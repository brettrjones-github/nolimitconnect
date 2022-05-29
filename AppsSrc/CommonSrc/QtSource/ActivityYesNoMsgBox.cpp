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
// http://www.nolimitconnect.org
//============================================================================

#include "ActivityYesNoMsgBox.h"
#include "MyIcons.h"
#include "AppGlobals.h"
#include "AppCommon.h"
#include "GuiHelpers.h"

#include <PktLib/VxCommon.h>
#include <CoreLib/VxDebug.h>

//============================================================================
ActivityYesNoMsgBox::ActivityYesNoMsgBox(	AppCommon&	app, 
											QWidget *		parent, 
											QString			title, 
											QString			bodyText ) 
: ActivityBase( OBJNAME_ACTIVITY_YES_NO_MSG_BOX, app, parent, eAppletMessengerFrame, true )
{
	ui.setupUi(this);
    connectBarWidgets();
	ui.m_AcceptButton->setIcon( eMyIconAcceptNormal );
	ui.m_CancelButton->setIcon( eMyIconCancelNormal );
	setTitle( title );
	ui.m_BodyTextLabel->setText( bodyText );

    connect(ui.m_TitleBarWidget, SIGNAL(signalBackButtonClicked()), this, SLOT(slotHomeButtonClicked()));

	connect( ui.m_AcceptButton, SIGNAL(clicked()), this, SLOT(accept()) );
	connect( ui.m_CancelButton, SIGNAL(clicked()), this, SLOT(reject()) );
	this->setFocus();
	ui.m_AcceptButton->setFocus();
}


//============================================================================
void ActivityYesNoMsgBox::setTitle( QString strTitle )
{
	ui.m_TitleBarWidget->setTitleBarText(strTitle);
}

//============================================================================
void ActivityYesNoMsgBox::setBodyText( QString strBodyText )
{
	ui.m_BodyTextLabel->setText( strBodyText );
}

//============================================================================
void ActivityYesNoMsgBox::hideCancelButton( void )
{
	ui.m_CancelButton->setVisible( false );
}

//============================================================================
void ActivityYesNoMsgBox::makeNeverShowAgainVisible( bool makeVisible )
{
	ui.m_NeverShowAgainCheckBox->setVisible( makeVisible );
}

//============================================================================
bool ActivityYesNoMsgBox::wasNeverShowAgainChecked( void )
{
	return ui.m_NeverShowAgainCheckBox->isChecked();
}

//============================================================================
void ActivityYesNoMsgBox::slotHomeButtonClicked( void )
{
	reject();
}
