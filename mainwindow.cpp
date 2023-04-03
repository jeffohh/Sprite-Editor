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
#include<QPainter>

MainWindow::MainWindow(Model& model, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , model(model)
{
    ui->setupUi(this);


    // [=== INITIALIZE SECTION ===]
    {
        updatePaintColor(DEFAULT_PAINT_COLOR);// Tzhou: initialize the current color label to black

        setColorScenesToViews();// Tzhou: Initialize the views in color picker area.

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
    connect(ui->canvasView, &ImageViewEditor::getColor, &model, &Model::getColor);
    // Jeffrey: ideally, Model will have a "Picker" tool enum then just use mouseDown?



    // [=== COLOR CONNECTIONS ===] @TZHou @Ruini
    // --- Color Dialog ---
    connect(ui->changeColorBtn, &QPushButton::pressed, this, &MainWindow::changeColorBtnIsPressed);

    //Tzhou: EXtra feature
    connect(ui->customColorView1, &DragAndDropGraphicsView::itemDrop,
            this, &MainWindow::imageEnter);
    connect(ui->customColorView2, &DragAndDropGraphicsView::itemDrop,
            this, &MainWindow::imageEnter);
    connect(ui->customColorView3, &DragAndDropGraphicsView::itemDrop,
            this, &MainWindow::imageEnter);
    connect(ui->customColorView4, &DragAndDropGraphicsView::itemDrop,
            this, &MainWindow::imageEnter);
    connect(ui->customColorView5, &DragAndDropGraphicsView::itemDrop,
            this, &MainWindow::imageEnter);
    connect(this, &MainWindow::updateCustomColor,
            &model, &Model::updateCustomColor);
    connect(ui->customColorView1, &DragAndDropGraphicsView::customColorIsSelected,
            &model, &Model::customColorIsSelected);
    connect(ui->customColorView2, &DragAndDropGraphicsView::customColorIsSelected,
            &model, &Model::customColorIsSelected);
    connect(ui->customColorView3, &DragAndDropGraphicsView::customColorIsSelected,
            &model, &Model::customColorIsSelected);
    connect(ui->customColorView4, &DragAndDropGraphicsView::customColorIsSelected,
            &model, &Model::customColorIsSelected);
    connect(ui->customColorView5, &DragAndDropGraphicsView::customColorIsSelected,
            &model, &Model::customColorIsSelected);



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
//    connect(&model, &Model::newCanvasCreated, this, &MainWindow::newCanvasCreated);


}

MainWindow::~MainWindow()
{
    delete ui;
}



// [=== INITIALIZE SECTION ===] @Andy Tran
void MainWindow::initializeFrameView(){
    previousFrame = 0;
    currentFrame = 0;

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
        framesHorizontalLayout->itemAt(frameList.size())->widget()->setStyleSheet("");
    });

    addFrameWidget(framesHorizontalLayout);
}

void MainWindow::addFrameWidget(QHBoxLayout* framesHorizontalLayout)
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
        framesHorizontalLayout->insertWidget(currentFrame, newFrame);

        qDebug() << "frameList.size() addFrameWidget: " << frameList.size();

        //Set the border style for the previous and current widgets
        framesHorizontalLayout->itemAt(previousFrame)->widget()->setStyleSheet("border: none");
        framesHorizontalLayout->itemAt(currentFrame)->widget()->setStyleSheet("border: 3px solid black;");
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

    // Set focus on the newest FrameView
    newFrame->setFocus();

    //Connect events of FrameView and Model
    connect(newFrame, &FrameView::mouseClicked, &model, &Model::mouseClicked);
    connect(newFrame, &FrameView::deletePressed, &model, &Model::deletePressed);
}

void MainWindow::deleteFrameWidget(int deletedIndex) {
    //Delete a Widget at deletedIndex
    QLayoutItem *item = framesHorizontalLayout->takeAt(deletedIndex);
    delete item->widget();
    delete item;
}

void MainWindow::loadWidgets() {

    //Andy Tran: Need to optimized

        // Clear the current frame widgets
       while (!framesHorizontalLayout->isEmpty()) {
           QLayoutItem* item = framesHorizontalLayout->takeAt(0);
           delete item->widget();
           delete item;
       }

       //Add Push Button
       QPushButton* addFrameBtn = new QPushButton("+");
       addFrameBtn->setFixedSize(64, 64);

       //Frames Layout
       framesHorizontalLayout->setAlignment(Qt::AlignLeft);
       framesHorizontalLayout->addWidget(addFrameBtn);

       //Add Frame Button Connection
       connect(addFrameBtn, &QPushButton::clicked, &model, &Model::onAddFrame);
       connect(addFrameBtn, &QPushButton::clicked, this, [=]() {
            addFrameWidget(framesHorizontalLayout);
            framesHorizontalLayout->itemAt(frameList.size())->widget()->setStyleSheet("");
       });

       // Create frame widgets for each frame in the frameList
       for (size_t i = 0; i < frameList.size(); ++i) {
           previousFrame = currentFrame;
           currentFrame = i;
           addFrameWidget(framesHorizontalLayout);
       }

       //
       framesHorizontalLayout->itemAt(currentFrame)->widget()->setStyleSheet("border: none");
       framesHorizontalLayout->itemAt(0)->widget()->setStyleSheet("border: 3px solid black;");

       //
       currentFrame = 0;

       //
       initializeView();
 }

