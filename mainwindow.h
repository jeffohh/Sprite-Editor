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
    /**
     * @author Andy Tran, Jeffrey Le
     * @brief MainWindow::updateCanvas Update all the information that needed were sent from Model
     * @param canvas Current Canvas
     * @param list Current Frame List
     * @param currentFrame Current Frame focusing
     * @param action Current Action need to execute by the View
     * @param newSize Current Size of the Sprite
     * @param deletedIndex Current index that need to be deleted (default is -1 which means nothing should be deleted)
     */
    void updateCanvas(QImage*, vector<QImage>*, int, Action, int, int);

    /**
     * @author Jeffrey Le
     * @brief Displays the Model's mergeCanvas
     *
     * Displays the Model's view of the mergeCanvas, replicating it to the View.
     *
     * @param QImage*   the canvas
     */
    void updatePreviewCanvas(QImage* canvas);

    /**
     * @author Andy Tran
     * @brief MainWindow::onTimerTimeout Automatically called whenever the Timer is timeout to continue the animation
     */
    void onTimerTimeout();

    /**
     * @author Andy Tran
     * @brief MainWindow::onChangeFpsSliderValue Whenever FPS changed, update the FPS label
     * and restart the QTimer to start the animation with new FPS
     * @param value
     */
    void onChangeFpsSliderValue(int value);

    /**
     * @author Andy Tran
     * @brief MainWindow::deleteFrameWidget Delete a widget at an particular index
     * @param deletedIndex the index that needs to be deleted
     */
    void deleteFrameWidget(int);

    //Duong
    void handleNewCanvas();
    void handleOpenCanvas();
    void handleSaveCanvas();
    void handleSize();

    /**
     * @authors Tingting Zhou, Ruini Tong
     * @brief displayAbout Displays About content of this project
     */
    void displayAbout();

    /**
     * @author Tingting Zhou
     * @brief updatePaintColor Updates the current color
     * @param newColor new color to set to
     */
    void updatePaintColor(QColor newColor);

    /**
     * @author Tingting Zhou, Ruini Tong
     * @brief changeColorBtnIsPressed Provokes a color dialog for user to change color
     */
    void changeColorBtnIsPressed();

    /**
     * @author Tingting Zhou
     * @brief imageEnter Adds the drap and drop image to the view.
     * @param pixmap the image is to be dropped
     * @param view the view accepts the image
     */
    void imageEnter(QPixmap pixmap, QGraphicsView *view);

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

    //Color panel
    QColor DEFAULT_PAINT_COLOR = Qt::black; // Default pen or brush color
    QGraphicsScene* customeColorScene1 = new QGraphicsScene();
    QGraphicsScene* customeColorScene2 = new QGraphicsScene();
    QGraphicsScene* customeColorScene3 = new QGraphicsScene();
    QGraphicsScene* customeColorScene4 = new QGraphicsScene();
    QGraphicsScene* customeColorScene5 = new QGraphicsScene();

    /**
     * @authors Tingting Zhou, Andy Tran, Ruini Tong
     * @brief setIconToToolBtns Sets icons to tool buttons
     */
    void setIconToToolBtns();

    /**
     * @author Tingting Zhou
     * @brief setColorScenesToViews Sets the scenes for each view in custom color area
     */
    void setColorScenesToViews();

    /**
     * @author Andy Tran
     * @brief MainWindow::initializePreview Initialize pixmap, scene, view. Ready to start the animation
     */
    void initializePreview();

    /**
     * @author Andy Tran
     * @brief MainWindow::initializeFrameView Initialize the Frame View which included
     * Add Button, Connection between the Add Button and Model, Frame View Layout.
     */
    void initializeFrameView();

    /**
     * @author Andy Tran
     * @brief MainWindow::addFrameWidget Add a new frame widget
     */
    void addFrameWidget();

    /**
     * @author Andy Tran
     * @brief MainWindow::loadFrameWidgets Generated all the widgets from .ssp file including "+" button
     */
    void loadFrameWidgets();

    /**
     * @author Andy Tran
     * @brief MainWindow::deleteAllWidgets Delete all current Widgets in Frame View
     */
    void deleteAllWidgets();

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

    /**
     * @author Tingting Zhou
     * @brief updateCustomColor Signal to change the custom color
     */
    void updateCustomColor(QGraphicsView*);

};
#endif // MAINWINDOW_H
