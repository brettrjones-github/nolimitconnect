#pragma once
//============================================================================
// Copyright (C) 2016 Brett R. Jones
// Issued to MIT style license by Brett R. Jones in 2017
//
// You may use, copy, modify, merge, publish, distribute, sub-license, and/or sell this software
// provided this Copyright is not modified or removed and is included all copies or substantial portions of the Software
//
// This code is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// brett.jones@engineer.com
// http://www.gotvptop.net
//============================================================================

#include <PktLib/VxCommon.h>

#include <QWidget>

class AppCommon;
class QFrame;
class VxPushButton;
class QLabel;
class TodGameWidget;
class VidWidget;
class EngineSettings;
class P2PEngine;

class TextChatLogic : public QWidget
{
	Q_OBJECT

public:
	TextChatLogic( AppCommon& myApp, P2PEngine& engine, EPluginType pluginType, QWidget * parent );

	void						setGuiWidgets(	VxNetIdent *	hisIdent );

signals:
	void						signalUserWentOffline( void );

protected slots:

protected:
	void						handleUserWentOffline( void );

	AppCommon& 				m_MyApp;
	P2PEngine&					m_Engine; 
	EPluginType					m_ePluginType;
	VxNetIdent *				m_HisIdent;
};
