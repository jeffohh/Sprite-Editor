#include "mainwindow.h"
#include "canvasform.h"
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
    , model(model)
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
    //currentColor = DEFAULT_PAINT_COLOR;
    setPaintColorView(DEFAULT_PAINT_COLOR);// alpha value: [0, 225], 0 means transparent, 225 means opaque.
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

    //-----------------TZHou: Color Picker----------------------
    //handle color picker change color event
    //1. user pick a color => model's paintColor change.
    connect(this, &MainWindow::updateColor,
            &model, &Model::updatePaintColor);
    //2. Model informs View to change color
    connect(&model, &Model::updateColorPickerPanel,
            this, &MainWindow::setPaintColorView);

    //handle alpha value changed
    connect(ui->alphaSlider, &QSlider::valueChanged,
            &model, &Model::updateAlpha);
    connect(&model, &Model::updateAlphaSliderLabel,
            ui->alphaValueLabel, &QLabel::setText);

    connect(&model, &Model::resetAlphaSlider,
            ui->alphaSlider, &QSlider::setValue );
    //-------------------------------------------------------------


    //Andy Tran Added
    //handle eraser event
    connect(ui->btnEraser,&QPushButton::clicked,ui->canvasView,&ImageViewEditor::eraserClicked);
    connect(ui->btnEraser,&QPushButton::clicked,this, [=](){
            ui->btnPencil->setEnabled(true);
            ui->btnEraser->setEnabled(false);
        });
    connect(ui->canvasView, &ImageViewEditor::changeTool, &model, &Model::changeTool);

    //Duong
    //Handle clicking on new
    connect(ui->actionNew_Project, &QAction::triggered, this, &MainWindow::handleNewCanvas);

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
void MainWindow::setPaintColorView(QColor newColor)
{
    QString style = QString("QPushButton {background-color: rgba(%1,%2,%3,%4);}");

    ui->currentColorBtn->setStyleSheet(style.arg(newColor.red()).arg(newColor.green())
            .arg(newColor.blue()).arg(newColor.alpha()));

}

void MainWindow::on_changeColorBtn_clicked()
{
    bool OKBtnIsPressed;
    QColor color = QColorDialog::getColor(DEFAULT_PAINT_COLOR,this);
    if(&OKBtnIsPressed && color.isValid())
    {
       //comment edited:  update paint color in Model - tzhou
       emit updateColor(color); //Ruini Tong
    }
}




//Duong
void MainWindow::handleNewCanvas() {
    CanvasForm form(this);
    connect(&form, &CanvasForm::canvasSizeChanged, &model, &Model::createNewCanvas);
    form.exec();
}


