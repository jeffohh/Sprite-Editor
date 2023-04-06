#include "draganddropevents.h"

/*Code reviewed by Andy Tran, Andy Duong*/
dragAndDropEvents::dragAndDropEvents(){

}

DragAndDropGraphicsView::DragAndDropGraphicsView(QWidget *widget): QGraphicsView(widget){
    setAcceptDrops(true);
}

void DragAndDropGraphicsView::dragEnterEvent(QDragEnterEvent *event)
{
    event->accept();
    event->acceptProposedAction();

}

void DragAndDropGraphicsView::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();

}

void DragAndDropGraphicsView::dragMoveEvent(QDragMoveEvent *event)
{
    event->accept();
    event->acceptProposedAction();
}

void DragAndDropGraphicsView::dropEvent(QDropEvent *event)
{
    if(event->source() == this) return;
    QLabel *label =qobject_cast<QLabel*>(event->source());
    QPixmap labelImage = label->pixmap();
    emit itemDrop(labelImage, this);
}

void DragAndDropGraphicsView::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    emit customColorIsSelected(this);
}

DragAndDropLabel::DragAndDropLabel(QWidget *widget)
{
    Q_UNUSED(widget)
}


void DragAndDropLabel::createDrag(const QPoint &pos, QWidget *widget)
{
    if(widget ==nullptr) return;
    QByteArray byteArray(reinterpret_cast<char*>(&widget), sizeof(QWidget*));
    QMimeData *mimedata = new QMimeData();
    mimedata->setData("Label", byteArray);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimedata);

    QPoint globalpos = mapToGlobal(pos);
    QPoint p = widget->mapFromGlobal(globalpos);

    drag->setHotSpot(p);
    drag->setPixmap(widget->grab());
    drag->exec();

}

void DragAndDropLabel::mousePressEvent(QMouseEvent *event)
{
    createDrag(event->pos(), this);
}
