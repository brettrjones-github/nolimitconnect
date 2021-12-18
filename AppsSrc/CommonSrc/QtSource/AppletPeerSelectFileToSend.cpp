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

#include "AppletPeerSelectFileToSend.h"
#include "ActivityMessageBox.h"
#include "ActivityBrowseFiles.h"
#include "ActivityViewLibraryFiles.h"

#include "AppGlobals.h"
#include "MyIcons.h"
#include "AppCommon.h"

#include <ptop_src/ptop_engine_src/P2PEngine/FileShareSettings.h>
#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxDebug.h>

#include <QFileDialog>

//============================================================================
AppletPeerSelectFileToSend::AppletPeerSelectFileToSend(	AppCommon&	app, QWidget * parent )
: AppletPeerBase( OBJNAME_ACTIVITY_SELECT_FILE_TO_SEND, app, parent )
{
    /*
    , m_ePluginType( ePluginTypeFileXfer )
	ui.setupUi(this);
	ui.m_TitleBarWidget->setTitleBarText( QObject::tr( "Offer File" ) );
	connect( &m_MyApp,				SIGNAL(signalStatusMsg(QString)),		ui.m_TitleBarWidget,	SLOT(slotTitleStatusBarMsg(QString)) );
    */

    setPluginType( ePluginTypeFileXfer );
    setAppletType( eAppletPeerSelectFileToSend );
    ui.setupUi( getContentItemsFrame() );
    setTitleBarText( DescribeApplet( m_EAppletType ) );

	//ui.m_FriendIdentWidget->updateGuiFromData( m_HisIdent );
	EPluginAccess ePluginAccess = m_HisIdent->getMyAccessPermissionFromHim( m_ePluginType );

	ui.m_PermissionButton->setIcon( getMyIcons().getPluginIcon( m_ePluginType, ePluginAccess ) );
	QString strPermissionFromHim = DescribePluginType( m_ePluginType );
	ui.m_PermissionLabel->setText( strPermissionFromHim );	

	ui.m_ChooseFromLibraryButton->setIcons( eMyIconLibraryNormal );
	ui.m_BrowseForFileButton->setIcons( eMyIconFileBrowseNormal );
	ui.m_SendOfferButton->setIcons( eMyIconSendFileNormal );
	ui.m_OfferMsgEdit->appendPlainText(QObject::tr("File "));
    connectBarWidgets();

	connect( ui.m_ChooseFromLibraryButton,	SIGNAL(clicked()),							this,	SLOT(onBrowseLibraryButClick()) );
	connect( ui.m_ChooseFromLibraryLabel,	SIGNAL(clicked()),							this,	SLOT(onBrowseLibraryButClick()) );
	connect( ui.m_BrowseForFileButton,		SIGNAL(clicked()),							this,	SLOT(onBrowseForFileButClick()) );
	connect( ui.m_BrowseForFileLabel,		SIGNAL(clicked()),							this,	SLOT(onBrowseForFileButClick()) );
	connect( ui.m_SendOfferButton,			SIGNAL(clicked()),							this,	SLOT(onSendButClick()) );
}

//============================================================================
bool AppletPeerSelectFileToSend::verifyUserInput( void )
{
	if( (0 != ui.m_FileNameEdit->text().length()) &&
		VxFileUtil::fileExists( ui.m_FileNameEdit->text().toUtf8().constData() ) )
	{
		return true;
	}

	ActivityMessageBox errMsgBox( m_MyApp, this, LOG_ERROR, "No File Specified");
	errMsgBox.exec();
	return false;
}

//============================================================================
void AppletPeerSelectFileToSend::onBrowseLibraryButClick()
{
	ActivityViewLibraryFiles dlg( m_MyApp, this, true );
	dlg.exec();
	if( dlg.getWasFileSelected() )
	{
		ui.m_FileNameEdit->setText( dlg.getSelectedFileName() );
	}
}

//============================================================================
void AppletPeerSelectFileToSend::onBrowseForFileButClick()
{
	ActivityBrowseFiles dlg( m_MyApp, eFileFilterAll, this, true );
	dlg.exec();
	if( dlg.getWasFileSelected() )
	{
		ui.m_FileNameEdit->setText( dlg.getSelectedFileName() );
	}
}

//============================================================================  
void AppletPeerSelectFileToSend::onSendButClick()
{
	if( verifyUserInput() )
	{
		if( false == m_LclSessionId.isVxGUIDValid() )
		{
			m_LclSessionId.initializeWithNewVxGUID();
		}

		if( false == m_FromGui.fromGuiMakePluginOffer(	m_ePluginType, 
														m_HisIdent->getMyOnlineId(), 
                                                        0,
														ui.m_OfferMsgEdit->toPlainText().toUtf8().constData(), 
														ui.m_FileNameEdit->text().toUtf8().constData(),
														0,
														m_LclSessionId ) )
		{
			ActivityMessageBox errMsgBox( m_MyApp, this, LOG_INFO, "%s is offline", m_HisIdent->getOnlineName() );
			errMsgBox.exec();
			reject();
		}

		accept();
	}
}
