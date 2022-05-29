#pragma once
//============================================================================
// Copyright (C) 2017 Brett R. Jones 
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

#include "AppDefs.h"
#include "VxWidgetBase.h"
#include "ui_AppletLaunchWidget.h"

class AppCommon;
class AppletLaunchPage;

class AppletLaunchWidget : public VxWidgetBase
{
	Q_OBJECT;
public:
	AppletLaunchWidget( AppCommon& myApp, EApplet appletType,  AppletLaunchPage * parent = 0 );
	virtual ~AppletLaunchWidget() override = default;

	VxPushButton *				getButton( void );

signals:
	void						signalAppletLaunchClicked( EApplet appletType );

public slots:
	void						slotAppletIconPressed( void );

public:
	//=== vars ===//
	Ui::LaunchWidgetClass		ui;
	AppCommon&					m_MyApp;
	EApplet						m_AppletType;
	AppletLaunchPage *			m_AppletLaunchPage;

};

