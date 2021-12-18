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

#include "ActivityDownloadItemMenu.h"
#include "AppCommon.h"

#include "GuiFileXferSession.h"
#include "MyIcons.h"

//============================================================================
ActivityDownloadItemMenu::ActivityDownloadItemMenu(	AppCommon& app, 
													GuiFileXferSession * poSession, 
													QWidget * parent )
: ActivityBase( OBJNAME_ACTIVITY_DOWNLOAD_ITEM_MENU, app, parent, eAppletMessengerFrame, false, true )
, m_ePluginType( ePluginTypeFileServer )
, m_Session( poSession )
{
	ui.setupUi(this);
	ui.m_TitleBarWidget->setTitleBarText( QObject::tr("Download Item Menu") );

    connectBarWidgets();

	setupStyledDlg(	poSession->getIdent(), 
					ui.FriendIdentWidget,
					m_ePluginType,
					ui.PermissionButton, 
					ui.PermissionLabel );

	ui.PermissionButton->setIcon( getMyIcons().getFileIcon( poSession->getFileType() ) );
	ui.PermissionLabel->setText( poSession->describeFileType() );

	ui.FileNameEdit->setText( m_Session->getJustFileName() );

	connect( ui.CancelButton, SIGNAL(clicked()), this, SLOT(onCancelButClick()) );
}

//============================================================================
void ActivityDownloadItemMenu::onCancelButClick()
{
	accept();
}
