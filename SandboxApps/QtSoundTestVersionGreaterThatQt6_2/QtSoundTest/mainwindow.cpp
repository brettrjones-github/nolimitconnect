#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <errno.h>
#include "VxFileUtil.h"
#include "VxDebug.h"

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent)
, ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_SoundTestLogic = new SoundTestLogic( ui->m_WaveForm, this );

    connect( ui->m_PauseVoipCheckBox, SIGNAL( stateChanged( int ) ), m_SoundTestLogic, SLOT( pauseVoipState( int ) ) );
    connect( ui->m_PauseKodiCheckbox, SIGNAL( stateChanged( int ) ), m_SoundTestLogic, SLOT( pauseKodiState( int) ) );
    connect( ui->m_MuteSpeakersCheckBox, SIGNAL( stateChanged( int ) ), m_SoundTestLogic, SLOT( muteSpeakerState( int ) ) );
    connect( ui->m_MuteMicrophoneCheckBox, SIGNAL( stateChanged( int ) ), m_SoundTestLogic, SLOT( muteMicrophoneState( int ) ) );

    ui->m_PauseVoipCheckBox->setChecked( true );
    ui->m_PauseKodiCheckbox->setChecked( true );
    ui->m_PauseKodiCheckbox->setVisible( false );
    ui->m_MuteMicrophoneCheckBox->setChecked( true );
}

MainWindow::~MainWindow()
{
    delete ui;
}
