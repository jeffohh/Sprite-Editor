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

public slots:
    // jeff!!
    void updateCanvas(QImage*, vector<QImage>*, int, Action, int);
    void updatePreviewCanvas(QImage*);

    //Andy Tran
    void onTimerTimeout();
    void onChangeFpsSliderValue(int value);
    void addFrameWidget(QHBoxLayout* framesHorizontalLayout);
    void initializePreview();
    void initializeFrameView();
    void deleteFrameWidget(int);
    void deleteAllWidgets();
    void loadFrameWidgets();
//    void transformGraphicsView(QGraphicsView*,QImage*);
//    void resizeAllFrameWidgets();

    //Duong
    void handleNewCanvas();
    void handleOpenCanvas();
    void handleSaveCanvas();
    void centerAndAutoZoom(int, int);
    void handleSize(); //AndyTran added


    //tzhou
    void updatePaintColor(QColor newColor);
    void changeColorBtnIsPressed();
    void imageEnter(QPixmap pixmap, QGraphicsView *view);//Extra Feature


    //Ruini Tong
    void changeSizeSliderValue(int value);
    void disableTool(Tool);
    //-------------------------------------------

private:
    Ui::MainWindow *ui;
    Model& model;

    //Andy Tran
    bool isInit = true;
    Action action = UPDATE;
    int canvasSize = 32;

    //Clone from Model's frameList
    vector<QImage> frameList;

    //Frame View
    int currentFrame = 0;
    int previousFrame = 0;
    QHBoxLayout* framesHorizontalLayout = new QHBoxLayout();

    //Preview
    QSize viewSize;
    QGraphicsScene* previewScene = new QGraphicsScene(this);
    QGraphicsPixmapItem toPixmapItem;
    QTimer *timer = new QTimer();
    int fps = 12;
    int frameDuration = 1000/12;
    unsigned int curPreviewIndex = 0;
    //------------------------

    //TZhou
    QColor DEFAULT_PAINT_COLOR = Qt::black; // Default pen or brush color
    QGraphicsScene* customeColorScene1 = new QGraphicsScene();
    QGraphicsScene* customeColorScene2 = new QGraphicsScene();
    QGraphicsScene* customeColorScene3 = new QGraphicsScene();
    QGraphicsScene* customeColorScene4 = new QGraphicsScene();
    QGraphicsScene* customeColorScene5 = new QGraphicsScene();
    void setIconToToolBtns();
    void setColorScenesToViews();

    //----------------------


signals:
    //Andy Tran
    void resizeFrameList(int);

    //Renee
    // used by any color change
    void paintColorChanged(QColor);
    void newCanvasRequested(int width, int height);

    //Ruini Tong
    void changeTool(Tool);
    void pencilCursor();
    void eraserCursor();
    void pickerCursor();
    void bucketCursor();

    //Tzhou
    void updateCustomColor(QGraphicsView*);

};
#endif // MAINWINDOW_H
