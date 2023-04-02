#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "model.h"
#include "canvasform.h"
#include "frameview.h"

#include <QDir>
#include <QCoreApplication>
#include <iostream>
#include<QString>
#include<QDebug>

MainWindow::MainWindow(Model& model, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , model(model)
{
    ui->setupUi(this);


    // [=== INITIALIZE SECTION ===]
    {
        //Tzhou
        // Initializes the current color to be black, and its buttons.
        updatePaintColor(DEFAULT_PAINT_COLOR);// alpha value: [0, 225], 0 means transparent, 225 means opaque.

        setIconToToolBtns(); // Jeffrey: is there a way we can move this to only mainwindow.ui?

        //Canvas
        ui->canvasView->updatePixmap(&model.canvas);

        //Frame View
        initializeFrameView();
    }



    // [=== CANVAS CONNECTIONS ===] @Jeffrey
    connect(&model, &Model::updateCanvas, this, &MainWindow::updateCanvas);

    // --- Canvas Input ---
    connect(ui->canvasView, &ImageViewEditor::mouseDown, &model, &Model::mouseDown);
    connect(ui->canvasView, &ImageViewEditor::mousePressed, &model, &Model::mousePressed);

    // [=== PREVIEW CONNECTIONS ===] @Andy Tran
    connect(timer, &QTimer::timeout, this, &MainWindow::onTimerTimeout); // Andy Tran - connection for preview
    connect(ui->fpsSlider, &QSlider::valueChanged, this, &MainWindow::onChangeFpsSliderValue);

    // [=== FRAME VIEW CONNECTIONS ===] @Andy Tran
    connect(&model, &Model::deleteFrameWidget, this, &MainWindow::deleteFrameWidget);

    // [=== TOOL CONNECTIONS ===] @Ruini
    // --- Tool Select ---
    connect(ui->btnPencil,&QPushButton::clicked,this,[=](){
        emit changeTool(PENCIL);
    });
    connect(ui->btnPicker,&QPushButton::clicked,this,[=](){
        emit changeTool(PICKER);
    });
    connect(ui->btnEraser,&QPushButton::clicked,this,[=](){
        emit changeTool(ERASER);
    });
    connect(ui->btnBucket,&QPushButton::clicked,this,[=](){
        emit changeTool(BUCKET);
    });

    connect(this, &MainWindow::changeTool, &model, &Model::changeTool);
    connect(this, &MainWindow::changeTool, this, &MainWindow::disableTool); // Ruini

    // --- Tool Settings ---
    connect(ui->toolSlider, &QSlider::valueChanged, &model, &Model::setPenSize);
    connect(ui->toolSlider, &QSlider::valueChanged,this,&MainWindow::changeSizeSliderValue);

    // --- Tool: Color Picker ---
    connect(ui->canvasView, &ImageViewEditor::getColor, &model, &Model::getColor); // Jeffrey: ideally, Model will have a "Picker" tool enum then just use mouseDown?



    // [=== COLOR CONNECTIONS ===] @TZHou @Ruini
    // --- Color Dialog ---
    connect(ui->changeColorBtn, &QPushButton::pressed, this, &MainWindow::changeColorBtnIsPressed);

    // --- Color Model-View Updates ---
    //1. user pick a color => model's paintColor change.
    connect(this, &MainWindow::paintColorChanged, &model, &Model::paintColorChanged);
    //2. Model informs View to change color
    connect(&model, &Model::updatePaintColor, this, &MainWindow::updatePaintColor);

    // --- Color Alpha Channels ---
    connect(ui->alphaSlider, &QSlider::valueChanged, &model, &Model::updateAlpha);
    connect(&model, &Model::updateAlphaSliderLabel, ui->alphaValueLabel, &QLabel::setText);
    connect(&model, &Model::resetAlphaSlider, ui->alphaSlider, &QSlider::setValue );



    // [=== MENU CONNECTIONS ===] @Duong
    //Handle clicking on new
    connect(ui->actionNew_Project, &QAction::triggered, this, &MainWindow::handleNewCanvas);
    connect(ui->actionSave_Project, &QAction::triggered, this, &MainWindow::handleSaveCanvas);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::handleOpenCanvas);

    connect(&model, &Model::newCanvasCreated, this, &MainWindow::newCanvasCreated);
//    connect(this, &MainWindow::frameSelected, &model, &Model::frameSelected);

}

