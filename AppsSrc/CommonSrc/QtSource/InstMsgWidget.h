#pragma once
//============================================================================
// Copyright (C) 2015 Brett R. Jones
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

#include "ui_InstMsgWidget.h"
#include <GuiInterface/IDefs.h>

class P2PEngine;
class AppCommon;
class VxNetIdent;

class InstMsgWidget : public QWidget
{
	Q_OBJECT

public:
	InstMsgWidget( QWidget* parent = 0 );

	void						setInstMsgWidgets( EPluginType ePluginType, GuiUser * hisIdent );
	void						toGuiInstMsg( QString instMsg );

signals:
	void						signalToGuiInstMsg( QString instMsg );

private slots:
	void						slotClearHistoryButtonClicked( void );
	void						slotCancelButtonClicked( void );
	void						slotSendButtonClicked( void );
	void						slotToGuiInstMsg( QString instMsg );

private:
	void						sendChatMsg( QString strChatMsg );
	void						appendHistoryMsg( QString strChatMsg );

	Ui::InstMsgWidgetClass		ui;
	AppCommon&				    m_MyApp;
	P2PEngine&					m_Engine;
	EPluginType					m_ePluginType;
    GuiUser *				    m_HisIdent;
};
