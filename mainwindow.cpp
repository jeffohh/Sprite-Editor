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

    //Andy Tran - Initialize
    ui->fpsSlider->setMinimum(0);
    ui->fpsSlider->setMaximum(60);
    ui->fpsSlider->setValue(12);
    startPreview();

    //Tzhou
    // Initializes the current color to be black, and its buttons.
    //currentColor = DEFAULT_PAINT_COLOR;
    setPaintColorView(DEFAULT_PAINT_COLOR);// alpha value: [0, 225], 0 means transparent, 225 means opaque.
    ui->alphaSlider->setMinimum(0);
    ui->alphaSlider->setMaximum(10);
    ui->alphaSlider->setValue(10);
    //-----------------------------------


    // INITIALIZE VIEW
    ui->btnPencil->setEnabled(false);
    ui->canvasView->updatePixmap(&model.canvas);

    // CONNECTIONS START HERE
    //Andy Tran - Connection
    connect(timer, &QTimer::timeout, this, &MainWindow::onTimerTimeout);

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
void MainWindow::updateCanvas(QImage* canvas, vector<QImage>* frameList) {
    // we can update preview elements here?
    ui->canvasView->updatePixmap(canvas);


    //Andy Tran - Update pixmapFrames
    viewSize = ui->graphicsView->viewport()->size();
    pixmapFrames.clear();
    for (const auto& image : *frameList) {
        QImage scaledImage = image.scaled(viewSize, Qt::KeepAspectRatio, Qt::FastTransformation);
        pixmapFrames.push_back(QPixmap::fromImage(scaledImage));
    }
}

//Andy Tran
void MainWindow::startPreview() {
    pixmapFrames.push_back(QPixmap::fromImage(model.canvas));
    qDebug() << pixmapFrames.front();
    pixmapItem.setPixmap(pixmapFrames.front());
    previewScene->addItem(&pixmapItem);

    // Set the focus item to the first pixmap item
    previewScene->setFocusItem(&pixmapItem);
    ui->graphicsView->setScene(previewScene);
    timer->start(frameDuration);
}

void MainWindow::onTimerTimeout() {
    currentFrameIndex = (currentFrameIndex + 1) % pixmapFrames.size();
    pixmapItem.setPixmap(pixmapFrames[currentFrameIndex]);

    // Set the focus item to the current pixmap item
    previewScene->setFocusItem(&pixmapItem);
}

void MainWindow::on_fpsSlider_valueChanged(int value)
{
    QString textValue = QString::number(value);
    ui->fpsValueLabel->setText(textValue);
    fps = value;

    if(fps == 0){
        frameDuration = 0;
        timer->stop();
    }
    else{
        frameDuration = 1000 / fps;
        if(timer->isActive()) {
            timer->stop();
        }
        timer->start(frameDuration);
    }

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


