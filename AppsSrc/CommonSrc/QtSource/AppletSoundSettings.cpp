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

#include "AppletSoundSettings.h"
#include "ActivityInformation.h"

#include "AppCommon.h"
#include "AppGlobals.h"
#include "AppSettings.h"

#include "ActivityMessageBox.h"
#include "GuiHostSession.h"
#include "GuiParams.h"
#include "GuiHelpers.h"
#include "SoundMgr.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

#include <CoreLib/VxGlobals.h>

#include <QAudioSink>
#include <QByteArray>
#include <QComboBox>
#include <QIODevice>
#include <QLabel>
#include <QMainWindow>
#include <QObject>
#include <QPushButton>
#include <QSlider>
#include <QTimer>
#include <QScopedPointer>
#include <QMediaDevices>

namespace
{
    const int MAX_INFO_MSG_SIZE = 2048;
}

//============================================================================
AppletSoundSettings::AppletSoundSettings( AppCommon& app, QWidget*	parent )
: AppletClientBase( OBJNAME_APPLET_SOUND_SETTINGS, app, parent )
, m_devices( new QMediaDevices( this ) )
, m_PeakTimer( new QTimer( this ) )
{
    setAppletType( eAppletSoundSettings );
    ui.setupUi( getContentItemsFrame() );
    setTitleBarText( DescribeApplet( m_EAppletType ) );

    ui.m_AudioInPeakProgressBar->setValue( 0 );
    ui.m_AudioOutPeakProgressBar->setValue( 0 );
    ui.m_PauseMicrophoneCheckBox->setChecked( false );
    ui.m_PauseVoipCheckBox->setChecked( true );
    m_PeakTimer->setInterval( 200 );

    connectBarWidgets();

    QAudioFormat mixerFormat;
    mixerFormat.setSampleRate( 8000 );
    mixerFormat.setChannelCount( 1 );
    mixerFormat.setSampleFormat( QAudioFormat::Int16 );

    int toneSampleRateHz = 200;
    double fps = 1.0 / (double)toneSampleRateHz;
    int durationMicroSeconds = fps * 1000000;
    m_Test8000HzMono200HzToneGenerator.reset( new AudioTestGenerator( mixerFormat, durationMicroSeconds, toneSampleRateHz ) );

    updateInAudioDevices();

    int soundInDeviceIndex = 0;
    m_MyApp.getSoundMgr().getSoundInDeviceIndex( soundInDeviceIndex );
    ui.m_InDeviceComboBox->setCurrentIndex( soundInDeviceIndex );

    connect( ui.m_InDeviceComboBox, QOverload<int>::of( &QComboBox::activated ), this, &AppletSoundSettings::inDeviceChanged );
    connect( m_devices, &QMediaDevices::audioInputsChanged, this, &AppletSoundSettings::updateInAudioDevices );
    connect( ui.m_ApplyDefaultInDeviceButton, SIGNAL( clicked() ), this, SLOT( slotApplyInDeviceChange() ) );
    
    updateOutAudioDevices();

    int soundOutDeviceIndex = 0;
    m_MyApp.getSoundMgr().getSoundOutDeviceIndex( soundOutDeviceIndex );
    ui.m_OutDeviceComboBox->setCurrentIndex( soundOutDeviceIndex );

    connect( ui.m_OutDeviceComboBox, QOverload<int>::of( &QComboBox::activated ), this, &AppletSoundSettings::outDeviceChanged );
    connect( m_devices, &QMediaDevices::audioOutputsChanged, this, &AppletSoundSettings::updateOutAudioDevices );
    connect( ui.m_ApplyDefaultOutDeviceButton, SIGNAL(clicked()), this, SLOT(slotApplyOutDeviceChange()) );

    m_MyApp.activityStateChange( this, true );

    if( !m_MyApp.getFromGuiInterface().fromGuiPushToTalk( m_MyApp.getMyOnlineId(), true ) )
    {
        m_MyApp.getSoundMgr().playSnd( eSndDefBusy );
        QMessageBox::information( this, QObject::tr( "Push To Talk" ), QObject::tr( "Sound settings Push To Talk failed to enable" ), QMessageBox::Ok );
    }

    connect( m_PeakTimer, SIGNAL( timeout() ), this, SLOT( slotPeakTimerTimeout() ) );
    m_PeakTimer->start();

    ui.m_PauseVoipCheckBox->setVisible( false );
}

//============================================================================
AppletSoundSettings::~AppletSoundSettings()
{
    m_PeakTimer->stop();
    m_MyApp.activityStateChange( this, false );
    m_MyApp.getFromGuiInterface().fromGuiPushToTalk( m_MyApp.getMyOnlineId(), false );
}

//============================================================================
void AppletSoundSettings::setStatusLabel( QString strMsg )
{
    ui.m_StatusMsgLabel->setText( strMsg );
}

//============================================================================
void AppletSoundSettings::showEvent( QShowEvent * ev )
{
    ActivityBase::showEvent( ev );
    m_MyApp.wantToGuiActivityCallbacks( this, true );
}

