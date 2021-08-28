#include "mainwindow.h"
#include <QApplication>
#include "VxDebug.h"

int main(int argc, char *argv[])
{
    LogMsg( LOG_DEBUG, "Creating QApplication" );
    QApplication a(argc, argv);
    MainWindow w;
    LogMsg( LOG_DEBUG, "Showing Window" );
    w.show();
    LogMsg( LOG_DEBUG, "Running app.exec" );

    return a.exec();
}
