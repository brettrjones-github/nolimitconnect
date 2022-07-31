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

#include "AppletClientBase.h"
#include "GuiUserUpdateCallback.h"
#include "soundio/AudioDefs.h"
#include "soundio/AudioTestGenerator.h"

#include <GuiInterface/IDefs.h>
#include <ptop_src/ptop_engine_src/PluginSettings/PluginSetting.h>

#include <QString>
#include <QDialog>
#include <QMediaDevices>
#include <QIODevice>
#include <QTimer>

#include "ui_AppletSoundSettings.h"

class VxNetIdent;
class GuiHostSession;

class AppletSoundSettings : public AppletClientBase
{
	Q_OBJECT
public:
	AppletSoundSettings( AppCommon& app, QWidget* parent = nullptr );
	virtual ~AppletSoundSettings() override;

    void                        infoMsg( const char * infoMsg, ... );
    void                        toGuiInfoMsg( char * logMsg );

    void                        statusMsg( const char* errMsg, ... );
    void						setStatusLabel( QString strMsg );

signals:
    void                        signalInfoMsg( QString& infoStr );

protected slots:
    void                        inDeviceChanged( int index );
    void                        updateInAudioDevices( void );

    void                        outDeviceChanged( int index );
    void                        updateOutAudioDevices( void );

    void                        slotApplyInDeviceChange( void );
    void                        slotApplyOutDeviceChange( void );
    void                        slotPeakTimerTimeout( void );

    void                        slotStartTestSoundDelay( void );
    void                        slotTestedSoundDelayResult( int echoDelayMs );
    void                        slotEchoCancelEnableChange( int checkState );
    void                        slotAudioTestState( EAudioTestState audioTestState );

protected:
    void                        showEvent( QShowEvent* ev ) override;
    void                        hideEvent( QHideEvent* ev ) override;

    //=== vars ===//
    Ui::AppletSoundSettingsUi   ui;
    QTimer*                     m_PeakTimer{ nullptr };

    QMediaDevices*              m_devices = nullptr;
    QScopedPointer<AudioTestGenerator>   m_Test8000HzMono200HzToneGenerator;
};
