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

#include <CoreLib/VxDefs.h>

#include <QString>
#include <QDialog>
#include "ui_ActivityFileSearch.h"

class AppCommon;
class P2PEngine;
class VxNetIdent;

class ActivityFileSearch : public ActivityBase
{
	Q_OBJECT
public:
	ActivityFileSearch(	AppCommon& app, QWidget* parent = NULL );
	virtual ~ActivityFileSearch() override;

    // overrides required for dialogs with there own title bar and bottom bar widgets
    virtual TitleBarWidget *	getTitleBarWidget( void ) override { return ui.m_TitleBarWidget; }
    virtual BottomBarWidget *	getBottomBarWidget( void ) override { return ui.m_BottomBarWidget; }

	void						setTitle( QString strTitle );
	void						addFile(	VxNetIdent *	netIdent, 
											uint8_t				u8FileType, 
											uint64_t				u64FileLen, 
											const char *	pFileName );

private slots:
	//! user clicked the upper right x button
    void						slotHomeButtonClicked( void ) override;
	//! user selected menu item
	void						slotItemClicked( QListWidgetItem * item );

protected:
	//=== vars ===//
	Ui::FileSearchDialog		ui;
};

extern ActivityFileSearch * g_poFileSearchActivity;
