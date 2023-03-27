#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "model.h"

#include <iostream>
using std::cout;
using std::endl;

//Tzhou
#include<QString>
#include<QDebug>
//-----------------------


MainWindow::MainWindow(Model& model, QWidget *parent)
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

    //Tzhou
    // Initializes the current color to be black, and its buttons.
    QColor* black = new QColor(0,0,0, 255);
    ui->alphaSlider->setMinimum(0);
    ui->alphaSlider->setMaximum(10);
    ui->alphaSlider->setValue(10);
    setCurrentColorBtnTo(black);// alpha value: [0, 225], 0 means transparent, 225 means opaque.
    //-----------------------------------


    // INITIALIZE VIEW
    ui->canvasView->updatePixmap(&model.canvas);

    // CONNECTIONS START HERE
    connect(ui->canvasView, &ImageViewEditor::mouseDown, &model, &Model::mouseDown);
    connect(&model, &Model::updateCanvas, this, &MainWindow::updateCanvas);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Jeffrey Le *sunglasses_emoji*
void MainWindow::updateCanvas(QImage* canvas) {
    // we can update preview elements here?
    ui->canvasView->updatePixmap(canvas);
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


//Tzhou
void MainWindow::on_changeColorBtn_clicked()
{
    bool OKBtnIsPressed;
    QColor color = QColorDialog::getColor(*currentColor, this);
    if(&OKBtnIsPressed)
    {
        currentColor = &color;
        setCurrentColorBtnTo(&color);
    }
}

//Tzhou
void MainWindow::setCurrentColorBtnTo(QColor* newColor)
{
    currentColor = newColor;
    int r = newColor->red();
    int g = newColor->green();
    int b = newColor->blue();
    int a = newColor->alpha();
    QString style = QString("QPushButton {background-color: rgba(%1,%2,%3,%4);}");
    ui->currentColorBtn->setStyleSheet(style.arg(r).arg(g).arg(b).arg(a));
    qDebug()<<r<<g<<b<<a;
}

//TZhou
void MainWindow::on_alphaSlider_valueChanged(int value)
{
    QString realValue = QString::number(value/10.0, 'f', 1);
    ui->alphaValueLabel->setText(realValue);
    int alpha = 255*value/10.0;
    currentColor = new QColor(currentColor->red(),currentColor->green(),
                             currentColor->blue(), alpha);

    //Note: color->setAlpha will not work, have to assign a new object.
    setCurrentColorBtnTo(currentColor);
}

