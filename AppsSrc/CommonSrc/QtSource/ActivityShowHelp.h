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

#include "ui_ActivityShowHelp.h"
#include "ActivityShowHelp.h"

#include <CoreLib/VxDefs.h>

class AppCommon;
class P2PEngine;

class ActivityShowHelp : public ActivityBase
{
	Q_OBJECT

public:
	ActivityShowHelp( AppCommon& app, QWidget* parent = nullptr );
	virtual ~ActivityShowHelp() override = default;

    // overrides required for dialogs with there own title bar and bottom bar widgets
    virtual TitleBarWidget *	getTitleBarWidget( void ) override { return ui.m_TitleBarWidget; }
    virtual BottomBarWidget *	getBottomBarWidget( void ) override { return ui.m_BottomBarWidget; }

protected slots:
	void						slotReject();
	void						slotClipboard();

protected:
	void						loadHelpFile( const char* pFileName, uint64_t u64FileLen );
	void						loadHelp( void );


	//=== vars ===//
	Ui::ShowHelpDialog			ui;
};
