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
// http://www.nolimitconnect.com
//============================================================================

#include "AppletPeerBase.h"

#include <PktLib/VxCommon.h>
#include <QDialog>
#include "ui_AppletPeerChangeFriendship.h"

class GuiUser;

class AppletPeerChangeFriendship : public AppletPeerBase
{
	Q_OBJECT
public:

	AppletPeerChangeFriendship( AppCommon& app, QWidget * parent = NULL );
	virtual ~AppletPeerChangeFriendship() override = default;

	//=== helpers ===//
	//! set friend to change your permission to him   
	void						setFriend( GuiUser * poFriend );
	//! get current permission selection
	EFriendState				getPermissionSelection( void );
	//! set which radio button is checked
	void						setCheckedPermission( EFriendState eMyFriendshipToHim );

public slots:
	void						onOkButClick( void );
	void						onCancelButClick( void ); 
	void						onPermissionClick( void );
	void						onMakeFriendButClick( void );
	void						onIgnoreButClick( void );

protected:
	void						setHisPermissionToMe( EFriendState hisFriendshipToMe );
	void						setMyPermissionToHim( EFriendState myFriendshipToHim );

	//=== vars ===//
	Ui::AppletPeerChangeFriendshipUi	ui;
    GuiUser *				    m_Friend;					
};