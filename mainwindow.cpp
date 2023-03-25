#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
using std::cout;
using std::endl;

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

void MainWindow::test() {
    "remote changes"
}

