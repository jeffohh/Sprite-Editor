#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "model.h"

#include <iostream>
using std::cout;
using std::endl;

MainWindow::MainWindow(Model& model,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    num = 90;

    //Andy Tran
    // Create a QGraphicsScene object to hold the preview animation frames
    scene = new QGraphicsScene(this);

    //Just for testing
    // Create QImage objects for each frame of the animation
    QImage frame1(ui->graphicsView->viewport()->size(), QImage::Format_ARGB32);
    frame1.fill(Qt::red);
    QImage frame2(ui->graphicsView->viewport()->size(), QImage::Format_ARGB32);
    frame2.fill(Qt::blue);
    QImage frame3(ui->graphicsView->viewport()->size(), QImage::Format_ARGB32);
    frame3.fill(Qt::green);
    frameList.push_back(frame1);
    frameList.push_back(frame2);
    frameList.push_back(frame3);

    previewAnimation();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Andy Tran
void MainWindow::previewAnimation() {
    scene->clear();
    for(unsigned int i = 0; i < frameList.size();i++){
        scene->addPixmap(QPixmap::fromImage(frameList[i]));
    }

    // Create a QTimer object to update the display with the next frame
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [=]() {
        // Get the current frame index
        static unsigned int frameIndex = 0;

        // Get the current frame out of the list
        QImage currentFrame = frameList[frameIndex];

        // Update the QGraphicsScene with the current frame
        scene->addPixmap(QPixmap::fromImage(currentFrame));

        // Set the focus to the first item in the scene
        if (!scene->items().isEmpty()) {
            scene->setFocusItem(scene->items().first());
        }

        //Reset the frameIndex to start over
        if(frameIndex == frameList.size())
            frameIndex = 0;

        //move to the next frame
        frameIndex++;
    });

    // Set the timer to fire every frameDuration milliseconds
    int fps = 10; // need to change according to the slider
    int frameDuration = 1000 / fps;
    timer->start(frameDuration);

    // Set the QGraphicsScene to the QGraphicsView
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setFocus();
}


