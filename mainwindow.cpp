#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "model.h"

#include <iostream>

//Tzhou
#include<QString>
#include<QDebug>
//-----------------------


//Andy Tran
unsigned int currentFrameIndex = 0;
bool needsRestart = false;
//-------------

MainWindow::MainWindow(Model& model, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Andy Tran
    connect(timer, &QTimer::timeout, this, &MainWindow::onTimerTimeout);

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

    ui->fpsSlider->setMinimum(1);
    ui->fpsSlider->setMaximum(60);
    ui->fpsSlider->setValue(12);
    previewAnimation();



    //Tzhou
    // Initializes the current color to be black, and its buttons.
    QColor* black = new QColor(0,0,0, 255);
    currentColor = black;
    setCurrentColorBtnTo();// alpha value: [0, 225], 0 means transparent, 225 means opaque.
    ui->alphaSlider->setMinimum(0);
    ui->alphaSlider->setMaximum(10);
    ui->alphaSlider->setValue(10);
    //-----------------------------------


    // INITIALIZE VIEW
    ui->canvasView->updatePixmap(&model.canvas);

    // CONNECTIONS START HERE
    connect(ui->canvasView, &ImageViewEditor::mouseDown, &model, &Model::mouseDown);
    connect(ui->canvasView, &ImageViewEditor::mouseMove, &model, &Model::mouseMove);

    connect(&model, &Model::updateCanvas, this, &MainWindow::updateCanvas);

    //Ruini Tong
    //handle pencil event
    connect(ui->btnPencil,&QPushButton::clicked,ui->canvasView,&ImageViewEditor::pencilClicked);
    connect(ui->btnPencil,&QPushButton::clicked,this, [=](){
            ui->btnPencil->setEnabled(false);
            ui->btnEraser->setEnabled(true);
        });

    //Andy Tran Added
    //handle eraser event
    connect(ui->btnEraser,&QPushButton::clicked,ui->canvasView,&ImageViewEditor::eraserClicked);
    connect(ui->btnEraser,&QPushButton::clicked,this, [=](){
            ui->btnPencil->setEnabled(true);
            ui->btnEraser->setEnabled(false);
        });
    connect(ui->canvasView, &ImageViewEditor::changeTool, &model, &Model::changeTool);

    connect(this,&MainWindow::updateColor,&model,&Model::setToolColor);
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
void MainWindow::onTimerTimeout() {
    // Get the current frame out of the list
    QImage currentFrame = frameList[currentFrameIndex];

    // Update the QGraphicsScene with the current frame
    scene->addPixmap(QPixmap::fromImage(currentFrame));

    // Set the focus to the first item in the scene
    if (!scene->items().isEmpty()) {
        QGraphicsItem* firstItem = scene->items().first();
        scene->setFocusItem(firstItem);
    }

    // Move to the next frame
    currentFrameIndex = (currentFrameIndex + 1)%frameList.size();

    // Check if the animation needs to be restarted
    if (needsRestart && currentFrameIndex == 0) {
        needsRestart = false;
        timer->start(frameDuration);
    }
}

void MainWindow::previewAnimation() {
    //Clear and update from the frameList
    scene->clear();
    for(unsigned int i = 0; i < frameList.size();i++){
        scene->addPixmap(QPixmap::fromImage(frameList[i]));
    }

    // Set the timer to fire every frameDuration milliseconds
    frameDuration = 1000 / fps;
    if(timer->isActive()) {
        // Stop the timer and set a flag to indicate that the animation needs to be restarted
        timer->stop();
        needsRestart = true;
    } else {
        // Set the current frame index to 0 if the timer is not running
        currentFrameIndex = 0;
    }
    timer->start(frameDuration);

    ui->graphicsView->setScene(scene);
    ui->graphicsView->setFocus();
}


void MainWindow::on_fpsSlider_valueChanged(int value)
{
    QString textValue = QString::number(value);
    ui->fpsValueLabel->setText(textValue);
    fps = value;
    previewAnimation();
}
//----------------------------------------------------------------

//Tzhou
void MainWindow::on_changeColorBtn_clicked()
{
    bool OKBtnIsPressed;
    QColor color = QColorDialog::getColor(*currentColor, this);
    if(&OKBtnIsPressed)
    {
        currentColor = &color;
        on_alphaSlider_valueChanged(10);
        setCurrentColorBtnTo();

        qDebug()<<currentColor->red()<<currentColor->green()<<currentColor->blue()<<currentColor->alpha();
    }
}

//Tzhou
void MainWindow::setCurrentColorBtnTo()
{

    setCurrentRbga(currentColor);
    QString style = QString("QPushButton {background-color: rgba(%1,%2,%3,%4);}");

    ui->currentColorBtn->setStyleSheet(style.arg(currentRgba[0]).arg(currentRgba[1])
            .arg(currentRgba[2]).arg(currentRgba[3]));

    // show team : this doesn't work.
    //ui->currentColorBtn->setStyleSheet(style.arg(currentColor->red()).arg(currentColor->green())
    //            .arg(currentColor->blue()).arg(currentColor->alpha()));

    qDebug()<<"currentColor: "<<currentColor->red()<<currentColor->green()
           <<currentColor->blue()<<currentColor->alpha();

}

//TZhou
void MainWindow::setCurrentRbga(QColor *newColor)
{
    //update pencil color
    emit updateColor(*newColor); //Ruini Tong

    currentRgba[0] = newColor->red();
    currentRgba[1] = newColor->green();
    currentRgba[2]  = newColor->blue();
    currentRgba[3] = newColor->alpha();
}

//TZhou
void MainWindow::on_alphaSlider_valueChanged(int value)
{
    QString realValue = QString::number(value/10.0, 'f', 1);
    ui->alphaValueLabel->setText(realValue);
    int alpha = 255*value/10.0;
    ui->alphaSlider->setValue(value);

    currentColor->setAlpha(alpha);
    currentRgba[3] = alpha;

    /*** setCurrentColorBtnTo(currentColor) doesn't work *****/
    QString style = QString("QPushButton {background-color: rgba(%1,%2,%3,%4);}");
    ui->currentColorBtn->setStyleSheet(style.arg(currentRgba[0]).arg(currentRgba[1])
            .arg(currentRgba[2]).arg(currentRgba[3]));

    // show team
    //setCurrentColorBtnTo();
    /*******************************************************/

    qDebug()<<"int array: "<<currentColor->red()<<currentColor->green()<<currentColor->blue()<<currentColor->alpha();

}



