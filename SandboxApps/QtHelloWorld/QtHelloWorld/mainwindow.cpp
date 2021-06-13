#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    char textBuf[16000];
    sprintf( textBuf, "sizeof void * %d\n int %d\n long %d\n longlong %d\n float %d\n double %d\n",
             (int)sizeof(void *), (int)sizeof(int), (int)sizeof(long), (int)sizeof(long long), (int)sizeof(float), (int)sizeof(double)
             );
    ui->labelTextOut->setText(textBuf);
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(slotButtonClicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotButtonClicked()
{
    ui->labelTextOut->setText("Button Was Pressed");
}
