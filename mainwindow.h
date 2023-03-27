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

public slots:
    void updateCanvas(QImage*); // jeff!!

private:
    Ui::MainWindow *ui;
    int num;

    //Andy Tran
    vector<QImage> frameList;
    QGraphicsScene *scene;
    //------------------------

    //TZhou
    QColor* currentColor; // The current color of the pen, default black.
    void setCurrentColorBtnTo(QColor* newColor);
    //----------------------

private slots:
    //TZhou: color picker area
    void on_changeColorBtn_clicked();
    void on_alphaSlider_valueChanged(int value);
    //-------------------------------------------

};
#endif // MAINWINDOW_H
