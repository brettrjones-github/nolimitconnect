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
#include "ui_ActivityPermissions.h"

class ActivityPermissions : public ActivityBase
{
	Q_OBJECT
public:
	ActivityPermissions( AppCommon& app, QWidget * parent = NULL );
	virtual ~ActivityPermissions() override = default;

    // overrides required for dialogs with there own title bar and bottom bar widgets
    virtual TitleBarWidget *	getTitleBarWidget( void ) override { return ui.m_TitleBarWidget; }
    virtual BottomBarWidget *	getBottomBarWidget( void ) override { return ui.m_BottomBarWidget; }

	//=== helpers ===//
	//! Set plugin to set permissions on   
    void						setPluginType( EPluginType ePluginType ) override;
	//! Set plugin icon based on permission level
	void						setPluginIcon( EPluginType ePluginType, EFriendState ePluginPermission );
	//! get current permission selection
	EFriendState				getPermissionSelection( void );
	//! set which radio button is checked
	void						setCheckedPermission( EFriendState ePluginPermission );

public slots:

	void						onOkButClick( void );
	void						onCancelButClick( void ); 
	void						onPermissionClick( void );

protected:
	//=== vars ===//
	Ui::PermissionsDialog		ui;
	EPluginType					m_ePluginType;					

};
