#include "mainwindow.h"
#include <QApplication>

#if defined(TARGET_OS_WINDOWS) && defined(_MSC_VER) 
// removes the popup console window on windows os
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif // TARGET_OS_WINDOWS

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
