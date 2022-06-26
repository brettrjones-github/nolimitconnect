#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <errno.h>
#include "VxFileUtil.h"
#include "VxDebug.h"

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent)
, ui(new Ui::MainWindow)
, m_PeakTimer( new QTimer( this ) )
{
    ui->setupUi(this);
    m_PeakTimer->setInterval( 1000 );
    m_SoundTestLogic = new SoundTestLogic( ui->m_WaveForm, this );

    ui->m_AudioInPeakProgressBar->setRange( 0, 100 );
    ui->m_AudioInPeakProgressBar->setValue( 0 );
    ui->m_AudioOutPeakProgressBar->setRange( 0, 100 );
    ui->m_AudioOutPeakProgressBar->setValue( 0 );

    ui->m_VoipVolumeSlider->setRange( 0, 100 );
    ui->m_VoipVolumeSlider->setValue( 100 );
    ui->m_VoipVolumeSlider->setSingleStep( 10 );
    ui->m_SpeakerVolumeSlider->setRange( 0, 100 );
    ui->m_SpeakerVolumeSlider->setValue( 100 );
    ui->m_SpeakerVolumeSlider->setSingleStep( 10 );
    ui->m_MicrophoneVolumeSlider->setRange( 0, 100 );
    ui->m_MicrophoneVolumeSlider->setValue( 100 );
    ui->m_MicrophoneVolumeSlider->setSingleStep( 10 );

    connect( ui->m_PauseVoipCheckBox, SIGNAL( stateChanged( int ) ), m_SoundTestLogic, SLOT( pauseVoipState( int ) ) );
    connect( ui->m_PauseKodiCheckbox, SIGNAL( stateChanged( int ) ), m_SoundTestLogic, SLOT( pauseKodiState( int) ) );
    connect( ui->m_MuteSpeakersCheckBox, SIGNAL( stateChanged( int ) ), m_SoundTestLogic, SLOT( muteSpeakerState( int ) ) );
    connect( ui->m_MuteMicrophoneCheckBox, SIGNAL( stateChanged( int ) ), m_SoundTestLogic, SLOT( muteMicrophoneState( int ) ) );

    connect( m_PeakTimer, SIGNAL(timeout()), this, SLOT( slotPeakTimerExpired() ) );

    connect( ui->m_MicrophoneVolumeSlider, SIGNAL(valueChanged(int)), this, SLOT( slotMicrophoneVolumeChanged(int) ) );
    connect( ui->m_SpeakerVolumeSlider, SIGNAL(valueChanged(int)), this, SLOT( slotSpeakerVolumeChanged( int ) ) );
    connect( ui->m_VoipVolumeSlider, SIGNAL(valueChanged(int)), this, SLOT( slotVoipVolumeChanged( int ) ) );

    ui->m_PauseVoipCheckBox->setChecked( true );
    ui->m_PauseKodiCheckbox->setChecked( true );
    ui->m_PauseKodiCheckbox->setVisible( false );
    ui->m_MuteMicrophoneCheckBox->setChecked( true );

    m_PeakTimer->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotPeakTimerExpired( void )
{
    ui->m_AudioInPeakProgressBar->setValue( m_SoundTestLogic->getAudioInPeakAmplitude() );
    ui->m_AudioOutPeakProgressBar->setValue( m_SoundTestLogic->getAudioOutPeakAmplitude() );
}

void MainWindow::slotMicrophoneVolumeChanged( int volume )
{
    m_SoundTestLogic->microphoneVolumeChanged( volume );
}

void MainWindow::slotSpeakerVolumeChanged( int volume )
{
    m_SoundTestLogic->speakerVolumeChanged( volume );
}

void MainWindow::slotVoipVolumeChanged( int volume )
{
    m_SoundTestLogic->voipVolumeChanged( volume );
}