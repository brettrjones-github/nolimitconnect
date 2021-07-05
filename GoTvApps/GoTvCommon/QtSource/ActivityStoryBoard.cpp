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

#include "ActivityStoryBoard.h"
#include "MyIcons.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxGlobals.h>

#define SESSION_TIMEOUT 15000

ActivityStoryBoard * g_poStoryBoardActivity = NULL;

//============================================================================
ActivityStoryBoard::ActivityStoryBoard(	AppCommon&	app, 
										QWidget *		parent )
: ActivityBase( OBJNAME_ACTIVITY_STORYBOARD, app, parent, eAppletMessenger, true ) 
{
	ui.setupUi(this);

    connectBarWidgets();

    connect( ui.StoryEditWidget, SIGNAL(signalStoryBoardSavedModified()), this, SLOT(slotStoryBoardSavedModified()));
    connect( ui.m_TitleBarWidget, SIGNAL(signalBackButtonClicked()), this, SLOT(slotHomeButtonClicked()) );

	// get current working directory
	VxFileUtil::getCurrentWorkingDirectory( m_strSavedCwd );

    m_strStoryBoardDir = VxGetUserProfileDirectory();
	VxFileUtil::setCurrentWorkingDirectory( m_strStoryBoardDir.c_str() );
	m_strStoryBoardFile = m_strStoryBoardDir + "story_board.htm";
	ui.StoryEditWidget->loadStoryBoardFile( m_strStoryBoardFile.c_str() );

	g_poStoryBoardActivity = this;
	setTitle( QObject::tr( "Edit Story Board" ) );
}

//============================================================================
ActivityStoryBoard::~ActivityStoryBoard()
{
	g_poStoryBoardActivity = NULL;
}

//============================================================================
//=== methods ===//
//============================================================================

//============================================================================
//! set title of popup menu
void ActivityStoryBoard::setTitle( QString strTitle )
{
	ui.m_TitleBarWidget->setTitleBarText(strTitle);
}

//============================================================================
//! clicked the upper right x button
void ActivityStoryBoard::slotHomeButtonClicked( void )
{
	if( ui.StoryEditWidget->maybeSave() )
	{
		VxFileUtil::setCurrentWorkingDirectory( m_strSavedCwd.c_str() );
		accept();
	}
}

//============================================================================
void ActivityStoryBoard::slotStoryBoardSavedModified( void )
{
	m_FromGui.fromGuiUserModifiedStoryboard();
}
