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
    void startPreview();
    //----------------------

public slots:
    void updateCanvas(QImage*, vector<QImage>*); // jeff!!

    //Andy Tran
    void onTimerTimeout();
    void changeFpsSliderValue(int value);
    //------------------------------------------


    //-----------------TZhou: color picker area----------------------
    void updatePaintColor(QColor newColor);
    void changeColorBtnIsPressed();
    //-------------------------------------------

    //Andy D
    void handleNewCanvas();

    //Ruini Tong
    void changeSizeSliderValue(int value);
    //-------------------------------------------

private:
    Ui::MainWindow *ui;
    Model& model;

    //Andy Tran
    vector<std::shared_ptr<QImage>> frameList;
    QGraphicsScene* previewScene = new QGraphicsScene(this);
    QTimer *timer = new QTimer(this);
    int fps = 12;
    int frameDuration = 1000/12;
    vector<QPixmap> pixmapFrames;
    QGraphicsPixmapItem pixmapItem;
    QSize viewSize;
    //------------------------

    //TZhou
    QColor DEFAULT_PAINT_COLOR = Qt::black; // Default pen or brush color
    //----------------------


signals:

    //Renee
    // used by any color change
    void paintColorChanged(QColor);
    void newCanvasRequested(int width, int height);

    //Ruini Tong
    void changeTool(Tool);
    //----------------------

};
#endif // MAINWINDOW_H
