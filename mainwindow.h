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

//TZhou
#include<QColorDialog>
#include<QColor>
//-------------------


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

private slots:
    //Tzhou
    void on_changeColorBtn_clicked();
    //-----------------------

private:
    Ui::MainWindow *ui;
    int num;

    //Andy Tran
    vector<QImage> frameList;
    QGraphicsScene *scene;
    //------------------------

    //TZhou
    QColor* currentColor;
    void setCurrentColorBtnTo(QColor* newColor); //helper method
    //----------------------
};
#endif // MAINWINDOW_H
