#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "SoundTestLogic.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected slots:
    void                        slotPeakTimerExpired( void );

    void                        slotMicrophoneVolumeChanged( int volume );
    void                        slotSpeakerVolumeChanged( int volume );
    void                        slotVoipVolumeChanged( int volume );

protected:
    Ui::MainWindow*             ui;

    SoundTestLogic*             m_SoundTestLogic{ nullptr };
    QTimer*                     m_PeakTimer{ nullptr };
};

#endif // MAINWINDOW_H
