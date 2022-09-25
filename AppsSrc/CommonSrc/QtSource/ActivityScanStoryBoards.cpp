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

#include <QWidget> // must be declared first or Qt 6.2.4 will error in qmetatype.h 2167:23: array subscript value ‘53’ is outside the bounds

#include "ActivityScanStoryBoards.h"
#include "MyIcons.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <PktLib/VxSearchDefs.h>

#define SESSION_TIMEOUT 15000

ActivityScanStoryBoards * g_poScanStoryBoardsActivity = NULL;

//============================================================================
//! constructor
ActivityScanStoryBoards::ActivityScanStoryBoards(	AppCommon& app, 
													QWidget *		parent )
	: ActivityBase( OBJNAME_ACTIVITY_SCAN_STORYBOARDS, app, parent, eAppletScanStoryboard, true ) //Qt::Popup ) //
	, m_bAutoScan(true)
	, m_eScanType(eScanTypeCamServer)
{
	m_SessionTimer = new QTimer( this );
	m_SessionTimer->setInterval( SESSION_TIMEOUT );

	ui.setupUi(this);
	ui.m_TitleBarWidget->setTitleBarText( QObject::tr("Story Board" ) );

    connectBarWidgets();

    connect( this, SIGNAL(signalScanStoryBoard( VxNetIdent *, QString )), this, SLOT(slotScanStoryBoard( VxNetIdent *, QString )));
    connect(ui.m_TitleBarWidget, SIGNAL(signalBackButtonClicked()), this, SLOT(slotHomeButtonClicked()));
    connect(ui.NextStoryBoardButton, SIGNAL(clicked()), this, SLOT(onNextButtonClicked()));

	ui.FriendIdentWidget->setVisible( false );

	g_poScanStoryBoardsActivity = this;
	setTitle("Scan Story Boards" );

	m_FromGui.fromGuiStartScan( m_eScanType, RC_FLAG_HAS_STORY_BOARD, 0, "" );
}

//============================================================================
ActivityScanStoryBoards::~ActivityScanStoryBoards()
{
	g_poScanStoryBoardsActivity = NULL;
}

//============================================================================
void ActivityScanStoryBoards::setTitle( QString strTitle )
{
	ui.m_TitleBarWidget->setTitleBarText(strTitle);
}

//============================================================================
void ActivityScanStoryBoards::setupIdentWidget( VxNetIdent* netIdent )
{
	ui.FriendIdentWidget->updateIdentity( netIdent );
}

//============================================================================
void ActivityScanStoryBoards::newStoryBoardSession( VxNetIdent* netIdent, const char * pStory )
{
	QString strStory = pStory;
	emit signalScanStoryBoard( netIdent, pStory );
}

//============================================================================
void ActivityScanStoryBoards::slotScanStoryBoard( VxNetIdent* netIdent, QString strStory )
{
	ui.FriendIdentWidget->setVisible( true );
	setupIdentWidget( netIdent );
}

//============================================================================
//! clicked the upper right x button
void ActivityScanStoryBoards::slotHomeButtonClicked( void )
{
	closeApplet();
}

//============================================================================
void ActivityScanStoryBoards::onNextButtonClicked( void )
{
	m_FromGui.fromGuiNextScan( m_eScanType );
}

