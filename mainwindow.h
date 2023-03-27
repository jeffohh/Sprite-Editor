#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "model.h"

//Andy Duong
#include "canvas.h"

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
signals:
    void updateColor(QColor);

public slots:
    void updateCanvas(QImage*); // jeff!!

    //Andy Duong
    void handleViewClicked();

    //Andy Tran
    void onTimerTimeout();
private:
    Ui::MainWindow *ui;
    int num;

    //Andy Duong
    //This create an instance of canvas object and using QPixmap for pixel stuffs.
    QGraphicsScene *canvas;
    QPixmap pixmap;

    //Andy Tran
    vector<QImage> frameList;
    QGraphicsScene* scene = new QGraphicsScene(this);
    QTimer *timer = new QTimer(this);
    int fps = 12;
    int frameDuration;
    //------------------------

    //TZhou
    QColor* currentColor; // The current color of the pen, default black.
    int currentRgba[4]; // I have good reason to have this variable.
    void setCurrentColorBtnTo();
    void setCurrentRbga(QColor* newColor);
    //----------------------

private slots:
    //TZhou: color picker area
    void on_changeColorBtn_clicked();
    void on_alphaSlider_valueChanged(int value);
    //-------------------------------------------

    void on_fpsSlider_valueChanged(int value);
};
#endif // MAINWINDOW_H
