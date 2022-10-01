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

#include <QDialog>
#include "GuiUser.h"
#include "ui_AppletPeerReplyOfferFile.h"

class AppCommon;
class P2PEngine;
class GuiOfferSession;

class AppletPeerReplyFileOffer : public AppletPeerBase
{
	Q_OBJECT
public:
	AppletPeerReplyFileOffer( AppCommon& app, QWidget* parent = nullptr );
	virtual ~AppletPeerReplyFileOffer() override = default;

public slots:
	void						onReceiveFileButClick();
	void						onCancelButClick(); 

protected:
	//! Set plugin icon based on permission level
	void						setPluginIcon( EPluginType ePluginType, EFriendState ePluginPermission );

	//=== vars ===//
	Ui::AppletPeerReplyFileOfferUi	ui;
    GuiOfferSession *			m_Offer{ nullptr };
};