// [=== CANVAS SECTION ===] @Jeffrey @Andy Tran
void MainWindow::updateCanvas(QImage* canvas, vector<QImage>* list, int currentFrame, Action action, int deletedIndex) {
    //update Canvas View
    ui->canvasView->updatePixmap(canvas);
    this->action = action;
    previousFrame = currentFrame;

    //qDebug()<< "update canvas";

    //Update current frame and frame list
    switch(action){
        case UPDATE:
            previousFrame = this->currentFrame;
            break;
        default:
            break;
    }

    this->currentFrame = currentFrame;
    frameList.clear();
    frameList.resize(list->size());
    frameList = *list;

    FrameView *frame;
    switch(action){
        case DELETE_FRAME:
            if(deletedIndex < 0)
                break;

            deleteFrameWidget(deletedIndex);

            //Update color of border according to the currcent frame view
            framesHorizontalLayout->itemAt(this->currentFrame)->widget()->setStyleSheet("border: 3px solid black;");

            //Shift the index of FrameView elements to Left - Need to exclude the Add Frame Button (last element of the layout)
            for(int i = deletedIndex; i < framesHorizontalLayout->count() - 1; i++){
                FrameView *frame = qobject_cast<FrameView*>(framesHorizontalLayout->itemAt(i)->widget());
                if((frame->getIndex() - 1) >= 0)
                    frame->updateIndex(frame->getIndex()-1);
            }

            // Set focus on the current FrameView
            frame = qobject_cast<FrameView*>(framesHorizontalLayout->itemAt(this->currentFrame)->widget());
            if(frame) frame->setFocus();
            break;
        case UPDATE:
            //Update color of border according to the currcent frame view
            framesHorizontalLayout->itemAt(previousFrame)->widget()->setStyleSheet("border: none");
            framesHorizontalLayout->itemAt(this->currentFrame)->widget()->setStyleSheet("border: 3px solid black;");

            //Update the FrameView when modified Canvas
            frame = qobject_cast<FrameView*>(framesHorizontalLayout->itemAt(currentFrame)->widget());
            if(frame){
                frame->updatePixmap(&frameList[currentFrame]);

                // Set focus on the current FrameView
                frame->setFocus();
            }
            break;
        case OPEN_FILE:
            loadWidgets();
            break;
        case CREATE_NEW:
            newCanvasCreated();
        default:
            break;
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
    //QString style = QString("QPushButton {background-color: rgba(%1,%2,%3,%4);}");

    //ui->currentColorBtn->setStyleSheet(style.arg(newColor.red()).arg(newColor.green())
    //        .arg(newColor.blue()).arg(newColor.alpha()));

    int h = ui->label->height();
    int w = ui->label->width();
    QPixmap pix(w, h); // give pixmap some size
    pix.fill( newColor);
    QPainter paint(&pix); // assign painter to it. note the &
    ui->label->setPixmap(pix);
    ui->label->setParent(ui->colorPanel);
    qDebug()<<"Update Panint color";

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
    connect(&form, &CanvasForm::createNewCanvas, &model, &Model::createNewCanvas);
    form.exec();
}

void MainWindow::newCanvasCreated() {
    // Clear the current frame widgets
       while (!framesHorizontalLayout->isEmpty()) {
           QLayoutItem* item = framesHorizontalLayout->takeAt(0);
           delete item->widget();
           delete item;
       }

       // Reinitialize the frame view and preview components
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
//tzhou
void MainWindow::setColorScenesToViews()
{
    ui->customColorView1->setScene(customeColorScene1);
    ui->customColorView2->setScene(customeColorScene2);
    ui->customColorView3->setScene(customeColorScene3);
    ui->customColorView4->setScene(customeColorScene4);
    ui->customColorView5->setScene(customeColorScene5);
}

//Tzhou
void MainWindow::imageEnter(QPixmap pixmap, QGraphicsView *view)
{
    QGraphicsScene* scene = view->scene();
    if(scene->items().count()>0){
        scene->clear();
    }
    scene->addPixmap(pixmap.scaled(26,26));
    view->setScene(scene);
    view->viewport()->update();
    view->update();
    view->show();

    emit updateCustomColor(view);
}

