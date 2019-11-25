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

#include "AppletEditStoryboard.h"
#include "AppCommon.h"
#include "AppSettings.h"
#include "MyIcons.h"
#include "ActivitySnapShot.h"
#include "AppGlobals.h"
#include "AppCommon.h"
#include "AccountMgr.h"
#include "GuiHelpers.h"

#include <QMessageBox>
#include <QUuid>
#include <QFileDialog>

#include <GoTvCore/GoTvP2P/P2PEngine/EngineSettings.h>
#include <GoTvCore/GoTvP2P/AssetMgr/AssetMgr.h>

#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxDebug.h>
#include <CoreLib/VxGlobals.h>

#define SUPPORTED_IMAGE_FILES "Image files (*.bmp *.gif *.jpg *.jpeg *.png *.pbm *.pgm *.ppm *.xbm *.xpm)"

//============================================================================
AppletEditStoryboard::AppletEditStoryboard( AppCommon& app, QWidget * parent )
: AppletBase( OBJNAME_APPLET_EDIT_STORYBOARD, app, parent )
{
    setAppletType( eAppletEditStoryboard );
    ui.setupUi( getContentItemsFrame() );
	setTitleBarText( DescribeApplet( m_EAppletType ) );

    connect( ui.StoryEditWidget, SIGNAL( signalStoryBoardSavedModified() ), this, SLOT( slotStoryBoardSavedModified() ) );

    // get current working directory
    VxFileUtil::getCurrentWorkingDirectory( m_strSavedCwd );

    m_strStoryBoardDir = VxGetUserProfileDirectory();
    VxFileUtil::setCurrentWorkingDirectory( m_strStoryBoardDir.c_str() );
    m_strStoryBoardFile = m_strStoryBoardDir + "story_board.htm";
    ui.StoryEditWidget->loadStoryBoardFile( m_strStoryBoardFile.c_str() );

	m_MyApp.activityStateChange( this, true );
}


//============================================================================
void AppletEditStoryboard::slotStoryBoardSavedModified( void )
{
    m_FromGui.fromGuiUserModifiedStoryboard();
}
