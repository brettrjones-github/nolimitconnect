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

#include <PktLib/VxCommon.h>

#include <QDialog>
#include "ui_ActivityCreateAccount.h"

class AppCommon;
class P2PEngine;

class ActivityCreateAccount : public ActivityBase
{
	Q_OBJECT
public:
    ActivityCreateAccount( AppCommon& app, QWidget* parent = NULL );
	virtual ~ActivityCreateAccount() override = default;

    // overrides required for dialogs with there own title bar and bottom bar widgets
    virtual TitleBarWidget *	getTitleBarWidget( void ) override { return ui.m_TitleBarWidget; }
    virtual BottomBarWidget *	getBottomBarWidget( void ) override { return ui.m_BottomBarWidget; }

	void						setRootUserDataDirectory( const char* userDir ) { m_strRootUserDataDir = userDir; };
	//! validate user input
	bool						accountValidate( void );
    bool						wasLoginNameEntered( void );

	virtual void				onBackButtonClicked( void ) override;

private slots:
	//! login was clicked
	void						slotButtonLoginClicked( void );

protected:
	//=== vars ===//
	Ui::CreateAccountClass		ui;
	VxNetIdent					m_UserAccount;
	std::string					m_strRootUserDataDir;
};


