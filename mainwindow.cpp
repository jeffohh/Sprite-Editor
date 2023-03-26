#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "model.h"

#include <iostream>
using std::cout;
using std::endl;

MainWindow::MainWindow(Model& model,QWidget *parent)
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
    //remote changes
    //test pushing by Andy Tran
}

void MainWindow:: testFunction1(){
    int i = 1;
    int j = 1;
}

