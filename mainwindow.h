#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "model.h"

//Added by Andy Tran
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QImage>
#include <QPixmap>
#include <QVector>
#include <QTimer>
using std::vector;
//----------------

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Model& model,QWidget *parent = nullptr);
    ~MainWindow();

    //Andy Tran
    void previewAnimation();
    //----------------------

private:
    Ui::MainWindow *ui;
    int num;

    //Andy Tran
    vector<QImage> frameList;
    QGraphicsScene *scene;
    //------------------------
};
#endif // MAINWINDOW_H
