#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class AppLogic;
    
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void                        closeEvent( QCloseEvent *event );

    void                        saveHomeWindowGeometry();
    void                        restoreHomeWindowGeometry();
    void                        updateAndroidGeomety();

private:
    Ui::MainWindow *            ui;
    AppLogic&                   m_AppLogic;
};

#endif // MAINWINDOW_H