MainWindow::~MainWindow()
{
    delete ui;
}



// [=== INITIALIZE SECTION ===] @Andy Tran
void MainWindow::initializeFrameView(){
    QPushButton* addFrameBtn = new QPushButton("+");
    addFrameBtn->setFixedSize(64, 64);

    //Frames Layout
    framesHorizontalLayout->setAlignment(Qt::AlignLeft);
    framesHorizontalLayout->addWidget(addFrameBtn);

    ui->framesContents->setLayout(framesHorizontalLayout);

    //Add Frame Button Connection
    connect(addFrameBtn, &QPushButton::clicked, &model, &Model::onAddFrame);
    connect(addFrameBtn, &QPushButton::clicked, this, [=]() {
        addFrameWidget(framesHorizontalLayout);
    });

    addFrameWidget(framesHorizontalLayout);
}

void MainWindow::addFrameWidget(QHBoxLayout *framesHorizontalLayout)
{
    //Andy Tran - need to fix the scroll bar issue
    QImage currentImage = model.canvas;

    //Not Initializing -> set currentImage according to the frameList
    if(!isInit){
        currentImage = frameList[currentFrame];
    }

    FrameView* newFrame = new FrameView();

    //If the size is greater than 0 -> insert at index size - 1
    if(frameList.size() > 0){
        framesHorizontalLayout->insertWidget(frameList.size() - 1, newFrame);

        //Set the border style for the previous and current widgets
        framesHorizontalLayout->itemAt(previousFrame)->widget()->setStyleSheet("border: none");
        framesHorizontalLayout->itemAt(frameList.size())->widget()->setStyleSheet("");
        framesHorizontalLayout->itemAt(frameList.size() - 1)->widget()->setStyleSheet("border: 3px solid black;");

        // Set focus on the newest FrameView
        newFrame->setFocus();
    }
    //Initialize Frame View - insert at index 0
    else{
        framesHorizontalLayout->insertWidget(0, newFrame);
        framesHorizontalLayout->itemAt(0)->widget()->setStyleSheet("border: 3px solid black;");
    }

    newFrame->updatePixmap(&currentImage);
    newFrame->setSceneRect(0, 0, currentImage.width(), currentImage.height()); // set scene rect to image size
    newFrame->setTransform(QTransform::fromScale(newFrame->width() / currentImage.width(),
                                             newFrame->height() / currentImage.height()));

    //Connect events of FrameView and Model
    connect(newFrame, &FrameView::mouseClicked, &model, &Model::mouseClicked);
    connect(newFrame, &FrameView::deletePressed, &model, &Model::deletePressed);
}

void MainWindow::deleteFrameWidget(QImage* canvas, vector<QImage>* list, int currentFrame, int deletedIndex) {
    qDebug() << "deleted Index: " << deletedIndex;
    //Can abtract to a method
    //Update Canvas View
    ui->canvasView->updatePixmap(canvas);

    //Update current frame and frame list
    previousFrame = currentFrame;
    this->currentFrame = currentFrame;
    frameList.clear();
    frameList.resize(list->size());
    frameList = *list;
    //---------------------------------------------------------------
    //Delete a Widget at deletedIndex
    QLayoutItem *item = framesHorizontalLayout->takeAt(deletedIndex);
    delete item->widget();
    delete item;

    //Update color of border according to the currcent frame view
    framesHorizontalLayout->itemAt(this->currentFrame)->widget()->setStyleSheet("border: 3px solid black;");

    //Shift the index of FrameView elements to Left - Need to exclude the Add Frame Button (last element of the layout)
    for(int i = deletedIndex; i < framesHorizontalLayout->count() - 1; i++){
        FrameView *frame = qobject_cast<FrameView*>(framesHorizontalLayout->itemAt(i)->widget());
        if((frame->getIndex() - 1) >= 0)
            frame->updateIndex(frame->getIndex()-1);
    }

    // Set focus on the current FrameView
    FrameView *frame = qobject_cast<FrameView*>(framesHorizontalLayout->itemAt(this->currentFrame)->widget());
    if(frame) frame->setFocus();
}

