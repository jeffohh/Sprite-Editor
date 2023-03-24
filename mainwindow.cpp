
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    num = 90;
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow:: testFunction1(){
    int i = 0;
    int j = 0;
}
