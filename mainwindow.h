#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "model.h"
#include <QFileDialog>

//Added by Andy Tran
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QImage>
#include <QPixmap>
#include <QVector>
#include <QTimer>
#include "imagevieweditor.h"
#include "QHBoxLayout"
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
    void updateCanvas(QImage*, vector<QImage>*, int); // jeff!!

    //Andy Tran
    void onTimerTimeout();
    void onFrameListUpdate();
    void changeFpsSliderValue(int value);
    void addFrame(ImageViewEditor* newFrame);
    void addFrameWidget(QHBoxLayout *framesHorizontalLayout);
    //------------------------------------------

    //Duong
    void handleNewCanvas();
    void newCanvasCreated();
    void handleOpenCanvas();
    void handleSaveCanvas();

    //-----------------TZhou: color picker area----------------------
    void updatePaintColor(QColor newColor);
    void changeColorBtnIsPressed();
    //-------------------------------------------

    //Ruini Tong
    void changeSizeSliderValue(int value);
    void disableTool(Tool);
    //-------------------------------------------

private:
    Ui::MainWindow *ui;
    Model& model;

    //Andy Tran
    vector<QImage> frameList;
    QHBoxLayout* framesHorizontalLayout = new QHBoxLayout(this);
    QGraphicsScene* previewScene = new QGraphicsScene(this);
    QTimer *timer = new QTimer(this);
    int fps = 12;
    int frameDuration = 1000/12;
    QGraphicsPixmapItem pixmapItem;
    QSize viewSize;
    int currentFrame = 0;
    unsigned int currentFrameIndex = 0;
    bool needsRestart = false;
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

    //Duong
    void frameSelected(int index);

};
#endif // MAINWINDOW_H
