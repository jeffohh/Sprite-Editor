#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "model.h"
#include <QFileDialog>

//AndyTran
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QImage>
#include <QPixmap>
#include <QVector>
#include <QTimer>

//TZhou
#include<QColorDialog>
#include<QColor>

//from .cpp
#include <QDir>
#include <QCoreApplication>
#include <iostream>
#include<QString>
#include<QDebug>
#include<QPainter>
#include <QHBoxLayout>

#include "helpmenu.h"

using std::vector;

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
    //Jeff
    /**
     * @author AndyTran Jeff
     * @brief MainWindow::updateCanvas Update all the information that needed were sent from Model
     * @param canvas Current Canvas
     * @param list Current Frame List
     * @param currentFrame Current Frame focusing
     * @param action Current Action need to execute by the View
     * @param newSize Current Size of the Sprite
     * @param deletedIndex Current index that need to be deleted (default is -1 which means nothing should be deleted)
     */
    void updateCanvas(QImage*, vector<QImage>*, int, Action, int, int);
    void updatePreviewCanvas(QImage*);

    //Andy Tran
    /**
     * @author AndyTran
     * @brief MainWindow::onTimerTimeout Automatically called whenever the Timer is timeout to continue the animation
     */
    void onTimerTimeout();

    /**
     * @author AndyTran
     * @brief MainWindow::onChangeFpsSliderValue Whenever FPS changed, update the FPS label
     * and restart the QTimer to start the animation with new FPS
     * @param value
     */
    void onChangeFpsSliderValue(int value);

    /**
     * @author AndyTran
     * @brief MainWindow::addFrameWidget Add a new frame widget
     */
    void addFrameWidget();

    /**
     * @author AndyTran
     * @brief MainWindow::initializePreview Initialize pixmap, scene, view. Ready to start the animation
     */
    void initializePreview();

    /**
     * @author AndyTran
     * @brief MainWindow::initializeFrameView Initialize the Frame View which included
     * Add Button, Connection between the Add Button and Model, Frame View Layout.
     */
    void initializeFrameView();

    /**
     * @author AndyTran
     * @brief MainWindow::deleteFrameWidget Delete a widget at an particular index
     * @param deletedIndex the index that needs to be deleted
     */
    void deleteFrameWidget(int);

    /**
     * @author AndyTran
     * @brief MainWindow::deleteAllWidgets Delete all current Widgets in Frame View
     */
    void deleteAllWidgets();

    /**
     * @author AndyTran
     * @brief MainWindow::loadFrameWidgets Generated all the widgets from .ssp file including "+" button
     */
    void loadFrameWidgets();

    //Duong
    void handleNewCanvas();
    void handleOpenCanvas();
    void handleSaveCanvas();
    void centerAndAutoZoom(int, int);
    void handleSize();
    void displayAbout();

    //tzhou
    void updatePaintColor(QColor newColor);
    void changeColorBtnIsPressed();
    void imageEnter(QPixmap pixmap, QGraphicsView *view);//Extra Feature

    /**
     * @author Ruini Tong
     * @brief label display current size value
     * @param value
     */
    void changeSizeSliderValue(int value);

    /**
     * @author Ruini Tong
     * @brief disable slected tool, enable all the other tools
     * @param tool
     */
    void disableTool(Tool);

    /**
     * @author:Ruini Tong
     * @brief This method display the help menu.
     */
    void displayHelpMenu();

private:
    Ui::MainWindow *ui;
    Model& model;
    helpmenu helpMenu;

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
    QGraphicsPixmapItem imageItem;
    QTimer *timer = new QTimer();
    int fps = 12;
    int frameDuration = 1000/12;
    unsigned int curPreviewIndex = 0;

    //TZhou
    QColor DEFAULT_PAINT_COLOR = Qt::black; // Default pen or brush color
    QGraphicsScene* customeColorScene1 = new QGraphicsScene();
    QGraphicsScene* customeColorScene2 = new QGraphicsScene();
    QGraphicsScene* customeColorScene3 = new QGraphicsScene();
    QGraphicsScene* customeColorScene4 = new QGraphicsScene();
    QGraphicsScene* customeColorScene5 = new QGraphicsScene();
    void setIconToToolBtns();
    void setColorScenesToViews();

signals:
    //AndyTran
    void changeFPS();

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
