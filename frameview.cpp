#include "frameview.h"
#include "model.h"

/* All the below code had been reviewed by Andy Duong */
FrameView::FrameView(QWidget *parent) :
    QGraphicsView(parent)
{
    this->setFixedSize(64, 64);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    this->index = Model::frameIndex++;

    imageItem = new QGraphicsPixmapItem();

    this->setStyleSheet("border: 1px solid grey;");

    QGraphicsScene* scene = new QGraphicsScene();
    scene->addItem(imageItem);

    this->setScene(scene);

}

void FrameView::updatePixmap(QImage* image) {
    imageItem->setPixmap(QPixmap::fromImage(*image));
    setSceneRect(imageItem->boundingRect());
    centerOn(imageItem->boundingRect().center());
}

void FrameView::updateIndex(int newIndex){
    index = newIndex;
}

int FrameView::getIndex(){
    return index;
}

void FrameView::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        emit mouseClicked(imageItem, index);
    }
}

void FrameView::keyPressEvent(QKeyEvent *event)
{
    if ((event->key() == Qt::Key_Delete)||(event->key() == Qt::Key_Backspace)) {
        emit deletePressed(index);
    }
}
