#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "AppLogic.h"

#include <CoreLib/VxDebug.h>
#include <CoreLib/VxGlobals.h>

#include <QApplication>
#include <QDesktopWidget>
#include <QScreen>

//============================================================================
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_AppLogic( GetAppLogic() )
{
    ui->setupUi(this);

#if defined(NET_CLIENT_TEST)
    VxSetApplicationNameNoSpaces( "NetClientTest" );
    this->setWindowTitle( "Net Client Test" );
#endif

#if defined(NET_LISTEN_TEST)
    VxSetApplicationNameNoSpaces( "NetListenTest" );
    this->setWindowTitle( "Net Listen Test" );
#endif

    restoreHomeWindowGeometry();
}

//============================================================================
MainWindow::~MainWindow()
{
    delete ui;
}


//============================================================================
void MainWindow::closeEvent( QCloseEvent *event )
{
    saveHomeWindowGeometry();
    QMainWindow::closeEvent( event );
}

//============================================================================
void MainWindow::restoreHomeWindowGeometry()
{
#if !defined(TARGET_OS_ANDROID)
    QByteArray restoreGeom = m_AppLogic.getAppSettings().value( "mainWindowGeometry" ).toByteArray();
    if( restoreGeom.isEmpty() )
    {
        const QRect availableGeometry = QApplication::desktop()->availableGeometry( this );
        resize( availableGeometry.width() / 3, availableGeometry.height() / 2 );
        move( ( availableGeometry.width() - width() ) / 2,
            ( availableGeometry.height() - height() ) / 2 );
    }
    else
    {
        restoreGeometry( restoreGeom );
    }
#else
    updateAndroidGeomety();
#endif // !defined(TARGET_OS_ANDROID)}
}

//============================================================================
void MainWindow::saveHomeWindowGeometry()
{
#if !defined(TARGET_OS_ANDROID)
    if( !QWidget::isMaximized() && !QWidget::isMinimized() )
    {
        QByteArray saveGeom = saveGeometry();
        m_AppLogic.getAppSettings().setValue( "mainWindowGeometry", saveGeom );
    }

    //settings.setValue( "mainWindowState", saveState() );
#endif // !defined(TARGET_OS_ANDROID)
}

//============================================================================
void MainWindow::updateAndroidGeomety()
{
#if defined(TARGET_OS_ANDROID)
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect  screenGeometry = screen->availableGeometry();
    int height = screenGeometry.height() - 20;
    int width = screenGeometry.width() - 20;
    resize( width, height );
    move( screenGeometry.left() + 10, screenGeometry.top() + 10 );
    LogModule( eLogWindowPositions, LOG_VERBOSE, "MainWindow Screen Size %d %d", width, height );
#endif // defined(TARGET_OS_ANDROID)
}
