#include "frameview.h"
#include "model.h"
#include <QMouseEvent>


FrameView::FrameView(QWidget *parent) :
    QGraphicsView(parent)
{
    setFocusPolicy(Qt::StrongFocus);
    this->setFixedSize(64, 64);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    this->index = Model::frameIndex++;

    imageItem = new QGraphicsPixmapItem();

    QGraphicsScene* scene = new QGraphicsScene();
    scene->addItem(imageItem);

    this->setScene(scene);

}

void FrameView::updatePixmap(QImage* image) {
    imageItem->setPixmap(QPixmap::fromImage(*image));
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
    if (event->key() == Qt::Key_Delete) {
        emit deletePressed(index);
    }
}
