//============================================================================
// Copyright (C) 2019 Brett R. Jones
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
#include "AppCommon.h"	
#include "AppSettings.h"

#include "AppletSnapshot.h"

#include "FileShareItemWidget.h"
#include "MyIcons.h"
#include "ListEntryWidget.h"
#include "PopupMenu.h"
#include "AppGlobals.h"
#include "FileItemInfo.h"
#include "FileActionMenu.h"
#include "GuiHelpers.h"
#include "GuiParams.h"

#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>
#include <GoTvCore/GoTvP2P/AssetMgr/AssetMgr.h>

#include <PktLib/VxSearchDefs.h>
#include <NetLib/VxFileXferInfo.h>
#include <CoreLib/VxFileInfo.h>
#include <CoreLib/VxGlobals.h>
#include <VxVideoLib/VxVideoLib.h>

#include <QResizeEvent>
#include <QMessageBox>
#include <QTimer>

//============================================================================
AppletSnapshot::AppletSnapshot(	AppCommon& app, QWidget * parent )
: AppletBase( OBJNAME_APPLET_SNAPSHOT, app, parent )
, m_CloseDlgTimer( new QTimer( this ) )
{
    setAppletType( eAppletSnapshot );
    ui.setupUi( getContentItemsFrame() );
    setTitleBarText( DescribeApplet( m_EAppletType ) );

    ui.m_ImageScreen->setFixedSize( 320, 240 );
    ui.m_SnapshotScreen->setFixedSize( 320, 240 );

    connect( ui.snapshotButton, SIGNAL( clicked() ), this, SLOT( onSnapShotButClick() ) );
    connect( ui.cancelButton, SIGNAL( clicked() ), this, SLOT( onCancelButClick() ) );

    if( m_MyApp.getCamLogic().isCamAvailable() )
    {
        m_MyApp.getEngine().fromGuiWantMediaInput( eMediaInputVideoJpgSmall, this, this, true );
    }
    else
    {
        QMessageBox::warning( this, QObject::tr( "Camera Capture" ), QObject::tr( "No Camera Source Available." ) );
        connect( m_CloseDlgTimer, SIGNAL( timeout() ), this, SLOT( onCancelButClick() ) );
        m_CloseDlgTimer->setSingleShot( true );
        m_CloseDlgTimer->start( 1000 );
    }

    m_MyApp.activityStateChange( this, true );
}

//============================================================================
AppletSnapshot::~AppletSnapshot()
{
    m_MyApp.activityStateChange( this, false );
}

//============================================================================
//! take picture for me
void AppletSnapshot::onSnapShotButClick( void )
{
    m_SnapShotPending = true;
}

//============================================================================
void AppletSnapshot::callbackVideoJpgSmall( void * userData, VxGUID& vidFeedId, uint8_t * jpgData, uint32_t jpgDataLen, int motion0to100000 )
{
    if( jpgData && jpgDataLen && ( vidFeedId == m_MyApp.getMyOnlineId() ) )
    {
        VxLabel* camScreen = ui.m_ImageScreen;
        if( camScreen )
        {
            camScreen->playVideoFrame( jpgData, jpgDataLen, motion0to100000 );
        }

        if( m_SnapShotPending )
        {
            QImage capBitmap;
            if( capBitmap.loadFromData( jpgData, jpgDataLen, "JPG" ) )
            {
                VxLabel* camScreen = ui.m_SnapshotScreen;
                if( camScreen )
                {
                    camScreen->playVideoFrame( jpgData, jpgDataLen, motion0to100000 );
                }

                m_ImageBitmap = capBitmap;
                emit signalSnapshotImage( m_ImageBitmap );
            }

            m_SnapShotPending = false;
        }
    }
}

//============================================================================
//! Implement the OnClickListener callback    
void AppletSnapshot::onCancelButClick( void )
{
    onBackButtonClicked();
}

//============================================================================
void AppletSnapshot::onCloseEvent( void )
{
    m_MyApp.getEngine().fromGuiWantMediaInput( eMediaInputVideoJpgSmall, this, this, false );
    AppletBase::onCloseEvent();
}

