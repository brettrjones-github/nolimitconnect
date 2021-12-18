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

#include "ActivityFileSearch.h"
#include "AppCommon.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

ActivityFileSearch * g_poFileSearchActivity = NULL;

//============================================================================
ActivityFileSearch::ActivityFileSearch(	AppCommon& app,  QWidget * parent )
: ActivityBase( OBJNAME_ACTIVITY_FILE_SEARCH, app, parent, eAppletScanSharedFiles, true, false )
{
	ui.setupUi(this);
	ui.m_TitleBarWidget->setTitleBarText( QObject::tr("File Search" ) );

    connectBarWidgets();

    connect(ui.m_TitleBarWidget, SIGNAL(signalBackButtonClicked()), this, SLOT(slotHomeButtonClicked()));
    connect(ui.FileItemList, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(slotItemClicked(QListWidgetItem *)));
    connect(ui.FileItemList, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(slotItemClicked(QListWidgetItem *)));

    g_poFileSearchActivity = this;
	setTitle("FileSearch" );
}

//============================================================================
ActivityFileSearch::~ActivityFileSearch()
{
	g_poFileSearchActivity = NULL;
}

//============================================================================
void ActivityFileSearch::setTitle( QString strTitle )
{
	ui.m_TitleBarWidget->setTitleBarText(strTitle);
}

//============================================================================
//=== signals and slots ===//
//============================================================================

//============================================================================
//! clicked the upper right x button
void ActivityFileSearch::slotHomeButtonClicked( void )
{
	close();
}

//============================================================================
//! user selected menu item
void ActivityFileSearch::slotItemClicked(QListWidgetItem * item)
{
}


