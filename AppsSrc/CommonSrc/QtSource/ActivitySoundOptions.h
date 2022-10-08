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

#include "ui_ActivitySoundOptions.h"


class ActivitySoundOptions : public ActivityBase
{
	Q_OBJECT
public:

	ActivitySoundOptions(	AppCommon& app, 
							QWidget*		parent = nullptr );
	virtual ~ActivitySoundOptions() override = default;

    // overrides required for dialogs with there own title bar and bottom bar widgets
    virtual TitleBarWidget *	getTitleBarWidget( void ) override { return ui.m_TitleBarWidget; }
    virtual BottomBarWidget *	getBottomBarWidget( void ) override { return ui.m_BottomBarWidget; }

public slots:
	void						onApplyButClick();
	void						onCancelButClick(); 

protected:
	//=== vars ===//
	Ui::SoundOptionsDialog		ui;
	EPluginType					m_ePluginType;
	VxNetIdent*				m_Friend;

};