//============================================================================
void AppletSoundSettings::hideEvent( QHideEvent * ev )
{
    m_MyApp.wantToGuiActivityCallbacks( this, false );
    ActivityBase::hideEvent( ev );
}

//============================================================================
void AppletSoundSettings::toGuiInfoMsg( char * infoMsg )
{
    QString infoStr( infoMsg );

    static QRegularExpression removeExpress( "[\\n\\r]" );
    infoStr.remove( removeExpress );

    emit signalInfoMsg( infoStr );
}

//============================================================================
void AppletSoundSettings::infoMsg( const char* errMsg, ... )
{
    char as8Buf[ MAX_INFO_MSG_SIZE ];
    va_list argList;
    va_start( argList, errMsg );
    vsnprintf( as8Buf, sizeof( as8Buf ), errMsg, argList );
    as8Buf[ sizeof( as8Buf ) - 1 ] = 0;
    va_end( argList );

    toGuiInfoMsg( as8Buf );
}

//============================================================================
void AppletSoundSettings::inDeviceChanged( int index )
{
    //m_generator->stop();
    //m_audioOutput->stop();
    //m_audioOutput->disconnect( this );
    //initializeAudio( m_deviceBox->itemData( index ).value<QAudioDevice>() );

    m_MyApp.getSoundMgr().soundInDeviceChanged( index );
}

//============================================================================
void AppletSoundSettings::updateInAudioDevices( void )
{
    ui.m_InDeviceComboBox->clear();
    std::vector< std::pair<QString, QAudioDevice> > inDeviceList;
    m_MyApp.getSoundMgr().getSoundInDevices( inDeviceList );

    for( auto& devicePair : inDeviceList )
    {
        ui.m_InDeviceComboBox->addItem( devicePair.first, QVariant::fromValue( devicePair.second ) );
    }
}

//============================================================================
void AppletSoundSettings::outDeviceChanged( int index )
{
    //m_generator->stop();
    //m_audioOutput->stop();
    //m_audioOutput->disconnect( this );
    //initializeAudio( m_deviceBox->itemData( index ).value<QAudioDevice>() );

    m_MyApp.getSoundMgr().soundOutDeviceChanged( index );
}

//============================================================================
void AppletSoundSettings::updateOutAudioDevices( void )
{
    ui.m_OutDeviceComboBox->clear();
    std::vector< std::pair<QString, QAudioDevice> > outDeviceList;
    m_MyApp.getSoundMgr().getSoundOutDevices( outDeviceList );

    for( auto& devicePair : outDeviceList )
    {
        ui.m_OutDeviceComboBox->addItem( devicePair.first, QVariant::fromValue( devicePair.second ) );
    }
}

//============================================================================
void AppletSoundSettings::slotApplyInDeviceChange( void )
{
    QString sndInDevDescription = ui.m_InDeviceComboBox->currentText();
    if( !sndInDevDescription.isEmpty() )
    {
        if( m_MyApp.getSoundMgr().setSoundInDeviceIndex( ui.m_InDeviceComboBox->currentIndex() ) )
        {
            
            QMessageBox::information( this, QObject::tr( "Sound In Device" ), sndInDevDescription + QObject::tr( " device is saved as preferred Sound In Device" ), QMessageBox::Ok );
        }
        else
        {
            QMessageBox::information( this, QObject::tr( "Sound In Device" ), sndInDevDescription + QObject::tr( " failed to initialize" ), QMessageBox::Ok );
        }
    }
    else
    {
        QMessageBox::information( this, QObject::tr( "Sound In Device" ), QObject::tr( "No Sound In Device Is Available" ), QMessageBox::Ok );
    }
}

//============================================================================
void AppletSoundSettings::slotApplyOutDeviceChange( void )
{
    QString sndOutDevDescription = ui.m_OutDeviceComboBox->currentText();
    if( !sndOutDevDescription.isEmpty() )
    {
        if( m_MyApp.getSoundMgr().setSoundOutDeviceIndex( ui.m_OutDeviceComboBox->currentIndex() ) )
        {
            m_MyApp.getAppSettings().setSoundOutDeviceIndex( ui.m_OutDeviceComboBox->currentIndex() );
            QMessageBox::information( this, QObject::tr( "Sound Out Device" ), sndOutDevDescription + QObject::tr( " device is saved as preferred Sound Out Device" ), QMessageBox::Ok );
        }
        else
        {
            QMessageBox::information( this, QObject::tr( "Sound Out Device" ), sndOutDevDescription + QObject::tr( " failed to initialize" ), QMessageBox::Ok );
        }
    }
    else
    {
        QMessageBox::information( this, QObject::tr( "Sound Out Device" ), QObject::tr( "No Sound Out Device Is Available" ), QMessageBox::Ok );
    }
}

//============================================================================
void AppletSoundSettings::slotPeakTimerTimeout( void )
{
    ui.m_AudioInPeakProgressBar->setValue( m_MyApp.getSoundMgr().getAudioInPeakAmplitude() );
    ui.m_AudioOutPeakProgressBar->setValue( m_MyApp.getSoundMgr().getAudioOutPeakAmplitude() );
}