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

#include "config_apps.h"
#include "ui_SoundCtrlWidget.h"

#include <PktLib/VxCommon.h>

#include <QWidget>

class AppCommon;
class P2PEngine;
class VxNetIdent;
class MySndMgr;

class SoundCtrlWidget : public QWidget
{
	Q_OBJECT

public:
	SoundCtrlWidget( QWidget *parent = 0 );

	AppCommon&					getMyApp( void ) { return m_MyApp; }
	MyIcons&					getMyIcons( void );

	void						startup(	AppCommon&		app,
											EPluginType			ePluginType,
											VxNetIdent *		netIdent );

	void						setMicMute( bool mute );
	void						setSpeakerMute( bool mute );
	void						setSoundChannelId( VxGUID oChannelId );

signals:
	void						signalSoundLevel( int iSoundLevel );
	void						signalMicIsMuted( bool muted );
	void						signalSpeakerIsMuted( bool muted );

public slots:
	void						slotSoundPcmData( QByteArray audioData );
	void						slotMuteMicButtonClicked();
	void						slotMuteSpeakerButtonClicked();

private:
	void						setSoundLabel( QString strText );
	void						initSndCtrls( MySndMgr& sndMgr );

	Ui::SoundCtrlWidget			ui;
	AppCommon&					m_MyApp;
	MySndMgr&					m_SndMgr;
	bool						m_SpeakerIsMuted;
	bool						m_MicIsMuted;
	VxGUID						m_ChannelId;
};
