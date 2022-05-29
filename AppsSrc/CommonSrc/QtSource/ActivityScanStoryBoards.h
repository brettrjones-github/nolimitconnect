#pragma once
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

#include "ActivityBase.h"

#include <QString>
#include <QDialog>
#include <QTimer>
#include "ui_ActivityScanStoryBoards.h"
#include <GuiInterface/IDefs.h>

class VxNetIdent;

class ActivityScanStoryBoards :  public ActivityBase
{
	Q_OBJECT
public:
	ActivityScanStoryBoards(	AppCommon& app, 
								QWidget *		parent = NULL );
	virtual ~ActivityScanStoryBoards() override;

    // overrides required for dialogs with there own title bar and bottom bar widgets
    virtual TitleBarWidget *	getTitleBarWidget( void ) override { return ui.m_TitleBarWidget; }
    virtual BottomBarWidget *	getBottomBarWidget( void ) override { return ui.m_BottomBarWidget; }

public:
	void						setTitle( QString strTitle );
	void						newStoryBoardSession( VxNetIdent * netIdent, const char * pStory );

signals:
	void						signalScanStoryBoard( VxNetIdent * netIdent, QString strStory );

public slots:
	void						slotScanStoryBoard( VxNetIdent * netIdent, QString strStory );

private slots:
    void						slotHomeButtonClicked( void ) override;
	void						onNextButtonClicked( void );

protected:
	void						setupIdentWidget( VxNetIdent * netIdent );

	//=== vars ===//
	Ui::ScanStoryBoardsDialog	ui;
	bool						m_bAutoScan;
	QTimer *					m_SessionTimer;
	EScanType					m_eScanType;
};

extern ActivityScanStoryBoards * g_poScanStoryBoardsActivity;
