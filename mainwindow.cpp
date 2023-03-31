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

//-------------

MainWindow::MainWindow(Model& model, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , model(model)
{
    ui->setupUi(this);

    //Andy Tran - Initialize Preview
    startPreview();

    //Tzhou
    // Initializes the current color to be black, and its buttons.
    updatePaintColor(DEFAULT_PAINT_COLOR);// alpha value: [0, 225], 0 means transparent, 225 means opaque.
    //-----------------------------------

    // INITIALIZE VIEW
    ui->btnPencil->setEnabled(false);
    ui->canvasView->updatePixmap(&model.canvas);
    {
        QPushButton* addFrameBtn = new QPushButton("+");
        addFrameBtn->setFixedSize(50, 50);
        framesHorizontalLayout->setAlignment(Qt::AlignLeft);
        framesHorizontalLayout->addWidget(addFrameBtn);
        ui->framesContents->setLayout(framesHorizontalLayout);

        // NOTE, THIS IS A PREVIEW (BAREBONE CONCEPT)
        // Think about how to apply Model-View!
        // user clicks -> model creates new frame -> view creates
        // a bit tricky since we have to destroy frames too
        connect(addFrameBtn, &QPushButton::clicked, &model, &Model::onAddFrame);
        connect(addFrameBtn, &QPushButton::clicked, this, [=]() {
            addFrameWidget(framesHorizontalLayout);
        });
    }

    // CONNECTIONS START HERE
    //Andy Tran - Connection for Preview
    connect(timer, &QTimer::timeout, this, &MainWindow::onTimerTimeout);
//    connect(frameTimer, &QTimer::timeout, this, &MainWindow::onFrameListUpdate);

    connect(ui->canvasView, &ImageViewEditor::mouseDown, &model, &Model::mouseDown);

    //Ruin Eddit
    //connect(ui->canvasView, &ImageViewEditor::mouseMove, &model, &Model::mouseMove);
    connect(&model, &Model::updateCanvas, this, &MainWindow::updateCanvas);
    connect(ui->fpsSlider, &QSlider::valueChanged,
            this, &MainWindow::changeFpsSliderValue);

    //-----------------Tool Box----------------------
    //Ruini
    connect(this, &MainWindow::changeTool, this, &MainWindow::disableTool);
    //handle pencil event
    connect(ui->btnPencil,&QPushButton::clicked,this,[=](){
        emit changeTool(PENCIL);
    });
    //handle color picker event
    connect(ui->btnPicker,&QPushButton::clicked,this,[=](){
        emit changeTool(PICKER);
    });
    //handle eraser event     //Andy Tran Added
    connect(ui->btnEraser,&QPushButton::clicked,this,[=](){
        emit changeTool(ERASER);
    });
    //handle fill bucket event
    connect(ui->btnBucket,&QPushButton::clicked,this,[=](){
        emit changeTool(BUCKET);
    });
    connect(this, &MainWindow::changeTool, &model, &Model::changeTool);

    //tool size
    connect(ui->toolSlider, &QSlider::valueChanged, &model, &Model::setPenSize);
    connect(ui->toolSlider, &QSlider::valueChanged,this,&MainWindow::changeSizeSliderValue);

    //track mouse movenment
    connect(ui->canvasView, &ImageViewEditor::mousePressed, &model, &Model::mousePressed);

    //color picker
    connect(ui->canvasView, &ImageViewEditor::getColor, &model, &Model::getColor);
    //-------------------------------------------------------------


    //-----------------TZHou: Color Picker----------------------
    //handle color picker change color event

    //To remove the warning.
    connect(ui->changeColorBtn, &QPushButton::pressed,
            this, &MainWindow::changeColorBtnIsPressed);

    //1. user pick a color => model's paintColor change.
    connect(this, &MainWindow::paintColorChanged,
            &model, &Model::paintColorChanged);
    //2. Model informs View to change color
    connect(&model, &Model::updatePaintColor,
            this, &MainWindow::updatePaintColor);

    //handle alpha value changed
    connect(ui->alphaSlider, &QSlider::valueChanged,
            &model, &Model::updateAlpha);
    connect(&model, &Model::updateAlphaSliderLabel,
            ui->alphaValueLabel, &QLabel::setText);
    connect(&model, &Model::resetAlphaSlider,
            ui->alphaSlider, &QSlider::setValue );
    //-------------------------------------------------------------


    //Duong
    //Handle clicking on new
    connect(ui->actionNew_Project, &QAction::triggered, this, &MainWindow::handleNewCanvas);
    connect(ui->actionSave_Project, &QAction::triggered, this, &MainWindow::handleSaveCanvas);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::handleOpenCanvas);

    connect(&model, &Model::newCanvasCreated, this, &MainWindow::newCanvasCreated);
    connect(this, &MainWindow::frameSelected, &model, &Model::frameSelected);

}

MainWindow::~MainWindow()
{
    delete ui;
}

// Jeffrey Le *sunglasses_emoji*
void MainWindow::updateCanvas(QImage* canvas, vector<QImage>* list, int currentFrame) {
    ui->canvasView->updatePixmap(canvas);

    //Andy Tran - Update frameList
    this->currentFrame = currentFrame;
    frameList.clear();
    frameList.resize(list->size());
    frameList = *list;

    //update Frames
    onFrameListUpdate();
}

//Andy Tran
void MainWindow::addFrameWidget(QHBoxLayout *framesHorizontalLayout)
{
    //Andy Tran - need to fix the scroll bar issue
    ImageViewEditor* newFrame = new ImageViewEditor();
    newFrame->setFixedSize(64, 64);
    newFrame->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    newFrame->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    framesHorizontalLayout->insertWidget(currentFrame, newFrame);

    //need to move the scroll bar to the right where the new frame was created

    addFrame(newFrame);

    //Duong
    if(currentFrame == 0){
        emit frameSelected(0);
    }
}

