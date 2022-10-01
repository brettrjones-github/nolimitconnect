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

#include <QDialog>
#include <QTimer>
#include "ui_ActivityTimedMessage.h"

class ActivityTimedMessage : public QDialog
{
	Q_OBJECT

public:
	ActivityTimedMessage( QString strTitle, QString strMessage, int iTimeoutSeconds = 4, QWidget* parent = NULL );
	virtual ~ActivityTimedMessage() override = default;

    // overrides required for dialogs with there own title bar and bottom bar widgets
    //virtual TitleBarWidget *	getTitleBarWidget( void ) override { return ui.m_TitleBarWidget; }
    //virtual BottomBarWidget *	getBottomBarWidget( void ) override { return ui.m_BottomBarWidget; }

private:
	Ui::TimedMessageDlg			ui;
	QTimer						m_CloseDlgTimer;

};
