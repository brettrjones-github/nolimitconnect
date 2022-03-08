//============================================================================
// Copyright (C) 2018 Brett R. Jones
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

#include "AppletAboutMeClient.h"
#include "AppletMgr.h"
#include "AppCommon.h"
#include "AppSettings.h"
#include "MyIcons.h"
#include "AppletSnapshot.h"
#include "AppGlobals.h"
#include "AppCommon.h"
#include "AccountMgr.h"
#include "GuiHelpers.h"

#include <QMessageBox>
#include <QUuid>
#include <QFileDialog>

#include <ptop_src/ptop_engine_src/P2PEngine/EngineSettings.h>
#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxDebug.h>
#include <CoreLib/VxGlobals.h>

//============================================================================
AppletAboutMeClient::AppletAboutMeClient( AppCommon& app, QWidget * parent )
: AppletBase( OBJNAME_APPLET_ABOUT_ME_CLIENT, app, parent )
{
    setAppletType( eAppletAboutMeClient );
    ui.setupUi( getContentItemsFrame() );
	setTitleBarText( DescribeApplet( m_EAppletType ) );

	m_MyApp.activityStateChange( this, true );
}

//============================================================================
AppletAboutMeClient::~AppletAboutMeClient()
{
    m_MyApp.activityStateChange( this, false );
}

//============================================================================
void AppletAboutMeClient::loadRichTextFile( QString fileName )
{
    ui.m_RichEditWidget->loadRichTextFile( fileName.toUtf8().constData() );
}

//============================================================================
void AppletAboutMeClient::onBrowseButClick( void )
{
    QString startPath = QDir::current().path();

    // Get a filename from the file dialog.
    QString filename = QFileDialog::getOpenFileName( this,
                                                     QObject::tr( "Open Image" ),
                                                     startPath,
                                                     SUPPORTED_IMAGE_FILES );
    if( filename.length() > 0 )
    {
        QPixmap oBitmap;
        if( false == oBitmap.load( filename ) )
        {
            QString msgText = QObject::tr( "Failed To Read Image File " ) + filename;
            QMessageBox::critical( this, QObject::tr( "Error Reading Image" ), msgText );
        }
        else
        {

        }
    }
}