// [=== CANVAS SECTION ===] @Jeffrey @Andy Tran
void MainWindow::updateCanvas(QImage* canvas, vector<QImage>* list, int currentFrame) {
    //Can abtract to a method
    //update Canvas View
    ui->canvasView->updatePixmap(canvas);

    //Update current frame and frame list
    previousFrame = this->currentFrame;
    this->currentFrame = currentFrame;
    frameList.clear();
    frameList.resize(list->size());
    frameList = *list;
    //---------------------------------------------------------------

    //Update color of border according to the currcent frame view
    framesHorizontalLayout->itemAt(previousFrame)->widget()->setStyleSheet("border: none");
    framesHorizontalLayout->itemAt(this->currentFrame)->widget()->setStyleSheet("border: 3px solid black;");

    //Update the FrameView when modified Canvas
    FrameView *frame = qobject_cast<FrameView*>(framesHorizontalLayout->itemAt(currentFrame)->widget());
    if(frame){
        frame->updatePixmap(&frameList[currentFrame]);

        // Set focus on the current FrameView
        frame->setFocus();
    }

    //Initialize the View at the first place
    if(isInit){
        initializeView();
        isInit = false;
    }
}

void MainWindow::initializeView() {
    //Initialize - Preview
    toPixmapItem.setPixmap(QPixmap::fromImage(frameList.front()));
    previewScene->addItem(&toPixmapItem);
    previewScene->setFocusItem(&toPixmapItem);
    ui->graphicsView->setScene(previewScene);
    timer->start(frameDuration);
}

// [=== PREVIEW SECTION ===] @Andy Tran
void MainWindow::onTimerTimeout() {
    //Move to the next frame when QTimer timeout
    curPreviewIndex = (curPreviewIndex + 1) % frameList.size();
    toPixmapItem.setPixmap(QPixmap::fromImage(frameList[curPreviewIndex]));
    previewScene->setFocusItem(&toPixmapItem);
}

void MainWindow::onChangeFpsSliderValue(int value)
{
    //Change FPS label
    QString textValue = QString::number(value);
    ui->fpsValueLabel->setText(textValue);
    fps = value;

    //Reset Timer with new FPS and Frame Duration
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



// [=== TOOL SECTION ===] @Ruini
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

void MainWindow::changeSizeSliderValue(int value){
    QString textValue = QString::number(value);
    ui->sizeValueLabel->setText(textValue);
}



// [=== COLOR SECTION ===] @TZHou @Ruini
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



// [=== MENU SECTION ===] @Duong
void MainWindow::handleNewCanvas() {
    CanvasForm form(this);
    connect(&form, &CanvasForm::canvasSizeChanged, &model, &Model::createNewCanvas);
    form.exec();
}

void MainWindow::newCanvasCreated() {
    //Andy Tran Edited

    // Delete all existing frame widgets
    for (int i = framesHorizontalLayout->count() - 1; i >= 0; i--) {
        QLayoutItem *item = framesHorizontalLayout->takeAt(i);
        delete item->widget();
        delete item;
    }

    //Reset entire View
    initializeFrameView();
    initializeView();
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

//-------------------Extra Features ----------------------

//tzhou
void MainWindow::setIconToToolBtns(){

    ui->btnPencil->setIcon(QIcon(QPixmap(":/image/image/pencil1.png")));
    ui->btnPencil->setIconSize(QSize(30,30));
    ui->btnBucket->setIcon(QIcon(QPixmap(":/image/image/bucket1.png")));
    ui->btnBucket->setIconSize(QSize(30,30));
    ui->btnEraser->setIcon(QIcon(QPixmap(":/image/image/eraser1.png")));
    ui->btnEraser->setIconSize(QSize(30,30));
    ui->btnPicker->setIcon(QIcon(QPixmap(":/image/image/picker1.png")));
    ui->btnPicker->setIconSize(QSize(30,30));
}
