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

#include "GuiUser.h"
#include "AppletPeerBase.h"
#include "ui_ActivityDownloadItemMenu.h"

class AppCommon;
class P2PEngine;
class GuiFileXferSession;

class ActivityDownloadItemMenu : public ActivityBase
{
	Q_OBJECT

public:
	ActivityDownloadItemMenu(	AppCommon& app, 
								GuiFileXferSession* poSession, 
								QWidget* parent = nullptr );
	virtual ~ActivityDownloadItemMenu() override = default;

public slots:
	void						onCancelButClick(); 

protected:
	//! Set plugin icon based on permission level
	void						setPluginIcon( EPluginType ePluginType, EFriendState ePluginPermission );

	//=== vars ===//
	Ui::CancelDownloadDialog	ui;
	EPluginType					m_ePluginType;
	GuiFileXferSession*		m_Session;
	VxNetIdent*				m_Ident;
};

