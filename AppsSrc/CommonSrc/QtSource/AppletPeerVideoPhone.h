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

#include "ui_AppletPeerVideoPhone.h"

class P2PEngine;
class GuiOfferSession;

class AppletPeerVideoPhone : public AppletPeerBase
{
	Q_OBJECT

public:
	AppletPeerVideoPhone( AppCommon& app, QWidget* parent = nullptr );
	virtual ~AppletPeerVideoPhone() override = default;

	//! called by base class with in session state
    virtual void				onInSession( bool isInSession ) override;

protected:
	// override of ToGuiActivityInterface
    virtual void				toToGuiRxedPluginOffer( GuiOfferSession * offer ) override;
    virtual void				toToGuiRxedOfferReply( GuiOfferSession * offer ) override;

	void						setupActivityVideoPhone();
    void						toGuiInstMsg( GuiUser * friendIdent, EPluginType ePluginType, QString instMsg ) override;

	//=== vars ===//
	Ui::AppletPeerVideoPhoneUi	ui;
};

