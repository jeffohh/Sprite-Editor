#include "canvasLabel.h"
#include "QtGui/qevent.h"
#include <ostream>

canvasLabel::canvasLabel(QWidget *parent)
    : QLabel(parent)
{
    setMouseTracking(true);
}

void canvasLabel::mousePressEvent(QMouseEvent *ev)
{
//    int x = ev->position().x();
//    int y = ev->position().y();
//    emit setcoordinate(x, y);
    QString x = QString::number(ev->position().x());
    QString y = QString::number(ev->position().y());
    setText("Clicked: " + x + ", " + y);
}

//void canvas::paint(int x, int y)
//{
//    canvas.setPixel(x, y, qRgb(100,150,200));
//}
