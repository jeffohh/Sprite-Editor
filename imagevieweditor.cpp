#include "imagevieweditor.h"
#include <QMouseEvent>

#include <QDebug>

ImageViewEditor::ImageViewEditor(QWidget *parent) :
    QGraphicsView(parent)
{
    setStyleSheet("background-color: grey;");
    setTransform(QTransform().scale(scale, scale));

    imageItem = new QGraphicsPixmapItem();

    QGraphicsScene* scene = new QGraphicsScene();
    scene->addItem(imageItem);

    this->setScene(scene);

    QPixmap pencilImage(":/image/image/pencil1.png");
    pencilImage = pencilImage.scaled(32, 32, Qt::KeepAspectRatio);
    QCursor pencilCursor(pencilImage, 1,25);
    setCursor(pencilCursor);
}



// [=== CANVAS SECTION ===] @Jeffrey
void ImageViewEditor::updatePixmap(QImage* image) {
    imageItem->setPixmap(QPixmap::fromImage(*image));
}



// [=== INPUT SECTION ===] @Jeffrey
void ImageViewEditor::mousePressEvent(QMouseEvent *event) {
        if (event->button() == Qt::LeftButton) {
            QPoint pos = mapToScene(event->pos()).toPoint();
            emit mouseDown(pos);
            emit getColor(pos);
        }
        emit mousePressed(true);
}

void ImageViewEditor::mouseMoveEvent(QMouseEvent *event) {

    //Andy Tran Edited
    QPoint pos = mapToScene(event->pos()).toPoint();
    //emit mouseDown(pos);
    emit mouseMove(pos);

}

void ImageViewEditor::mouseReleaseEvent(QMouseEvent *event) {
    event->ignore();

    //Ruini Tong
    emit mousePressed(false);
}

void ImageViewEditor::wheelEvent(QWheelEvent *event)
{

        // Get the current zoom level

        // Zoom in or out based on the wheel delta
        if (event->angleDelta().y() > 0) {
            scale *= 1.2;
        } else {
            scale /= 1.2;
        }

        // Set the new zoom level
        setTransform(QTransform().scale(scale, scale));
}

//change cursor image
void ImageViewEditor::pencilCursor(){

    QPixmap pencilImage(":/image/image/pencil1.png");
    pencilImage = pencilImage.scaled(32, 32, Qt::KeepAspectRatio);
    QCursor pencilCursor(pencilImage, 1,25);
    setCursor(pencilCursor);
}

void ImageViewEditor::eraserCursor(){
    QPixmap eraserImage(":/image/image/eraser1.png");
    eraserImage = eraserImage.scaled(32, 32, Qt::KeepAspectRatio);
    QCursor eraserCursor(eraserImage, 1,25);
    setCursor(eraserCursor);
}

void ImageViewEditor::pickerCursor(){
    QPixmap pickerImage(":/image/image/picker1.png");
    pickerImage = pickerImage.scaled(32, 32, Qt::KeepAspectRatio);
    QCursor pickerCursor(pickerImage, 1,25);
    setCursor(pickerCursor);
}

void ImageViewEditor::bucketCursor(){
    QPixmap bucketImage(":/image/image/bucket1.png");
    bucketImage = bucketImage.scaled(32, 32, Qt::KeepAspectRatio);
    QCursor bucketCursor(bucketImage, 1,1);
    setCursor(bucketCursor);
}


