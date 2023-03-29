#include "imagevieweditor.h"
#include <QMouseEvent>

ImageViewEditor::ImageViewEditor(QWidget *parent) :
    QGraphicsView(parent)
{
    setStyleSheet("background-color: grey;");

    setTransform(QTransform().scale(scale, scale));

    imageItem = new QGraphicsPixmapItem();

    QGraphicsScene* scene = new QGraphicsScene();
    scene->addItem(imageItem);

    this->setScene(scene);
}

void ImageViewEditor::updatePixmap(QImage* image) {
    imageItem->setPixmap(QPixmap::fromImage(*image));
}

void ImageViewEditor::mousePressEvent(QMouseEvent *) {
//    if (event->button() == Qt::LeftButton) {
//        QPoint pos = mapToScene(event->pos()).toPoint();
//        emit mouseDown(pos);

//    }

      emit mousePressed(true);
}

void ImageViewEditor::mouseMoveEvent(QMouseEvent *event) {

    //Andy Tran Edited
    QPoint pos = mapToScene(event->pos()).toPoint();
    emit mouseDown(pos);

}

void ImageViewEditor::mouseReleaseEvent(QMouseEvent *) {
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
