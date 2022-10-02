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
#include "TodGameLogic.h"

#include "ui_AppletPeerTodGame.h"

class GuiOfferSession;

class AppletPeerTodGame : public AppletPeerBase
{
	Q_OBJECT

public:
	AppletPeerTodGame( AppCommon& app, QWidget* parent = nullptr );
	virtual ~AppletPeerTodGame() override = default;

	//! called by base class with in session state
    virtual void				onInSession( bool isInSession ) override;
	//! called after session end or dialog exit
	virtual void				onEndSession( void );

protected:
	// override of ToGuiActivityInterface
    virtual void				toToGuiRxedPluginOffer( GuiOfferSession * offer ) override;
    virtual void				toToGuiRxedOfferReply( GuiOfferSession * offer ) override;

	void						setupAppletPeerTodGame( void );
    void						toGuiInstMsg( GuiUser* friendIdent, EPluginType ePluginType, QString instMsg ) override;

	//=== vars ===//
	Ui::AppletPeerTodGameUi	    ui;
	TodGameLogic				m_TodGameLogic;
};
