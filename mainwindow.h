#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "model.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    void testMerge();

public:
    MainWindow(Model& model,QWidget *parent = nullptr);
    ~MainWindow();

    //Test by Andy Tran
    void test();
    void testFunction1();
private:
    Ui::MainWindow *ui;
    int num;
};
#endif // MAINWINDOW_H
