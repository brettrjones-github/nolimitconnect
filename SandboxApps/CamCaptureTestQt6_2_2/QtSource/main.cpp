

#include "CamCaptureTest.h"

#include <QtWidgets>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    CamCaptureTest camTest;
    camTest.show();

    return app.exec();
};
