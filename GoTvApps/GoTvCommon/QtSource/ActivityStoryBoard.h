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
// brett.jones@engineer.com
// http://www.gotvptop.net
//============================================================================

#include "ActivityBase.h"

#include <QString>
#include <QDialog>
#include <QTimer>
#include "ui_ActivityStoryBoard.h"
#include "ui_StoryWidget.h"
#include <GoTvInterface/IDefs.h>

class ActivityStoryBoard : public ActivityBase
{
	Q_OBJECT
public:
	ActivityStoryBoard(	AppCommon& app, 
						QWidget *		parent = NULL );
	virtual ~ActivityStoryBoard();

public:
	void						setTitle( QString strTitle );

private slots:
	void						slotHomeButtonClicked( void );
	void						slotStoryBoardSavedModified();

protected:
	//=== vars ===//
	Ui::StoryBoardDialog		ui;
	std::string					m_strSavedCwd;
	std::string					m_strStoryBoardDir;
	std::string					m_strStoryBoardFile;
};

extern ActivityStoryBoard * g_poStoryBoard;
