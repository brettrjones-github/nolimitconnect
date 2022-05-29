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

#include "AppletPeerBase.h"

#include <PktLib/VxCommon.h>

#include "GuiUser.h"
#include "ActivityBase.h"
#include "ui_AppletPeerSessionFileOffer.h"

class GuiOfferSession;

class AppletPeerSessionFileOffer : public AppletPeerBase
{
	Q_OBJECT

public:
	AppletPeerSessionFileOffer(	AppCommon& app, 
								QWidget * parent = NULL );
	virtual ~AppletPeerSessionFileOffer() override = default;

public slots:
	void						onAcceptButClick();
	void						onCancelButClick(); 

protected:
	//! Set plugin icon based on permission level
	void						setPluginIcon( EPluginType ePluginType, EFriendState ePluginPermission );

	//=== vars ===//
	Ui::AppletPeerSessionFileOfferUi	ui;
	EPluginType					m_ePluginType;
};
