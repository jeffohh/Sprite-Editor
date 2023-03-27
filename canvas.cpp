#include "canvas.h"
#include <QMouseEvent>

canvas::canvas(QWidget *parent):
    QGraphicsView(parent)
{

}

void canvas::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        emit viewClicked();
    }
    QGraphicsView::mousePressEvent(event);
}


