#include "frameview.h"
#include "model.h"

/**
 * @author AndyTran
 * @brief FrameView::FrameView A class represent the view of the frames
 * @param parent
 */
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

/**
 * @brief FrameView::updatePixmap Update the image for this frame
 * @param image
 */
void FrameView::updatePixmap(QImage* image) {
    imageItem->setPixmap(QPixmap::fromImage(*image));
    setSceneRect(imageItem->boundingRect());
    centerOn(imageItem->boundingRect().center());
}

/**
 * @brief FrameView::updateIndex Update the index of this frame
 * @param newIndex New index wants to be updated
 */
void FrameView::updateIndex(int newIndex){
    index = newIndex;
}

/**
 * @brief FrameView::getIndex Get the current index of this frame
 * @return The current Index
 */
int FrameView::getIndex(){
    return index;
}

/**
 * @brief FrameView::mousePressEvent Trigger whenever the left button is clicked.
 * Sending to the Model to handle the event
 * @param event
 */
void FrameView::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        emit mouseClicked(imageItem, index);
    }
}

/**
 * @brief FrameView::keyPressEvent Trigger whenever the delete button (Window) or back-space button (MacOS) is clicked.
 *  Sending to the Model to handle the event
 * @param event
 */
void FrameView::keyPressEvent(QKeyEvent *event)
{
    if ((event->key() == Qt::Key_Delete)||(event->key() == Qt::Key_Backspace)) {
        emit deletePressed(index);
    }
}