void MainWindow::addFrame(ImageViewEditor* newFrame){
    //Andy Tran - need to fix the size issue
    newFrame->updatePixmap(&frameList[currentFrame]);
    newFrame->setSceneRect(0, 0, frameList[currentFrame].width(), frameList[currentFrame].height()); // set scene rect to image size
    newFrame->setTransform(QTransform::fromScale(newFrame->width() / (qreal)frameList[currentFrame].width(),
                                             newFrame->height() / (qreal)frameList[currentFrame].height()));
}

void MainWindow::startPreview() {
    viewSize = ui->graphicsView->viewport()->size();
    QImage blankCanvas(32,32, QImage::Format_ARGB32);
    blankCanvas.fill(Qt::white);
    QImage scaledCanvas = blankCanvas.scaled(viewSize, Qt::KeepAspectRatio, Qt::FastTransformation);
    frameList.push_back(scaledCanvas);

    pixmapItem.setPixmap(QPixmap::fromImage(frameList.front()));
    previewScene->addItem(&pixmapItem);

    // Set the focus item to the first pixmap item
    previewScene->setFocusItem(&pixmapItem);
    ui->graphicsView->setScene(previewScene);   
    timer->start(frameDuration);

    //Andy Tran - need to fix the size issue
    //Initialize for frame menu
    ImageViewEditor* newFrame = new ImageViewEditor();
    newFrame->setFixedSize(64, 64);
    newFrame->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    newFrame->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    framesHorizontalLayout->insertWidget(0, newFrame);

    newFrame->updatePixmap(&blankCanvas);
    newFrame->setSceneRect(0, 0, blankCanvas.width(), blankCanvas.height()); // set scene rect to image size
    newFrame->setTransform(QTransform::fromScale(newFrame->width() / (qreal)blankCanvas.width(),
                                             newFrame->height() / (qreal)blankCanvas.height()));

}

//This is good I guess
void MainWindow::onFrameListUpdate(){
    ImageViewEditor *frame = qobject_cast<ImageViewEditor *>(framesHorizontalLayout->itemAt(currentFrame)->widget());
    if(frame){
        frame->updatePixmap(&frameList[currentFrame]);
    }
}

void MainWindow::onTimerTimeout() {
    currentFrameIndex = (currentFrameIndex + 1) % frameList.size();
    pixmapItem.setPixmap(QPixmap::fromImage(frameList[currentFrameIndex]));

    // Set the focus item to the current pixmap item
    previewScene->setFocusItem(&pixmapItem);
}

void MainWindow::changeFpsSliderValue(int value)
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
void MainWindow::updatePaintColor(QColor newColor)
{
    QString style = QString("QPushButton {background-color: rgba(%1,%2,%3,%4);}");

    ui->currentColorBtn->setStyleSheet(style.arg(newColor.red()).arg(newColor.green())
            .arg(newColor.blue()).arg(newColor.alpha()));

}

void MainWindow::changeColorBtnIsPressed()
{
    QColor color = QColorDialog::getColor(DEFAULT_PAINT_COLOR,this);
    if(color.isValid()) //If the "OK" button is pressed.
    {
       emit paintColorChanged(color); //Ruini Tong
    }
}


//Duong
void MainWindow::handleNewCanvas() {
    CanvasForm form(this);
    connect(&form, &CanvasForm::canvasSizeChanged, &model, &Model::createNewCanvas);
    form.exec();
}

void MainWindow::newCanvasCreated() {

    // Delete all existing frame widgets (except the add frame button)
       int itemCount = framesHorizontalLayout->count() - 1;
       for (int i = itemCount - 1; i >= 0; i--) {
           QLayoutItem *item = framesHorizontalLayout->takeAt(i);
           delete item->widget();
           delete item;
       }

       // Create a new frame widget for the new canvas
       addFrameWidget(framesHorizontalLayout);
   }

void MainWindow::handleSaveCanvas()
{
    QString filter = "Sprite Sheet Project (*.ssp)";
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Sprite Sheet Project"), "", filter, &filter);

    if (!fileName.isEmpty()) {
        if (!fileName.endsWith(".ssp")) {
            fileName += ".ssp";
        }
        model.saveFile(fileName);
    }
}

void MainWindow::handleOpenCanvas()
{
    QString filter = "Sprite Sheet Project (*.ssp)";
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Sprite Sheet Project"), "", filter, &filter);

    if (!fileName.isEmpty()) {
        model.openFile(fileName);
    }
}

//void MainWindow::frameLoaded(ImageViewEditor *frame)
//{
//    frame->setFixedSize(64, 64);
//    frame->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    frame->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    ui->horizontalLayout->addWidget(frame);
//    addFrame(frame);
//}

//Ruini Tong
void MainWindow::changeSizeSliderValue(int value){
    QString textValue = QString::number(value);
    ui->sizeValueLabel->setText(textValue);
}

void MainWindow::disableTool(Tool tool){
    ui->btnPencil->setEnabled(true);
    ui->btnPicker->setEnabled(true);
    ui->btnEraser->setEnabled(true);
    ui->btnBucket->setEnabled(true);

    switch (tool) {
    case PENCIL:
        ui->btnPencil->setEnabled(false);
        break;
    case PICKER:
        ui->btnPicker->setEnabled(false);
        break;
    case ERASER:
        ui->btnEraser->setEnabled(false);
        break;
    case BUCKET:
        ui->btnBucket->setEnabled(false);
        break;
    default:
        break;
    }
}



