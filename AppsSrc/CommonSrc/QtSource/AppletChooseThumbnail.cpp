//============================================================================
// Copyright (C) 2020 Brett R. Jones
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

#include "AppletChooseThumbnail.h"
#include "AppCommon.h"
#include "AppSettings.h"
#include "MyIcons.h"
#include "AppGlobals.h"
#include "AppCommon.h"
#include "AccountMgr.h"
#include "GuiHelpers.h"
#include "GuiParams.h"

#include <QMessageBox>
#include <QUuid>
#include <QFileDialog>

#include <ptop_src/ptop_engine_src/P2PEngine/EngineSettings.h>
#include <ptop_src/ptop_engine_src/AssetMgr/AssetMgr.h>

#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxDebug.h>
#include <CoreLib/VxGlobals.h>

//============================================================================
AppletChooseThumbnail::AppletChooseThumbnail( AppCommon& app, QWidget * parent )
: AppletBase( OBJNAME_APPLET_CHOOSE_THUMBNAIL, app, parent )
{
    setAppletType( eAppletChooseThumbnail );
    ui.setupUi( getContentItemsFrame() );
	setTitleBarText( DescribeApplet( m_EAppletType ) );

    QString infoText = QObject::tr( "Image for thumbnail will be resized to width " );
    infoText += QString::number( GuiParams::getThumbnailSize().width() );
    infoText += QObject::tr( " height " );
    infoText += QString::number( GuiParams::getThumbnailSize().height() );
    infoText += QObject::tr( " pixels. " );
    ui.m_InfoLabel->setText( infoText );

    connect( ui.m_ThumbnailEditWidget, SIGNAL( signalImageChanged() ), this, SLOT( slotImageChanged() ) );

	m_MyApp.activityStateChange( this, true );
}

//============================================================================
AppletChooseThumbnail::~AppletChooseThumbnail()
{
    m_MyApp.activityStateChange( this, false );
}

//============================================================================
void AppletChooseThumbnail::slotImageChanged( void )
{
    emit signalThumbSelected( this, ui.m_ThumbnailEditWidget );
}
