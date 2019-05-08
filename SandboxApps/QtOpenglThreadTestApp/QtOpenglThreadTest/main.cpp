#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication::setAttribute( Qt::AA_UseOpenGLES ); // force use of gles so can use gles shaders
    QApplication::setAttribute( Qt::AA_ShareOpenGLContexts );

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
