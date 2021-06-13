#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qttestsharedlib3.h"
#include "qtteststaticlib3.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QtTestSharedLib sharedLib;
    QtTestStaticLib staticLib;


    char textBuf[16000];
    sprintf( textBuf, "sizeof void * %d\n, int %d\n long %d\n longlong %d\n float %d\n double %d shared value %zd static value %d\n",
             (int)sizeof(void *), (int)sizeof(int), (int)sizeof(long), (int)sizeof(long long), (int)sizeof(float), (int)sizeof(double),
             staticLib.getStaticValue(),
             sharedLib.getSharedValue()
             );
    ui->labelTextOut->setText(textBuf);
}

MainWindow::~MainWindow()
{
    delete ui;
}
